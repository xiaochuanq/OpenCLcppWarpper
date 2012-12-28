#include "CLContext.h"
#include "CLEvent.h"
#include "CLException.h"
#include "CLKernel.h"
#include "CLProgram.h"
#include <iostream>
#include <vector>

CLKernel::CLKernel(CLProgram& program, const char* name)
{
	int err;
	_kernel = clCreateKernel( program.handle(), name, &err);
	if( err != CL_SUCCESS)
		throw CLKernelException( err );
}

CLKernel::~CLKernel(void)
{
	int err = clReleaseKernel ( _kernel );
	if( CL_SUCCESS != err )
		std::cerr << "Error in releasing CL kernel. Error Code " << err << std::endl;
}

template<typename... ArgTypes>
int CLKernel::operator()( ArgTypes&... args )
{
	setArg(0, args...); 
	run();
	return 0;
}

CLEvent CLKernel::run()
{
	cl_event clevent;
	cl_int err = clEnqueueNDRangeKernel(
		_contextPtr->cmd_queue().handle(),
		_kernel,
		_workDim,
		NULL,
		&_globalWorkSize,
		&_localWorkSize,
		NULL,
		NULL,
		&clevent );
	if( err == CL_SUCCESS)
		return CLEvent(clevent);
	throw CLKernelException( err );
	return CLEvent( clevent );
}

CLEvent CLKernel::run_after( const std::list<CLEvent>& event_list)
{
	if( event_list.empty() )
		run();
	std::vector< cl_event> ev( event_list.size() );
	for( std::list<CLEvent>::const_iterator iter = event_list.begin(); iter != event_list.end();
		++iter){
			ev.push_back( iter->handle() );
	}
	cl_event clevent;
	cl_int err = clEnqueueNDRangeKernel(
		_contextPtr->cmd_queue().handle(),
		_kernel,
		_workDim,
		NULL,
		&_globalWorkSize,
		&_localWorkSize,
		ev.size(),
		ev.data(),
		&clevent );
	if( err == CL_SUCCESS)
		return CLEvent( clevent );
	throw CLKernelException( err );
	return CLEvent( clevent );
}

CLEvent CLKernel::run_after( const CLEvent& evnt)
{
	cl_event clevent, e2wait = evnt.handle();
	cl_int err = clEnqueueNDRangeKernel(
		_contextPtr->cmd_queue().handle(),
		_kernel,
		_workDim,
		NULL,
		&_globalWorkSize,
		&_localWorkSize,
		1,
		&e2wait,
		&clevent );
	if( err == CL_SUCCESS)
		return CLEvent( clevent );
	throw CLKernelException( err );
	return CLEvent( clevent );
}