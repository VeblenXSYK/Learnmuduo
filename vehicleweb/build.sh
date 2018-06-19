#!/bin/bash

rm -rf WebServer* dist build

pyinstaller -F ./ServerVH.py -n WebServer -p /usr/local/lib/python2.7/dist-packages/

cp ./dist/WebServer ./
