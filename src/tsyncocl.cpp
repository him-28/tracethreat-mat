#include "thread/thread_sync.hpp"

int main()
{

	thread_sync<int> * ts = new thread_sync<int>();
  ts->getdata();

}
