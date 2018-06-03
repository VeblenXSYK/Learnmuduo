#include "Connector.h"

#include "Channel.h"
#include "EventLoop.h"
#include "SocketsOps.h"

#include "logging/Logging.h"

#include <boost/bind.hpp>

#include <errno.h>

using namespace muduo;

const int Connector::kMaxRetryDelayMs;

Connector::Connector(EventLoop* loop, const InetAddress& serverAddr)
	: loop_(loop),
	  serverAddr_(serverAddr),
	  connect_(false),
	  state_(kDisconnected),
	  retryDelayMs_(kInitRetryDelayMs)
{
	LOG_DEBUG << "ctor[" << this << "]";
}

Connector::~Connector()
{
	LOG_DEBUG << "dtor[" << this << "]";
	loop_->cancel(timerId_);		//注销定时器，避免在定时器到期前Connector析构了
	assert(!channel_);
}

void Connector::start()
{
	connect_ = true;
	loop_->runInLoop(boost::bind(&Connector::startInLoop, this)); // FIXME: unsafe
}

void Connector::startInLoop()
{
	loop_->assertInLoopThread();
	assert(state_ == kDisconnected);
	if (connect_) {
		connect();
	} else {
		LOG_DEBUG << "do not connect";
	}
}

/*
	TCP套接口被设置为非阻塞之后调用connect，connect会立即返回EINPROGRESS错误,
	表示连接操作正在进行中，但是仍未完成；同时TCP的三路握手操作继续进行；
	在这之后，我们可以调用poll来检查这个链接是否建立成功
*/
void Connector::connect()
{
	int sockfd = sockets::createNonblockingOrDie();
	int ret = sockets::connect(sockfd, serverAddr_.getSockAddrInet());
	int savedErrno = (ret == 0) ? 0 : errno;
	switch (savedErrno) {
	case 0:
	case EINPROGRESS:
	case EINTR:
	case EISCONN:
		LOG_DEBUG << "savedErrno：" << strerror(errno);
		connecting(sockfd);
		break;

	case EAGAIN:
	case EADDRINUSE:
	case EADDRNOTAVAIL:
	case ECONNREFUSED:
	case ENETUNREACH:
		retry(sockfd);
		break;

	case EACCES:
	case EPERM:
	case EAFNOSUPPORT:
	case EALREADY:
	case EBADF:
	case EFAULT:
	case ENOTSOCK:
		LOG_SYSERR << "connect error in Connector::startInLoop " << savedErrno;
		sockets::close(sockfd);
		break;

	default:
		LOG_SYSERR << "Unexpected error in Connector::startInLoop " << savedErrno;
		sockets::close(sockfd);
		// connectErrorCallback_();
		break;
	}
}

void Connector::restart()
{
	loop_->assertInLoopThread();
	setState(kDisconnected);
	retryDelayMs_ = kInitRetryDelayMs;
	connect_ = true;
	startInLoop();
}

void Connector::stop()
{
	connect_ = false;
	loop_->cancel(timerId_);
}

void Connector::connecting(int sockfd)
{
	setState(kConnecting);
	assert(!channel_);
	channel_.reset(new Channel(loop_, sockfd));
	channel_->setWriteCallback(boost::bind(&Connector::handleWrite, this)); // FIXME: unsafe
	channel_->setErrorCallback(boost::bind(&Connector::handleError, this)); // FIXME: unsafe

	// channel_->tie(shared_from_this()); is not working,
	// as channel_ is not managed by shared_ptr
	/*
		连接成功建立时，socket描述字变为可写。（连接建立时，写缓冲区空闲，所以可写）
		连接建立失败时，socket描述字既可读又可写。（由于有未决的错误，从而可读又可写）
	*/
	channel_->enableWriting();
}

int Connector::removeAndResetChannel()
{
	channel_->disableAll();
	loop_->removeChannel(get_pointer(channel_));
	int sockfd = channel_->fd();
	// Can't reset channel_ here, because we are inside Channel::handleEvent
	loop_->queueInLoop(boost::bind(&Connector::resetChannel, this)); // FIXME: unsafe
	return sockfd;
}

void Connector::resetChannel()
{
	channel_.reset();
}

void Connector::handleWrite()
{
	LOG_TRACE << "Connector::handleWrite " << state_;

	if (state_ == kConnecting) {
		int sockfd = removeAndResetChannel();
		//即便出现socket可写，也不一定意味着连接已成功建立，还需要getsocketopt确认一下
		int err = sockets::getSocketError(sockfd);
		if (err) {
			LOG_WARN << "Connector::handleWrite - SO_ERROR = "
			         << err << " " << strerror_tl(err);
			retry(sockfd);
		} else if (sockets::isSelfConnect(sockfd)) {
			LOG_WARN << "Connector::handleWrite - Self connect";
			retry(sockfd);
		} else {
			setState(kConnected);
			if (connect_) {
				newConnectionCallback_(sockfd);
			} else {
				sockets::close(sockfd);
			}
		}
	} else {
		// what happened?
		assert(state_ == kDisconnected);
	}
}

void Connector::handleError()
{
	LOG_ERROR << "Connector::handleError";
	assert(state_ == kConnecting);

	int sockfd = removeAndResetChannel();
	int err = sockets::getSocketError(sockfd);
	LOG_TRACE << "SO_ERROR = " << err << " " << strerror_tl(err);
	retry(sockfd);
}

void Connector::retry(int sockfd)
{
	/*
		socket是一次性的，一旦出错(比如对方拒绝连接)，就无法恢复，只能关闭重来
	*/
	sockets::close(sockfd);
	setState(kDisconnected);
	if (connect_) {
		LOG_INFO << "Connector::retry - Retry connecting to " << serverAddr_.toHostPort() << " in " << retryDelayMs_ << " milliseconds. ";
		timerId_ = loop_->runAfter(retryDelayMs_/1000.0,  // FIXME: unsafe
		                           boost::bind(&Connector::startInLoop, this));
		//重试的时间间隔应该延长
		retryDelayMs_ = std::min(retryDelayMs_ * 2, kMaxRetryDelayMs);
	} else {
		LOG_DEBUG << "do not connect";
	}
}

