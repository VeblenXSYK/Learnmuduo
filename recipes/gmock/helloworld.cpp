#include "helloworld.h"

HelloWorld::HelloWorld()
{
	
}

HelloWorld::~HelloWorld()
{
	
}

string HelloWorld::getMessage(Message *message) const
{
	return message->getMessage();
}
