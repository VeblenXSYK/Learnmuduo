#!/bin/bash

protoc -I=. --cpp_out=. ./commweb.proto
protoc -I=. --python_out=. ./commweb.proto
