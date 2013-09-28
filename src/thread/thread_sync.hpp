#ifndef THREAD_SYNC_HPP
#define THREAD_SYNC_HPP

#include <iostream>
//#include "gtest/gtest.h"



template<typename StackThread>
class thread_sync
{
public:

thread_sync(){ }
void getdata();

private:
StackThread st;
};

template<typename FileBuffer>
class thread_runner
{
public:

		void list_thread(std::string str){ }

private:


}; 


#endif /* THREAD_THREAD_SYNC_HPP */
