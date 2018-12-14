#ifndef MST_NODE_DATA_H
#define MST_NODE_DATA_H  

#include <iostream>
#include <string>
#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

using namespace std;


// master node  quest  master register center  about master node info
enum MST_QUEST  
{
    MST_QUEST_ONE=1,
    MST_QUEST_ALL=2

};

// master node quest version 
class  mstnodequest
{
public:
    mstnodequest(int version, MST_QUEST  type  ):_msgversion(version), _questtype(type)
    {
       _verfyflag=std::string("#$%@");  
    }  

    mstnodequest(){}
    int             _msgversion; 	
    int             _questtype; 	
	int64_t         _timeStamps;	
    std::string     _verfyflag;
    std::string     _masteraddr;
    friend class boost::serialization::access;
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {  
        ar & _verfyflag;
        ar & _msgversion;
		ar & _timeStamps;
        ar & _questtype;
        ar & _masteraddr;
        //ar & _llAmount;  
    }  
    int   GetVersion() const {return _msgversion;}  
    int   GetQuestType() const {return _questtype;}  
    void  SetMasterAddr(std::string addr){ _masteraddr=addr;}    

};
// master node quest version 
class  mstnoderes
{
public:
    mstnoderes(int version  ):_msgversion(version)
    {
       _verfyflag=std::string("#$%@");
       _num=1;
    }

    mstnoderes(){}

    int             _msgversion;
    int             _num;
    std::string     _verfyflag;
    std::string     _signstr;
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & _verfyflag;
        ar & _msgversion;
        ar & _num;
        ar & _signstr;  // 使用 查询的第一个地址来签名  。 
        //ar & _llAmount;  
    }
    int GetVersion() const {return _msgversion;}
    int GetNum() const {return _num;}
};

class test_mstnodequest
{
public:
// test
   void testAr() 
   {
      mstnodequest  d1(1111, MST_QUEST::MST_QUEST_ONE);  
      std::ostringstream os;  
      boost::archive::binary_oarchive oa(os);  
      oa << d1;//序列化到一个ostringstream里面  
  
      std::string content = os.str();//content保存了序列化后的数据。  
  
      mstnodequest  d2;  
      std::istringstream is(content);  
      boost::archive::binary_iarchive ia(is);  
      ia >> d2;//从一个保存序列化数据的string里面反序列化，从而得到原来的对象。  
  
      std::cout << "CMyData tag: " << d2.GetVersion() << ",  text: " << d2.GetQuestType() << "\n";
   }
};

class CMstNodeData  
{  
private:  
    friend class boost::serialization::access;  
  
    template<class Archive>  
    void serialize(Archive& ar, const unsigned int version)  
    {  
        ar & _version;  
        ar & _masteraddr;  
        //ar & _txid;  
        ar & _hostname;  
        ar & _hostip;  
        ar & _validflag;
        //ar & _llAmount;  
    }  
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
 */ 
      
public:  
    CMstNodeData():_version(0), _masteraddr(""){}  
  
    CMstNodeData(int version, std::string addr):_version(version), _masteraddr(addr){}  
  
    int GetVersion() const {return _version;}  
    int GetValidFlag() const {return _validflag;}  
    std::string GetMasterAddr() const {return _masteraddr;}  

    CMstNodeData & operator=(CMstNodeData &b)
    {
        _version   = b._version;
        _masteraddr= b._masteraddr;
        _hostname  = b._hostname;
        _hostip    = b._hostip;
        _validflag = b._validflag;
        return * this;
    }
public:  
    int _version;  
    std::string _masteraddr; // node addr
    std::string _txid;      //  
    std::string _hostname;  // 
    std::string _hostip;    // 
    int         _validflag; //
    int         _time;
    long long   _llAmount;  // 
    std::string _text;  
};  

#endif
