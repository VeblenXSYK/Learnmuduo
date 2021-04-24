#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>

template<typename T>
class AtomicIntegerT
{
public:
	AtomicIntegerT()
	: value_(0)
	{
	}

	T get() const
	{
		return __sync_val_compare_and_swap(const_cast<volatile T*>(&value_), 0, 0);
	}

	T getAndAdd(T x)
	{
		return __sync_fetch_and_add(&value_, x);
	}

	T addAndGet(T x)
	{
		return getAndAdd(x) + x;
	}

	T incrementAndGet()
	{
		return addAndGet(1);
	}

	void add(T x)
	{
		getAndAdd(x);
	}

	void increment()
	{
		incrementAndGet();
	}

	void decrement()
	{
		getAndAdd(-1);
	}

	T getAndSet(T newValue)
	{
		return __sync_lock_test_and_set(&value_, newValue);
	}
private:
	// 禁止拷贝构造函数
	AtomicIntegerT( const AtomicIntegerT& ); 
	AtomicIntegerT& operator=( const AtomicIntegerT& ); 

private:
	volatile T value_;
};
AtomicIntegerT<int64_t>	g_AtomicInt64;
int64_t	g_Int64;

void *thr_worker(void *arg) 
{
	for (int i = 0; i < 1000; ++i) 
	{
		g_AtomicInt64.increment();
		g_Int64++;
	}
   
	return NULL;
}

int main()
{
	for (int i = 0; i < 100; ++i) 
	{
		pthread_t tID;
		pthread_create(&tID, NULL, thr_worker, NULL);
	}
	
	while(1)
	{
		sleep(1);
		printf("g_AtomicInt64:%ld, g_Int64:%ld\n", g_AtomicInt64.get(), g_Int64);
	}
	
	return 0;
}
