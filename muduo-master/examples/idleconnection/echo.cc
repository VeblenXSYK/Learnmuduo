#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <assert.h>
#include <stdio.h>

#include "echo.h"

using namespace muduo;
using namespace muduo::net;


EchoServer::EchoServer(EventLoop* loop,
                       const InetAddress& listenAddr,
                       int idleSeconds)
	: server_(loop, listenAddr, "EchoServer"),
	  connectionBuckets_(idleSeconds)
{
	server_.setConnectionCallback(
	    boost::bind(&EchoServer::onConnection, this, _1));
	server_.setMessageCallback(
	    boost::bind(&EchoServer::onMessage, this, _1, _2, _3));
	loop->runEvery(1.0, boost::bind(&EchoServer::onTimer, this));
	connectionBuckets_.resize(idleSeconds);
	dumpConnectionBuckets();
}

void EchoServer::start()
{
	server_.start();
}

void EchoServer::onConnection(const TcpConnectionPtr& conn)
{
	LOG_INFO << "EchoServer - " << conn->peerAddress().toIpPort() << " -> "
	         << conn->localAddress().toIpPort() << " is "
	         << (conn->connected() ? "UP" : "DOWN");

	if (conn->connected()) {
		EntryPtr entry(new Entry(conn));
		connectionBuckets_.back().insert(entry);
		dumpConnectionBuckets();
		WeakEntryPtr weakEntry(entry);
		conn->setContext(weakEntry);				// 保存Entry的弱引用到TCP连接的上下文，便于之后在读消息的时候取出
	} else {
		assert(!conn->getContext().empty());
		WeakEntryPtr weakEntry(boost::any_cast<WeakEntryPtr>(conn->getContext()));
		LOG_DEBUG << "Entry use_count = " << weakEntry.use_count();
	}
}

void EchoServer::onMessage(const TcpConnectionPtr& conn,
                           Buffer* buf,
                           Timestamp time)
{
	string msg(buf->retrieveAllAsString());
	LOG_INFO << conn->name() << " echo " << msg.size()
	         << " bytes at " << time.toString();
	conn->send(msg);

	assert(!conn->getContext().empty());
	WeakEntryPtr weakEntry(boost::any_cast<WeakEntryPtr>(conn->getContext()));
	EntryPtr entry(weakEntry.lock());
	if (entry) {
		connectionBuckets_.back().insert(entry);
		dumpConnectionBuckets();
	}
}

void EchoServer::onTimer()
{
	connectionBuckets_.push_back(Bucket());			// 添加新的无序集合到缓冲区末尾，第一个无序集合被移出缓冲区
	dumpConnectionBuckets();
}

void EchoServer::dumpConnectionBuckets() const
{
	LOG_INFO << "size = " << connectionBuckets_.size();
	int idx = 0;
	for (WeakConnectionList::const_iterator bucketI = connectionBuckets_.begin();
	     bucketI != connectionBuckets_.end();
	     ++bucketI, ++idx) 
	{
		const Bucket& bucket = *bucketI;
		printf("[%d] len = %zd : ", idx, bucket.size());
		for (Bucket::const_iterator it = bucket.begin(); it != bucket.end(); ++it)
		{
			// expired()的功能等价于use_count() == 0，但更快，表示观测的资源（也就是shared_ptr管理的资源）已经不复存在了
			bool connectionDead = (*it)->weakConn_.expired();
			printf("%p(%ld)%s, ", get_pointer(*it), it->use_count(), connectionDead ? " DEAD" : "");
		}
		puts("");
	}
}

