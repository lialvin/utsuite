#include <string>
#include <boost/log/trivial.hpp>  
#include <iostream>
#include <boost/lexical_cast.hpp>  
#include <boost/algorithm/string.hpp>

#include "codec.h"

#include "nodesql.h"
#include "mastenode.h"
#include "connection_pool.h"
#include "key.h"
#include "utilstrencodings.h"
#include "initwallet.h"
#include "messagesigner.h"

using namespace muduo;
using namespace muduo::net;

using namespace std;
using namespace boost;

std::string g_privkeystr;
std::string g_pubkeystr;

std::map<std::string , CMstNodeData> g_mstdatamap;
unsigned int  g_curtime;
const int   TIME_INTERVAL = 120;
bool FindFromMasterNodeMemory( std::string masteraddr, std::vector<CMstNodeData> & vecnode  )
{
    std::map<std::string, CMstNodeData>::iterator it  =   g_mstdatamap.find(masteraddr);
//   加个时间判断 超过一定的时间 就重新度数据库 
   if(it != g_mstdatamap.end())
   {
       CMstNodeData mstNodeData  = it->second;
       //if(mstNodeData._time > g_curtime  - TIME_INTERVAL) 
       {
          vecnode.push_back(it->second);
          return true;
       }
       //else // 超时 
       {
          g_mstdatamap.erase(it);
          return false;
       }
   }
   
   return false;
       
}

void   AddMasterNodeMemory(std::string masteraddr ,int  invalidflag  )
{
    CMstNodeData mstnode(0,masteraddr);
    mstnode._validflag= invalidflag; // 0 is invalid;  1 is valid
    mstnode._time = g_curtime; 

    std::map<std::string, CMstNodeData>::iterator it  =   g_mstdatamap.find(masteraddr);
   //   加个时间判断 超过一定的时间 就重新度数据库 
   if(it != g_mstdatamap.end())
   {
      it->second._validflag=invalidflag;
   }
   else
   {
      g_mstdatamap.insert(pair<std::string,CMstNodeData>(masteraddr,mstnode ));
   }  
}

void   AddMasterNodeMemory(std::string masteraddr , CMstNodeData &  node  )
{
   node._time = g_curtime;
   std::map<std::string, CMstNodeData>::iterator it  =   g_mstdatamap.find(masteraddr);
   if(it != g_mstdatamap.end())
   {
      it->second._validflag=node._validflag; 
   }
   else   
   {
       g_mstdatamap.insert(pair<std::string,CMstNodeData>(masteraddr,node )); 
   } 
}
// sign  addr
std::string  SignMessage( std::string addr, int64_t timeStamps)
{
   CKey keyRet;
   CPubKey pubkeyRet;
   GetKeyFromString(  keyRet,  pubkeyRet,  g_privkeystr,   g_pubkeystr ) ;
    
   CPubKey testKey(ParseHex(g_pubkeystr));

   cout<< "Pubkey: str=" << g_pubkeystr << endl
   		<< "center key=" << pubkeyRet.GetID().ToString() << endl
   		<< "ulord key=" << testKey.GetID().ToString() << endl;

    CMessageSigner messSign;
    CMessageSigner messVery;

    std::vector<unsigned char> vchSigRet;
    std::vector<unsigned char> vchkey;

    messSign.SignMessage(addr, timeStamps, vchSigRet, keyRet);
    cout<<"vchsigret "<< HexStr(vchSigRet)<<endl<<"vchsig "<< EncodeBase64(&vchSigRet[0], vchSigRet.size())<<endl<<"pubkey "<<pubkeyRet.GetID().ToString()<<endl;
    return HexStr(vchSigRet);
}


int   ParseQuest(const TcpConnectionPtr & tcpcli,const std::string &buf, LengthHeaderCodec& codec )
{
    // const TcpConnectionPtr  tcpcli;
    ConnPool* pPool =  ConnPool::GetInstance();
    g_curtime  = time(NULL);
    mstnodequest  mstquest;  
    std::istringstream is(buf);  
    boost::archive::binary_iarchive ia(is);  
    ia >> mstquest;//从一个保存序列化数据的string里面反序列化，从而得到原来的对象。 
    std::vector<CMstNodeData> vecnode;
    if(mstquest._questtype == MST_QUEST_ONE)
    {
       // 先从内存中找 如果找不到就从数据库找  超时连接要断掉 超过1分钟   
       if(FindFromMasterNodeMemory( mstquest._masteraddr, vecnode  )==true)
       {
           // find        
          cout << " FindFromMasterNodeMemory  true "   << endl;
       }
       else 
       {
           //sql::Connection *  pConn = pPool->GetConnection();
           sql::Connection *  pConn = pPool->CreateConnection();
           if(pConn==NULL)
           {    
               cout << "error  pConn  get is null "   << endl;
               return 0;
           }         
   
           ReadMasterNodeToNet(pConn, mstquest._masteraddr, vecnode); 
           if(vecnode.size()==0)
           {
              AddMasterNodeMemory(mstquest._masteraddr , false  );
              CMstNodeData   node(0,mstquest._masteraddr);
              node._validflag=0;
              cout << "find from sql   get is null "   << endl;
              
              vecnode.push_back( node);
           }
           else
           {
               AddMasterNodeMemory(mstquest._masteraddr , vecnode[0]  );
           } 
           
           cout << "find from sql   get  size  " << vecnode.size()  << endl;
            
           //pPool->ReleaseConnection(pConn);
           pPool->DestroyConnection(pConn);
       }
    }
    else if(mstquest._questtype ==MST_QUEST_ALL )
    {
       sql::Connection *  pConn = pPool->GetConnection();
       if(pConn==NULL)
       {
           cout << "error  pConn  get is null "   << endl;
           return 0;
       } 
       ReadAllNodeToNet(pConn, vecnode );
       pPool->ReleaseConnection(pConn);  

    }

    mstnoderes  mstres(mstquest._msgversion);
    mstres._num= vecnode.size();
    mstres._signstr = SignMessage(vecnode[0]._masteraddr, mstquest._timeStamps);
    std::ostringstream os;  
    boost::archive::binary_oarchive oa(os);  
    oa<<mstres; 
    for(auto node : vecnode) 
    {
        oa << node;//序列化到一个ostringstream里面  
    } 
    std::string content = os.str();//content保存了序列化后的数据。  
    muduo::StringPiece message(content);
    //codec.send(get_pointer(*tcpcli), message);
    codec.send(tcpcli, message);
    cout << "codes send finished "   << endl;
    return 0;
}

void initreaddb()
{
  std::vector<CMstNodeData> vecnode;
  ConnPool* pPool =  ConnPool::GetInstance();
  {
     sql::Connection *  pConn = pPool->GetConnection();
     if(pConn==NULL)
     {
        cout << "error  pConn  get is null "   << endl;
        return ;
     }
     ReadAllNodeToNet(pConn, vecnode );
     pPool->ReleaseConnection(pConn);

  }
}
//    std::cin.get();  
void ReadAllNodeToNet(sql::Connection * con,std::vector<CMstNodeData>& vecnode )
{
    char sqlBuf[500] = {""};

    sprintf(sqlBuf ,"select *from ulord_master ");

    sql::Statement * stmt = NULL;
    stmt = con->createStatement();
    if(stmt == NULL)
    {
        BOOST_LOG_TRIVIAL(info)<<"sql query   query ulord failed  ";
        cout << "query master node nodeaddr "   << endl;
    }
    stmt->execute("SET CHARSET GB2312");
    stmt->execute("USE mysql");
    CMstNodeData mstnode;
    sql::ResultSet * resultSet =  stmt->executeQuery(sqlBuf);
    int i = 0;
    while (resultSet->next()) {
        mstnode._masteraddr = resultSet->getString(1 ); // ultSet->getString("addr" )
        mstnode._hostname  = resultSet->getString(4);
        mstnode._hostip   = resultSet->getString(5);
        vecnode.push_back(mstnode);
        AddMasterNodeMemory(mstnode._masteraddr, mstnode  );
        cout<<"master addr  "<<  mstnode._masteraddr <<"hostname " << mstnode._hostname << "hostip  "<< mstnode._hostip <<endl;
        i++;
    }

    cout<<"total "<<i<<" record"<<endl;
    delete stmt;
    delete resultSet;

}

void  ReadMasterNodeToNet(sql::Connection * con, std::string nodeaddr,std::vector<CMstNodeData>& vecnode)
{
    char sqlBuf[500] = {""};

    sprintf(sqlBuf ,"select *from ulord_master where addr =\'%s\'",
          nodeaddr.c_str());

    sql::Statement * stmt = NULL;
    stmt = con->createStatement();
    if(stmt == NULL)
    {
        BOOST_LOG_TRIVIAL(info)<<"sql query" << "query ulord failed  ";
        cout << "query master node nodeaddr "   << endl;
   
    }
    stmt->execute("SET CHARSET GB2312");
    stmt->execute("USE mysql");
/*addr char(50) not null primary key,
amount bigint NOT NULL DEFAULT '0',
txid       char(50) null,
hostname   char(50) NULL DEFAULT ' ',
ip         char(50) NULL DEFAULT ' ',
disksize     int NOT NULL DEFAULT '0',
netsize      int NOT NULL DEFAULT '0',
cpusize      int NOT NULL DEFAULT '0',
ramsize      int NOT NULL DEFAULT '0',
score        int NOT NULL DEFAULT '0',
validflag             int NOT NULL DEFAULT '0',
 */
    CMstNodeData mstnode;
    sql::ResultSet * resultSet =  stmt->executeQuery(sqlBuf);
    int i = 0;
    while (resultSet->next()) {
        mstnode._masteraddr = resultSet->getString(1 ); // ultSet->getString("addr" )
        mstnode._hostname  = resultSet->getString(4);  
        mstnode._hostip   = resultSet->getString(5);  
		mstnode._validflag = resultSet->getInt(11); 
        cout<<"master addr "<<  mstnode._masteraddr <<" hostname " << mstnode._hostname << " hostip "<< mstnode._hostip << " validflag " << mstnode._validflag <<endl;
        vecnode.push_back(mstnode);
        i++;
    }
    
    //BOOST_LOG_TRIVIAL(info)<<"sql query","query btcoin_hight failed  ";
    // cout << "query master node g_sqlblockheight "   << endl;

    cout<<"total "<<i<<" record"<<endl;
    delete stmt;
    delete resultSet;

}
