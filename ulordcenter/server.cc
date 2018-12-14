#include "codec.h"

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>

#include <boost/bind.hpp>

#include <set>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include "nodesql.h"

using namespace muduo;
using namespace muduo::net;

class UlordServer : boost::noncopyable
{
 public:
  UlordServer(EventLoop* loop,int idleSeconds,
             const InetAddress& listenAddr)
  : server_(loop, listenAddr, "UlordServer"), idleSeconds_(idleSeconds),
    codec_(boost::bind(&UlordServer::onStringMessage, this, _1, _2, _3))
  {
    server_.setConnectionCallback(
        boost::bind(&UlordServer::onConnection, this, _1));
    server_.setMessageCallback(
        boost::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
    loop->runEvery(1.0, boost::bind(&UlordServer::onTimer, this));
    dumpConnectionList();
  }

  typedef boost::weak_ptr<TcpConnection> WeakTcpConnectionPtr;
  typedef std::list<WeakTcpConnectionPtr> WeakConnectionList;

  void setThreadNum(int numThreads)
  {
    server_.setThreadNum(numThreads);
  }
  void start()
  {
    server_.start();
  }

 private:
  void onConnection(const TcpConnectionPtr& conn)
  {
    LOG_INFO << conn->localAddress().toIpPort() << " -> "
             << conn->peerAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");
    if (conn->connected())
    { 
      Node node;
      node.lastReceiveTime = Timestamp::now();
      connectionList_.push_back(conn);
      node.position = --connectionList_.end();
      conn->setContext(node);
    }
    else
    { 
      assert(!conn->getContext().empty());
      const Node& node = boost::any_cast<const Node&>(conn->getContext());
      connectionList_.erase(node.position);
    }
    dumpConnectionList();
  }

  void onMessage(const TcpConnectionPtr& tcpcli, Buffer* buf,Timestamp time)
  {
      //LOG_INFO << "receive msg  "<< buf->length();
     // ParseQuest(tcpcli, message ,codec_)
  }

  void onStringMessage(const TcpConnectionPtr&  tcpcli,
                       const std::string& message,
                       Timestamp)
  {
    LOG_INFO << "receive msg  "<< message.size(); 
    ParseQuest(tcpcli, message ,codec_);

    // noever 
   /* assert(!tcpcli->getContext().empty());
    Node* node = boost::any_cast<Node>(tcpcli->getMutableContext());
    node->lastReceiveTime = time;
    connectionList_.splice(connectionList_.end(), connectionList_, node->position);
    assert(node->position == --connectionList_.end());

    dumpConnectionList(); */

  }

  void onTimer()
  {
      dumpConnectionList();
      Timestamp now = Timestamp::now();
      for (WeakConnectionList::iterator it = connectionList_.begin();
         it != connectionList_.end();)
      {
          TcpConnectionPtr conn = it->lock();
          if (conn)
          {
              Node* n = boost::any_cast<Node>(conn->getMutableContext());
              double age = timeDifference(now, n->lastReceiveTime);
              if (age > idleSeconds_)
              {
                  if (conn->connected())
                  {
                      conn->shutdown();
                      LOG_INFO << "shutting down " << conn->name();
                      conn->forceCloseWithDelay(3.5);  // > round trip of the whole Internet.
                  }
              }
              else if (age < 0)
              {
                   LOG_WARN << "Time jump";
                   n->lastReceiveTime = now;
              }
              else
              {
                  break;
              }
              ++it;
         }
         else
         {
             LOG_WARN << "Expired";
             it = connectionList_.erase(it);
         }
     }
 }
  
  void dumpConnectionList() const
  {

     //LOG_INFO << "size = " << connectionList_.size();

     for (WeakConnectionList::const_iterator it = connectionList_.begin();
        it != connectionList_.end(); ++it)
     {
         TcpConnectionPtr conn = it->lock();
         if (conn)
         {
             printf("conn %p\n", get_pointer(conn));
             const Node& n = boost::any_cast<const Node&>(conn->getContext());
             printf("    time %s\n", n.lastReceiveTime.toString().c_str());
         }
         else
         {
             printf("expired\n");
         }
     }

  }


  struct Node : public muduo::copyable
  { 
    Timestamp lastReceiveTime;
    WeakConnectionList::iterator position;
  };

  typedef std::set<TcpConnectionPtr> ConnectionList;
  int idleSeconds_;
  TcpServer server_;
  LengthHeaderCodec codec_;
  ConnectionList connections_;
  WeakConnectionList connectionList_;
};

