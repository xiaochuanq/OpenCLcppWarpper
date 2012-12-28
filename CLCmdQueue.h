#ifndef CLCMDQUEUE_H
#define CLCMDQUEUE_H
#include "CL/cl.h"

class CLDevice;
class CLContext;

class CLCmdQueue
{
public:
	CLCmdQueue( const CLContext&, const CLDevice&, cl_command_queue_properties = 0);
	~CLCmdQueue(void);
	cl_command_queue handle(){ return m_hCmdQueue; }
private:
	cl_command_queue m_hCmdQueue;
};

#endif