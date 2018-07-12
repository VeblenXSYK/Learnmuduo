#include <iostream>
#include <typeinfo>
#include <string>
#include <google/protobuf/descriptor.h>
#include <examples/protobuf/codec/query.pb.h>

using namespace std;


int main()
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	
	typedef muduo::Query T;
	
	std::string type_name = T::descriptor()->full_name();
	cout << type_name << endl;
	
	//验证T::descriptor()与DescriptorPool::generated_pool()->FindMessageTypeByName("T")相同
	const google::protobuf::Descriptor *descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
	cout << "FindMessageTypeByName() = " << descriptor << endl;
	cout << "T::descriptor() = " << T::descriptor() << endl;
	
	//验证T::default_instance()与MessageFactory::generated_factory()->GetPrototype(descriptor)相同
	const google::protobuf::Message *prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
	cout << "GetPrototype() = " << prototype << endl;
	cout << "T::default_instance() = " << &T::default_instance() << endl;
	
	//创建T对象
	T *new_obj = dynamic_cast<T *>(prototype->New());
	assert(new_obj != NULL);
	assert(new_obj != prototype);
	assert(typeid(*new_obj) == typeid(T::default_instance()));
	cout << "prototype->New() = " << new_obj << endl;
	//prototype->New()f返回的是动态创建的指针，调用方有责任释放它
	delete new_obj;
	
	// Optional:  Delete all global objects allocated by libprotobuf.
	google::protobuf::ShutdownProtobufLibrary();
	
	return 0;
}