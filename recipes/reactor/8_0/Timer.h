#ifndef MUDUO_NET_TIMER_H
#define MUDUO_NET_TIMER_H

#include <boost/noncopyable.hpp>

#include "datetime/Timestamp.h"
#include "Callbacks.h"

namespace muduo
{

///
/// Internal class for timer event.
///
class Timer : boost::noncopyable
{
public:
	Timer(const TimerCallback& cb, Timestamp when, double interval)
		: callback_(cb),
		  expiration_(when),
		  interval_(interval),
		  repeat_(interval > 0.0)
	{ }

	void run() const
	{
		callback_();
	}

	Timestamp expiration() const
	{
		return expiration_;
	}
	bool repeat() const
	{
		return repeat_;
	}

	void restart(Timestamp now);	//重新启动定时器

private:
	const TimerCallback callback_;	//到期回调函数
	Timestamp expiration_;			//到期时间戳
	const double interval_;			//定时时间
	const bool repeat_;				//是否周期性定时
};

}
#endif  // MUDUO_NET_TIMER_H
