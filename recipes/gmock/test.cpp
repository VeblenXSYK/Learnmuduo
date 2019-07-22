#include <string>
#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "helloworld.h"
#include "mockmessage.h"


TEST(HelloWorldTest, getMessage)
{
	MockMessage message;
	std::string msg = "Hello World";
	
	/*
		设置一个mock函数调用的预期结果
			第一个参数是mock对象，第二个参数是mock函数名及其参数
			WillRepeatedly(action)表示每次调用的行为
	*/
	EXPECT_CALL(message, getMessage()).WillRepeatedly(testing::Return(testing::ByRef(msg)));
	
	HelloWorld helloworld;
	EXPECT_EQ("Hello World", helloworld.getMessage(&message));
	EXPECT_EQ("Hello World", helloworld.getMessage(&message));	// 如果将之前的WillRepeatedly改为WillOnce则会出错，WillOnce(action)表示一次调用的行为
	EXPECT_EQ("Hello World", helloworld.getMessage(&message));
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
