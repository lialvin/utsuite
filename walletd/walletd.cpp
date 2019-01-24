#include <stdio.h>
#include "include/a.h"
#include "readcfg.h"
#include "walletd.h"
#include "cmdmsg.h"
#include <string>
#include "loopbuf.h"
#include "wallet_pdu.h"

#include "key.h"
#include "utilstrencodings.h"
#include "initwallet.h"
#include "messagesigner.h"
#include <secp256k1_ecdh.h>
#include <secp256k1.h>
#include "base58.h"
#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace std;

void printhelp()
{
       cout<<"1  help  :"  <<endl;
       cout<<"2  getnewaddress"<<endl;
       cout<<"3  dumpwallet"<<endl;
       cout<<"4  dumpprivkey  address"<<endl;
       cout<<"5  importkey prikey "<<endl;
       cout<<"6  importwallet"<<endl;
       cout<<"7  createmultisig addr 1 addr 2 "<<endl;
       cout<<"8  extractdestfromscript "<<endl;
       cout<<"9  signmnpmsg collkey mnprivkey ipaddr"<<endl;
       cout<<"10  genlic privkey mnprivkey txid itime txididx"<<endl;
       cout<<"11  testjson "<<endl;
       cout<<"12  createrawtx"<<endl;
       cout<<"13  signrawtx"<<endl;
       cout<<"14  uos2utaddr"<<endl;

}

//
std::string  ExtractDestFromScript(std::string str_script  )
{
     auto script =   ParseHex(str_script);

     CTxDestination address1;
     CScript script_(script.begin(),script.end() );  

     ExtractDestination(script_, address1);
     CBitcoinAddress address2(address1);
     return address2.ToString();
}


int main(int argc, char* argv[])
{

   string  dbuser ,dbhost, dbpass;
   int testnet, endheight;
   readcfg(testnet,endheight,dbuser,dbhost,dbpass);
  
    //Secp256k1Init ecc("main");
    char netparams[20]="";
    if(testnet==1)
       strcpy(netparams,"test"); 
    else
       strcpy(netparams,"main");


    cout<<"net params   :"<< netparams <<endl;
     
    Secp256k1Init ecc(netparams);
    initwallet();

    
    if(argc<2) // help 
    {
       printhelp();
       return 0;
    }     

    string stropt;
    if(argc>1)  
    {
        stropt = string(argv[1]);
    }
    
     
        
    string pubkey,strprivhexkey,strpubkeyid;
    if(stropt==string("getnewaddress") ) 
    {
       string addr = getnewaddress("", pubkey);
       cout<<"get new address addr=  "<<addr << " pubkey  " << pubkey<<endl;
    } 
    else if(stropt==string("dumpwallet") ) 
    {
       string  strWalletFile(argv[2]);
       dumpwallet(strWalletFile);
       cout<<"dump wallet   "<<strWalletFile <<endl;
    } 
    else if(stropt==string("dumpprivkey") ) 
    {
       string  straddr(argv[2]);
       string privkey= dumpprivkey(straddr,pubkey,strprivhexkey,strpubkeyid);
       cout<<"dump base58pubkey "<<straddr<<" base58 priv key "  << privkey   <<endl;
       cout<<"  hex priv key  " <<  strprivhexkey <<  "   hex pub key  "<< pubkey  << endl; 
       cout<<"  strpubkeyid  " <<  strpubkeyid   << endl; 
    }   
    else if(stropt==string("importkey") )
    {
       string  strkey(argv[2]);
       string  addr;
       bool bret= importprivkey(strkey ,addr);
       cout<<"import privkey  " << strkey  <<"  addr  "<< addr   <<endl;
    }
    else if(stropt==string("importwallet") ) 
    {
      string  strWalletFile(argv[2]);
      importwallet(strWalletFile);
       cout<<"import wallet  "<<strWalletFile <<endl;
    } 
    else if(stropt==string("createmultisig") )
    {
        string  straddr1(argv[2]);
        string  straddr2(argv[3]);
        string  strhexaddr3;
        string  straddr3 = createmultisig( straddr1 , straddr2 ,strhexaddr3 );
        cout<<"createmultisig "<<straddr3 <<"   hex str   " <<  strhexaddr3 <<endl;
    } 
    else if(stropt==string("extractdestfromscript"))
    {
        string  str_script(argv[2]); 
        string addr = ExtractDestFromScript( str_script);
    }
    else if(stropt==string("signmnpmsg"))
    {
        string  strPrivKey(argv[2]); 
        string  strMasterKey(argv[3]); 
        string  straddr(argv[4]); 
 
        string sign_str = signmnpmessage( strPrivKey , strMasterKey, straddr);
        cout<<"signmnpmsg=="<< sign_str << endl;
    }
    else if(stropt==string("genlic"))
    {
       string  strPrivKey(argv[2]);
       string  mnprivkey(argv[3]);
       string  strTxid(argv[4]);
       int64_t   idate =boost::lexical_cast<int64_t>(argv[5]);
       int     txididx= boost::lexical_cast<int>(argv[6]) ;
       string lic_ret= signlicmessage(strPrivKey,strTxid,mnprivkey,idate,txididx);
       cout<<"lic_ret="<<lic_ret<<endl;
    }
    else if(stropt==string("producekey"))
    {
        string  strcount(argv[2]);
        producekey(strcount);
    }
    else if(stropt==string("testjson"))
    {
        string  strjson(argv[2]);
        testjson( strjson ); 
        if(argc>3)
        {
           string  strjsonout(argv[3]);
           testjson( strjsonout ); 
        }
    }
    else if(stropt==string("createrawtx"))
    {
        string  strjsonin(argv[2]);
        string  strjsonout(argv[3]);
        string out = createrawtx( strjsonin, strjsonout); 
        cout << out<<endl; 
    }
    else if(stropt==string("signrawtx"))
    {
        string  strjsonhex(argv[2]);
        string  strjsontx(argv[3]);
        string  strjsonkey(argv[4]);
        cout <<strjsonhex << " hex "<<  strjsontx <<" param 3 "<< strjsonkey <<"  " <<endl; 
        testjson( strjsonhex ); 
    }
    else if(stropt==string("uos2utaddr"))
    {
        string  strhex(argv[2]);
        string addr = transutaddr(strhex);
        cout<<" addr=" << addr<<endl;
    }
    else 
    {
       printhelp();
    } 
   
    exitwallet();
   
   system("pause");


   return 0 ;

   CLoopBuf * sendbuf =new CLoopBuf;
   CLoopBuf * recvbuf  = new CLoopBuf;

   sendbuf->Init(6553600 , "walletsend_buf",1 ,1 );
   recvbuf->Init(6553600 , "walletrecv_buf",1 ,1 );
  
   WALLET_PDU * pPdu = new  WALLET_PDU;    
   int   nPduBufSize = sizeof(WALLET_PDU);
   while(1) 
   {
        recvbuf->PopOut((unsigned  char* )pPdu,nPduBufSize);
         
        sendbuf->PushIn((unsigned char*)pPdu, nPduBufSize);
   }

    
   char szBuf[128];
   return 0;
}
