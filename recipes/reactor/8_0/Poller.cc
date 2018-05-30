#include <assert.h>
#include <poll.h>
#include "Poller.h"
#include "Channel.h"
#include "logging/Logging.h"


using namespace muduo;

Poller::Poller(EventLoop* loop) : ownerLoop_(loop)
{
}

Poller::~Poller()
{
}

/*
	获取当前活动的IO事件传入activeChannels
*/
Timestamp Poller::poll(int timeoutMs, ChannelList *activeChannels)
{
	// XXX pollfds_ shouldn't change
	int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeoutMs);
	Timestamp now(Timestamp::now());
	if (numEvents > 0) {
		LOG_TRACE << numEvents << " events happended";
		fillActiveChannels(numEvents, activeChannels);
	} else if (numEvents == 0) {
		LOG_TRACE << " nothing happended";
	} else {
		LOG_SYSERR << "Poller::poll()";
	}
	return now;
}

void Poller::fillActiveChannels(int numEvents, ChannelList* activeChannels) const
{
	for (PollFdList::const_iterator pfd = pollfds_.begin(); pfd != pollfds_.end() && numEvents > 0; ++pfd) {
		if (pfd->revents > 0) {
			--numEvents;
			ChannelMap::const_iterator ch = channels_.find(pfd->fd);
			assert(ch != channels_.end());
			Channel* channel = ch->second;
			assert(channel->fd() == pfd->fd);
			channel->set_revents(pfd->revents);
			// pfd->revents = 0;
			activeChannels->push_back(channel);
		}
	}
}

/*
	负责在pollfds_数组中添加或者更新Channel
*/
void Poller::updateChannel(Channel* channel)
{
	assertInLoopThread();
	LOG_TRACE << "fd = " << channel->fd() << " events = " << channel->events();
	if (channel->index() < 0) {
		// a new one, add to pollfds_
		assert(channels_.find(channel->fd()) == channels_.end());	//map中存在按k索引的元素，则返回指向该元素的iterator；如果不存在则返回end()
		//设置pollfd
		struct pollfd pfd;
		pfd.fd = channel->fd();
		pfd.events = static_cast<short>(channel->events());
		pfd.revents = 0;
		pollfds_.push_back(pfd);
		//设置该Channel在pollfds_的下标
		int idx = static_cast<int>(pollfds_.size())-1;
		channel->set_index(idx);
		//以<文件描述符，Channel*>方式存储在ChannelMap中
		channels_[pfd.fd] = channel;
	} else {
		// update existing one
		assert(channels_.find(channel->fd()) != channels_.end());
		assert(channels_[channel->fd()] == channel);
		int idx = channel->index();
		assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
		struct pollfd& pfd = pollfds_[idx];
		//assert(pfd.fd == channel->fd() || pfd.fd == -1);
		assert(pfd.fd == channel->fd() || pfd.fd == -channel->fd() - 1);
		pfd.events = static_cast<short>(channel->events());
		pfd.revents = 0;
		if (channel->isNoneEvent()) {
			// ignore this pollfd
			//pfd.fd = -1;
			pfd.fd = -channel->fd() - 1;
		}
	}
}

/*
	负责在pollfds_数组中删除Channel
*/
void Poller::removeChannel(Channel* channel)
{
	assertInLoopThread();
	LOG_TRACE << "fd = " << channel->fd();
	assert(channels_.find(channel->fd()) != channels_.end());
	assert(channels_[channel->fd()] == channel);
	assert(channel->isNoneEvent());
	int idx = channel->index();
	assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
	const struct pollfd& pfd = pollfds_[idx];
	(void)pfd;
	assert(pfd.fd == -channel->fd()-1 && pfd.events == channel->events());
	size_t n = channels_.erase(channel->fd());
	assert(n == 1);
	(void)n;
	if (implicit_cast<size_t>(idx) == pollfds_.size()-1) {
		//假如恰好在最后
		pollfds_.pop_back();
	} else {
		int channelAtEnd = pollfds_.back().fd;
		//将待删除的元素与最后一个元素交换
		iter_swap(pollfds_.begin()+idx, pollfds_.end()-1);
		if (channelAtEnd < 0) {
			channelAtEnd = -channelAtEnd-1;
		}
		channels_[channelAtEnd]->set_index(idx);
		pollfds_.pop_back();
	}
}

