#ifndef MESSAGE_H
#define MESSAGE_H
 
#include <string>
using namespace std;
 
class Message
{
public:
    virtual ~Message() {}
    virtual string getMessage() = 0;
};
 
#endif /* MESSAGE_H */
