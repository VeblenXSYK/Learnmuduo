#ifndef HELLOWORLD_H
#define HELLOWORLD_H

#include <string>

#include "message.h"

using namespace std;

class HelloWorld
{
public:
	HelloWorld();
	virtual ~HelloWorld();
	string getMessage(Message* message) const;
};

#endif
