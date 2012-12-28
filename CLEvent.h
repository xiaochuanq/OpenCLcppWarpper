#ifndef CLEVENT_H
#define CLEVENT_H
#include "CL/cl.h"

class CLEvent
{
public:
	CLEvent( cl_event e) : m_hEvent( e ){}
	~CLEvent(void);
	cl_event handle(){ return m_hEvent; }
	const cl_event handle() const { return m_hEvent; }
private:
	cl_event m_hEvent;
};

#endif