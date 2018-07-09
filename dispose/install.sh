#!/bin/bash


curdir=$(cd `dirname $0`;pwd)
rootdir="/.vehicle"

echo -e "\n创建必要的目录..."
if [ ! -d "${rootdir}" ];then
    mkdir ${rootdir}
	cd ${rootdir}
	mkdir -p output bin usrcnf license testdata
	cd ${curdir}
    echo "	目录成功创建"
else
    echo "	目录已经存在"
fi

echo -e "\n添加必要的文件..."
cp usrcnf/* ${rootdir}/usrcnf/
