#!/bin/bash

CXX=g++-4.8 ./configure LDFLAGS="-Wl,--no-as-needed -L/home/chatsiri/workspacecpp/gtest-1.6.0/lib -L/home/chatsiri/sda1/workspacecpp/boost_1_57_0/stage/lib -L/usr/lib64 -L/home/chatsiri/sda1/workspacecpp/clamav-devel-hnmav/build/lib -L/home/chatsiri/workspacecpp/tbb/tbb41_20121003oss/build/linux_intel64_gcc_cc4.6_libc2.15_kernel3.2.0_release -L/home/chatsiri/sda1/workspacecpp/protobuf-2.5.0/build/lib -L/home/chatsiri/sda1/workspacecpp/google-glog/build/lib -L/home/chatsiri/sda1/workspacecpp/rocksdb/ -L/home/chatsiri/sda1/workspacecpp/treadstoneproject/tracethreat_client/build/lib -L/home/chatsiri/sda1/workspacecpp/folly/folly/build/lib -lfolly  -Wl,-rpath,/home/chatsiri/sda1/workspacecpp/boost_1_57_0/stage/lib -L/home/chatsiri/sda1/workspacecpp/mongo-cxx-driver/build/lib" CPPFLAGS="-I/home/chatsiri/workspacecpp/gtest-1.6.0/include -I/home/chatsiri/sda1/workspacecpp/boost_1_57_0/ -I/home/chatsiri/sda1/workspacecpp/clamav-devel-hnmav/ -I/home/chatsiri/workspacecpp/tbb/tbb41_20121003oss/include -I/home/chatsiri/sda1/workspacecpp/protobuf-2.5.0/build/include -I/home/chatsiri/sda1/workspacecpp/google-glog/build/include -I/home/chatsiri/sda1/workspacecpp/rocksdb/include -I/home/chatsiri/sda1/workspacecpp/folly -I/home/chatsiri/sda1/workspacecpp/treadstoneproject/tracethreat_client/src -I/home/chatsiri/sda1/workspacecpp/mongo-cxx-driver/build/include"
