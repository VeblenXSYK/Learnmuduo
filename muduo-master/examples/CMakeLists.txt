#包含子目录
#add_subdirectory(ace/ttcp)
add_subdirectory(asio/chat)
add_subdirectory(asio/tutorial)
#add_subdirectory(fastcgi)
add_subdirectory(filetransfer)
add_subdirectory(hub)
add_subdirectory(idleconnection)
add_subdirectory(maxconnection)
#add_subdirectory(memcached/client)
#add_subdirectory(memcached/server)
#add_subdirectory(multiplexer)
add_subdirectory(netty/discard)
#add_subdirectory(netty/echo)
#add_subdirectory(netty/uptime)
add_subdirectory(pingpong)
add_subdirectory(roundtrip)
#add_subdirectory(shorturl)
add_subdirectory(simple)
add_subdirectory(socks4a)
#add_subdirectory(sudoku)
add_subdirectory(twisted/finger)
#add_subdirectory(wordcount)
add_subdirectory(zeromq)

#如果某个target或subdirectory被设置为EXCLUDE_FROM_ALL属性,
#那么这个target(或这个subdirectory中的所有target)就会被排除在all target列表之外，
#这样当执行默认的make(或nmake)时，这个target(或这个subdirectory中的所有target)就不会被编译。
if(CARES_INCLUDE_DIR AND CARES_LIBRARY)
  #add_subdirectory(cdns)
else()
  #add_subdirectory(cdns EXCLUDE_FROM_ALL)	#EXCLUDE_FROM_ALL：
endif()

if(CURL_FOUND)
  #add_subdirectory(curl)
else()
  #add_subdirectory(curl EXCLUDE_FROM_ALL)
endif()

if(PROTOBUF_FOUND)
  #add_subdirectory(ace/logging)
  add_subdirectory(protobuf)
else()
  add_subdirectory(ace/logging EXCLUDE_FROM_ALL)
  add_subdirectory(protobuf EXCLUDE_FROM_ALL)
endif()

if(GD_INCLUDE_DIR AND GD_LIBRARY)
  #add_subdirectory(procmon)
else()
  #add_subdirectory(procmon EXCLUDE_FROM_ALL)
endif()
