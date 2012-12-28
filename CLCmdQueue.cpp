#include <iostream>
#include "CLCmdQueue.h"
#include "CLContext.h"
#include "CLException.h"

CLCmdQueue::CLCmdQueue( const CLContext& context, const CLDevice& dev, cl_command_queue_properties properties)
{
	int err;
	m_hCmdQueue = clCreateCommandQueue( context.handle(), dev.handle(), properties, &err);
	if( err != CL_SUCCESS )
		throw CLCmdQueueException( err );
}

CLCmdQueue::~CLCmdQueue(void)
{
	int err = clReleaseCommandQueue( m_hCmdQueue );
	if( err != CL_SUCCESS )
		std::cerr << "Error in releasing CL command queue. Error code " << err << std::endl;
}
