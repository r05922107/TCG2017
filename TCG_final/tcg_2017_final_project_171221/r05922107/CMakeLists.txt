cmake_minimum_required(VERSION 3.9)
project(r05922107)

set(CMAKE_CXX_STANDARD 11)

add_executable(r05922107
        anqi.cc
        anqi.hh
        ClientSocket.cpp
        ClientSocket.h
        main.cc
        Protocol.cpp
        Protocol.h
        Search.h HashTable.h HashTable.cpp)
