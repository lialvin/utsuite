#ifndef BT_MYSQL____H
#define BT_MYSQL____H
#include <string>
#include <vector>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <pthread.h>
#include <list>
#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>

#include "codec.h"
#include "mastenode.h"

void   ReadAllNodeToNet(sql::Connection * con,std::vector<CMstNodeData>& vecnode );
void   ReadMasterNodeToNet( sql::Connection * con, std::string nodeaddr,std::vector<CMstNodeData>& vecnode);
int    ParseQuest(const muduo::net::TcpConnectionPtr & tcpcli, const std::string &buf, LengthHeaderCodec& codec );
void   initreaddb();
extern std::string  g_privkeystr;
extern std::string  g_pubkeystr;
#endif
