local_lat.cc 
```
说明：
	echo server with LengthHeaderCodec
使用：
	./zeromq_local_lat 5555
```

remote_lat.cc
```
说明：
	echo client with LengthHeaderCodec
使用：
	./zeromq_remote_lat 127.0.0.1 5555 4096 100000
```