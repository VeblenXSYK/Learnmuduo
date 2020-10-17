#include <iostream>
#include <algorithm>
#include <stdio.h>

// 整数到字符串的转换
const char *convert(char buf[], int value)
{
	static char digits[19] = {
		'9', '8', '7', '6', '5', '4', '3', '2', '1',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9' 
	};
	static const char *zero = digits + 9; // zero 指向 '0'
	
	int i = value;
	char *p = buf;
	do{
		int lsd = i % 10;
		i /= 10;
		*p++ = zero[lsd];
	}while(i != 0);
	
	if(value < 0)
	{
		*p++ = '-';
	}
	
	*p = '\0';
	
	std::reverse(buf, p);
	
	return p;
}

int main()
{
	char buf[1024] = { 0 };
	convert(buf, -1024);
	printf("buf:%s\n", buf);
	
	return 0;
}
