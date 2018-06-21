#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <map>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "../Mutex.h"

using std::string;

class Stock : boost::noncopyable
{
public:
	Stock(const string& name) : name_(name)
	{
		printf(" Stock[%p] %s\n", this, name_.c_str());
	}
	
	~Stock()
	{
		printf("~Stock[%p] %s\n", this, name_.c_str());
	}
	
	const string& key() const
	{
		return name_;
	}
private:
	string name_;
};

/*
	����map����shared_ptr������Stock���󲻻ᱻ����
*/
namespace version1
{
class StockFactory : boost::noncopyable
{
public:
	boost::shared_ptr<Stock> get(const string& key)
	{
		muduo::MutexLockGuard lock(mutex_);
		boost::shared_ptr<Stock>& pStock = stocks_[key];
		printf(" pStock:%p\n", get_pointer(pStock));
		if(!pStock)
		{
			pStock.reset(new Stock(key));
		}
		
		return pStock;
	}
	
private:
	muduo::MutexLock mutex_;
	std::map<string, boost::shared_ptr<Stock> > stocks_;
};
}

/*
	map����weak_ptr
*/
namespace version2
{
class StockFactory : boost::noncopyable
{
public:
	boost::shared_ptr<Stock> get(const string& key)
	{
		boost::shared_ptr<Stock> pStock;
		muduo::MutexLockGuard lock(mutex_);
		boost::weak_ptr<Stock>& wkStock = stocks_[key];
		pStock = wkStock.lock();
		if(!pStock)
		{
			pStock.reset(new Stock(key));
			wkStock = pStock;
		}
		
		return pStock;
	}
	
private:
	muduo::MutexLock mutex_;
	std::map<string, boost::weak_ptr<Stock> > stocks_;
};
}

/*
	����shared_ptr�����������ܣ�����map��Stockռ�õĿռ�û���ͷ�
*/
namespace version3
{
class StockFactory : boost::noncopyable
{
public:
	boost::shared_ptr<Stock> get(const string& key)
	{
		boost::shared_ptr<Stock> pStock;
		muduo::MutexLockGuard lock(mutex_);
		boost::weak_ptr<Stock>& wkStock = stocks_[key];
		pStock = wkStock.lock();
		if(!pStock)
		{
			/*
				����shared_ptr�����������ܣ�����������ʱִ��deleteStock(ptr)��
				ptr��shared_ptr����Ķ���ָ��
			*/
			pStock.reset(new Stock(key), boost::bind(&StockFactory::deleteStock, this, _1));
			wkStock = pStock;
		}
		return pStock;
	}
	
	~StockFactory()
	{
		printf("~StockFactory\n");
	}
	
private:
	void deleteStock(Stock* stock)
	{
		printf("deleteStock[%p]\n", stock);
		if(stock)
		{
			//���map��Stockռ�õĿռ�
			muduo::MutexLockGuard lock(mutex_);
			stocks_.erase(stock->key());
		}
		delete stock;
	}
	
	muduo::MutexLock mutex_;
	std::map<string, boost::weak_ptr<Stock> > stocks_;
};
}

/*
	enable_shared_from_this�ӳ�StockFactory�����������
*/
namespace version4
{
class StockFactory : public boost::enable_shared_from_this<StockFactory>, boost::noncopyable
{
public:
	boost::shared_ptr<Stock> get(const string& key)
	{
		boost::shared_ptr<Stock> pStock;
		muduo::MutexLockGuard lock(mutex_);
		boost::weak_ptr<Stock>& wkStock = stocks_[key];
		pStock = wkStock.lock();
		if(!pStock)
		{
			/*
				shared_from_this():��ȡָ��ǰ����shared_ptr<StockFactory>����
			*/
			pStock.reset(new Stock(key), boost::bind(&StockFactory::deleteStock, shared_from_this(), _1));
			wkStock = pStock;
		}
		return pStock;
	}
	
	~StockFactory()
	{
		printf("~StockFactory\n");
	}

private:
	void deleteStock(Stock* stock)
	{
		printf("deleteStock[%p]\n", stock);
		if(stock)
		{
			//���map��Stockռ�õĿռ�
			muduo::MutexLockGuard lock(mutex_);
			stocks_.erase(stock->key());
		}
		delete stock;
	}
	
	muduo::MutexLock mutex_;
	std::map<string, boost::weak_ptr<Stock> > stocks_;
};
}

/*
	���ص�
*/
namespace version5
{
class StockFactory : public boost::enable_shared_from_this<StockFactory>, boost::noncopyable
{
public:
	boost::shared_ptr<Stock> get(const string& key)
	{
		boost::shared_ptr<Stock> pStock;
		muduo::MutexLockGuard lock(mutex_);
		boost::weak_ptr<Stock>& wkStock = stocks_[key];
		pStock = wkStock.lock();
		if(!pStock)
		{
			/*
				ǿ�ư�shared_from_this()ת��Ϊweak_ptr���Ų����ӳ�������
			*/
			pStock.reset(new Stock(key), boost::bind(&StockFactory::weakDeleteCallback,
			                         boost::weak_ptr<StockFactory>(shared_from_this()),
			                         _1));
			wkStock = pStock;
		}
		return pStock;
	}
	
	~StockFactory()
	{
		printf("~StockFactory\n");
	}

private:
	static void weakDeleteCallback(const boost::weak_ptr<StockFactory>& wkFactory, Stock* stock)
	//void weakDeleteCallback(const boost::weak_ptr<StockFactory>& wkFactory, Stock* stock)
	{
		printf("weakDeleteCallback[%p]\n", stock);
		boost::shared_ptr<StockFactory> factory(wkFactory.lock());
		if(factory)
		{
			factory->removeStock(stock);
		}
		else
		{
			printf("factory died.\n");
		}
		delete stock;
	}
	
	void removeStock(Stock* stock)
	{
		if(stock)
		{
			//���map��Stockռ�õĿռ�
			muduo::MutexLockGuard lock(mutex_);
			stocks_.erase(stock->key());
		}
	}
	
	muduo::MutexLock mutex_;
	std::map<string, boost::weak_ptr<Stock> > stocks_;
};
}

//using version1::StockFactory;
//using version2::StockFactory;
//using version3::StockFactory;
//using version4::StockFactory;
using version5::StockFactory;

void testLongLiftFactory()
{
	boost::shared_ptr<StockFactory> factory(new StockFactory);
	{
		boost::shared_ptr<Stock> stock = factory->get("NYSE:IBM");
		printf("shared_ptr count: %ld\n", stock.use_count());
		boost::shared_ptr<Stock> stock2 = factory->get("NYSE:IBM");
		printf("shared_ptr count: %ld\n", stock.use_count());
		assert(stock == stock2);
	}
}

void testShortLiftFactory()
{
	boost::shared_ptr<Stock> stock;
	{
		boost::shared_ptr<StockFactory> factory(new StockFactory);
		stock = factory->get("NYSE:IBM");
		printf("shared_ptr count: %ld\n", stock.use_count());
		boost::shared_ptr<Stock> stock2 = factory->get("NYSE:IBM");
		printf("shared_ptr count: %ld\n", stock.use_count());
		assert(stock == stock2);
	}
}


int main()
{
	printf("Start\n");
	
	//testLongLiftFactory();
	testShortLiftFactory();
	
	printf("End\n");
}
