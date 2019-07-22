#ifndef MOCKMESSAGE_H
#define MOCKMESSAGE_H

#include <string>
#include <gmock/gmock.h>

#include "message.h"

using namespace std;

class MockMessage : public Message
{
public:
	/*
		mock Message类成员函数getMessage，
			string()表示getMessage函数没有参数，返回string
	*/
	MOCK_METHOD0(getMessage, string());
};

#endif
