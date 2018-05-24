#ifndef MUDUO_NET_TIMERID_H
#define MUDUO_NET_TIMERID_H

#include "datetime/copyable.h"

namespace muduo
{

class Timer;

///
/// An opaque identifier, for canceling Timer.
///
class TimerId : public muduo::copyable
{
public:
	//不允许隐式转换
	explicit TimerId(Timer* timer)
		: value_(timer)
	{
	}

	// default copy-ctor, dtor and assignment are okay

private:
	Timer* value_;
};

}

#endif  // MUDUO_NET_TIMERID_H
