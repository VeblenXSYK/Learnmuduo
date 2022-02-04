#
# CMakeLists.txt - cmake build file for API Design examples
#
# Martin Reddy, 1 Jan 2010 - http://APIBook.com/
#

# Declare the minimum version of cmake that we need
cmake_minimum_required(VERSION 2.4)

# Using the C:\Program Files\ version of cmake won't build shared libraries
# properly under Cygwin. You must install Cygwin's version of cmake for that.

if ("${CMAKE_GENERATOR}" STREQUAL "Unix Makefiles" AND WIN32 AND NOT CYGWIN)

message("WARNING: plugin_api will not be built unless you use Cygwin's /usr/bin/cmake")

else ()

# 构建动态库 libcoreapi.so
add_library(coreapi SHARED coreapi.cpp pluginapi.cpp pluginmanager.cpp)
# 构建静态库 libcoreapi_static.a
add_library(coreapi_static STATIC coreapi.cpp pluginapi.cpp pluginmanager.cpp)
# 设置动态库 libcoreapi.so 支持 -DBUILDING_CORE 编译
set_target_properties(coreapi PROPERTIES DEFINE_SYMBOL BUILDING_CORE)

add_library(plugin1 SHARED plugin1.cpp)
# 设置链接库 libcoreapi.so
target_link_libraries(plugin1 coreapi)

add_library(plugin2 SHARED plugin2.cpp)
target_link_libraries(plugin2 coreapi)

# 构建目标文件
add_executable(plugin_api main.cpp)
target_link_libraries(plugin_api coreapi)

# 查找链接库 libdl.so
if (WIN32)
   find_library(DL_LIB Kernel32.dll)
else ()
   find_library(DL_LIB dl)
endif ()
# 设置链接库 libdl.so
target_link_libraries(plugin_api ${DL_LIB})

endif ()
