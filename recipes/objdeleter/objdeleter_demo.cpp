#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdexcept>

template <class T>
class OBJDeleter
{
public:
	OBJDeleter(T *i_p) : p(i_p) 
	{
		printf("OBJDeleter\n");
	}
	
	~OBJDeleter()
	{
		printf("~OBJDeleter\n");
		
		if (NULL != p)
		{
			delete p;
			p = NULL;
		}
	}
private:
	T* p; 
};

template <class T>
class OBJArrayDeleter
{
public:
	OBJArrayDeleter(T *i_p) : p(i_p) 
	{
		printf("OBJArrayDeleter\n");
	}
	
	~OBJArrayDeleter()
	{
		printf("~OBJArrayDeleter\n");
		
		if (NULL != p)
		{
			delete []p;
			p = NULL;
		}
	}
private:
	T* p; 
};

class Test {
public:
	Test()
	{
		memset(data, 0, sizeof(data));
	}
private:
	char data[100*1024*1024];
};

typedef OBJDeleter<Test> OBJPtrDeleter;
typedef OBJArrayDeleter<Test> OBJArrayPtrDeleter;

int main()
{
	while(1)
	{
		{
			Test *p1 = new(std::nothrow) Test;
			OBJPtrDeleter objDel(p1);
			if(NULL == p1)
			{
				printf("new Test error\n");
			}
		}
		
		{
			Test *p1 = new(std::nothrow) Test[10];
			OBJArrayPtrDeleter objArrayDel(p1);
			if(NULL == p1)
			{
				printf("new Test[100] error\n");
			}
		}
		
		printf("\n");
		
		sleep(5);
	}
	
	return 0;
}
