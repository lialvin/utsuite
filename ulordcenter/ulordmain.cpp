#include <stdio.h>
#include "include/a.h"
#include "readcfg.h"
#include "ulordmain.h"
#include <string>
#include "server.cc"
#include "mastenode.h"
#include "connection_pool.h"
#include "initwallet.h"
using namespace std;
void  test();
void  testdb();
int main(int argc, char* argv[])
{
   std::string  dbuser ,dbhost, dbpass;
   int32_t nPort ;
   int nThread; 
   readcfg(nPort,nThread,dbuser,dbhost,dbpass);

   LOG_INFO << "pid = " << getpid()<< "main chainparam ";
   Secp256k1Init eccinit("main");
   
   ConnPool::GetInstance(dbuser, dbpass , nThread*2); 
   //testdb();
   {
     EventLoop loop;
     uint16_t port = static_cast<uint16_t>(nPort);
     InetAddress serverAddr(port);
     UlordServer server(&loop,60, serverAddr);
     if (nThread> 1)
     {
        server.setThreadNum(nThread);
     }
     server.start();
     initreaddb();
     loop.loop();
   }
    
}

void  testdb()
{
    ConnPool* pPool =  ConnPool::GetInstance(); 
    sql::Connection *  pConn = pPool->GetConnection();
    std::vector<CMstNodeData>vecnode;
    ReadMasterNodeToNet(pConn, std::string("NdsRM9waShDUT3TqhgdsGCzqH33Wwb8zDB"), vecnode);
    ReadAllNodeToNet(pConn, vecnode);

}

void test()
{
   test_mstnodequest test1;
   test1.testAr(); 
   double b=25.0;
   double a=0.0;
   char szBuf[128];
   sprintf(szBuf  ,"a is %lf,b is %lf\n" , a,b);

}
