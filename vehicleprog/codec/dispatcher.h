#ifndef PROTOBUF_DISPATCHER_H
#define PROTOBUF_DISPATCHER_H

#include <muduo/net/Callbacks.h>

#include <google/protobuf/message.h>

#include <map>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<google::protobuf::Message> MessagePtr;

class Callback : boost::noncopyable
{
public:
	virtual ~Callback() {};
	virtual void onMessage(const muduo::net::TcpConnectionPtr&,
	                       const MessagePtr& message,
	                       muduo::Timestamp) const = 0;
};

template <typename T>
class CallbackT : public Callback
{
public:
	typedef boost::function<void (const muduo::net::TcpConnectionPtr&,
	                              const boost::shared_ptr<T>& message,
	                              muduo::Timestamp)> ProtobufMessageTCallback;

	CallbackT(const ProtobufMessageTCallback& callback)
		: callback_(callback)
	{
	}

	virtual void onMessage(const muduo::net::TcpConnectionPtr& conn,
	                       const MessagePtr& message,
	                       muduo::Timestamp receiveTime) const
	{
		boost::shared_ptr<T> concrete = muduo::down_pointer_cast<T>(message);
		assert(concrete != NULL);
		callback_(conn, concrete, receiveTime);
	}

private:
	ProtobufMessageTCallback callback_;
};

class ProtobufDispatcher
{
public:
	typedef boost::function<void (const muduo::net::TcpConnectionPtr&,
	                              const MessagePtr& message,
	                              muduo::Timestamp)> ProtobufMessageCallback;

	explicit ProtobufDispatcher(const ProtobufMessageCallback& defaultCb)
		: defaultCallback_(defaultCb)
	{
	}

	void onProtobufMessage(const muduo::net::TcpConnectionPtr& conn,
	                       const MessagePtr& message,
	                       muduo::Timestamp receiveTime) const
	{
		CallbackMap::const_iterator it = callbacks_.find(message->GetDescriptor());
		if (it != callbacks_.end()) 
		{
			it->second->onMessage(conn, message, receiveTime);
		} 
		else 
		{
			defaultCallback_(conn, message, receiveTime);
		}
	}

	template<typename T>
	void registerMessageCallback(const typename CallbackT<T>::ProtobufMessageTCallback& callback)
	{
		boost::shared_ptr<CallbackT<T> > pd(new CallbackT<T>(callback));
		callbacks_[T::descriptor()] = pd;
	}

private:
	typedef std::map<const google::protobuf::Descriptor*, boost::shared_ptr<Callback> > CallbackMap;

	CallbackMap callbacks_;						//以键值对的形式存储不同消息的回调函数
	ProtobufMessageCallback defaultCallback_;	//默认的回调函数（当callbacks_中没有时则调用）
};

#endif
