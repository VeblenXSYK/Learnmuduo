#include "Channel.h"
#include "EventLoop.h"
#include "logging/Logging.h"

#include <sstream>

#include <poll.h>

using namespace muduo;

const int Channel::kNoneEvent = 0;
//#define POLLIN  0x001
//#define POLLPRI 0x002
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fdArg)
	: loop_(loop),
	  fd_(fdArg),
	  events_(0),
	  revents_(0),
	  index_(-1),
	  eventHandling_(false)
{
}

Channel::~Channel()
{
	//在事件处理期间本Channel对象不会析构
	assert(!eventHandling_);
}

void Channel::update()
{
	loop_->updateChannel(this);
}

void Channel::handleEvent()
{
	eventHandling_ = true;
	if (revents_ & POLLNVAL) {
		LOG_WARN << "Channel::handle_event() POLLNVAL";
	}
	if ((revents_ & POLLHUP) && !(revents_ & POLLIN)) {	//POLLHUP:发生挂起(当socket一端关闭时，另一端会捕获POLLUP事件)
		LOG_WARN << "Channel::handle_event() POLLHUP";
		if (closeCallback_) closeCallback_();
	}
	if (revents_ & (POLLERR | POLLNVAL)) {				//POLLNVAL:描述符不是一个打开的文件
		if (errorCallback_) errorCallback_();
	}
	if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {	//POLLPRI:高优先级数据可读 POLLRDHUP:代表对端断开连接
		if (revents_ | POLLRDHUP)
			LOG_WARN << "Channel::handle_event() POLLRDHUP";
		if (readCallback_) readCallback_();
	}
	if (revents_ & POLLOUT) {
		if (writeCallback_) writeCallback_();
	}
	eventHandling_ = false;
}
