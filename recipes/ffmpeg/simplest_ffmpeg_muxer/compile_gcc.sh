#! /bin/sh
#��򵥵Ļ���FFmpeg������Ƶ������----�����б���
#Simplest FFmpeg Muxer----Compile in Shell 
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#compile
gcc simplest_ffmpeg_muxer.cpp -g -o simplest_ffmpeg_muxer -I /usr/local/include -L /usr/local/lib \
-lavformat -lavcodec -lavutil -lpthread -lm -ldl -lz -lswresample
