#ifndef MUDUO_EXAMPLES_IDLECONNECTION_ECHO_H
#define MUDUO_EXAMPLES_IDLECONNECTION_ECHO_H

#include <muduo/net/TcpServer.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/unordered_set.hpp>

// RFC 862
class EchoServer
{
public:
	EchoServer(muduo::net::EventLoop* loop,
	           const muduo::net::InetAddress& listenAddr,
	           int idleSeconds);

	void start();

private:
	void onConnection(const muduo::net::TcpConnectionPtr& conn);

	void onMessage(const muduo::net::TcpConnectionPtr& conn,
	               muduo::net::Buffer* buf,
	               muduo::Timestamp time);

	void onTimer();

	void dumpConnectionBuckets() const;

	typedef boost::weak_ptr<muduo::net::TcpConnection> WeakTcpConnectionPtr;

	struct Entry : public muduo::copyable {
		explicit Entry(const WeakTcpConnectionPtr& weakConn)
			: weakConn_(weakConn)
		{
		}

		~Entry()
		{
			muduo::net::TcpConnectionPtr conn = weakConn_.lock();
			if (conn) {
				conn->shutdown();
			}
		}

		WeakTcpConnectionPtr weakConn_;
	};
	typedef boost::shared_ptr<Entry> EntryPtr;
	typedef boost::weak_ptr<Entry> WeakEntryPtr;
	typedef boost::unordered_set<EntryPtr> Bucket;
	typedef boost::circular_buffer<Bucket> WeakConnectionList;		// 定义一个环形缓冲区，其中每一个元素为一个无序的集合，集合中每一个元素指向Entry的强引用，Entry中存放指向TCP连接的弱引用

	muduo::net::TcpServer server_;
	WeakConnectionList connectionBuckets_;
};

#endif  // MUDUO_EXAMPLES_IDLECONNECTION_ECHO_H
