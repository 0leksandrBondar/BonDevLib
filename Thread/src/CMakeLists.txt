cmake_minimum_required(VERSION 3.12)


add_library(bondevThread
	Thread.cpp
	Thread.h
	Task.h
	
)

target_include_directories(bondevThread PUBLIC ./)