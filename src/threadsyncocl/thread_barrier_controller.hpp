#ifndef 
#include "pthread.h"


template<typename BufferSync>
class barrier_controller
{
	
	public:
		virtual void condition() = 0;
		virtual void barrier_point() = 0;

};


template<typename BufferSync>
class barrier_buffer : public barrier_controller<BufferSync>
{

public:
		

private:

};
