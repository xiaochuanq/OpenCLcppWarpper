#ifndef CLKERNEL_H
#define CLKERNEL_H
#include <list>
#include <vector>
#include "CL/cl.h"
#include "CLEvent.h"
class CLContext;
class CLProgram;

class CLKernel
{
public:
	CLKernel( CLProgram& program, const char* name );
	CLKernel( const cl_kernel& kernel ) : _kernel(kernel){ }
	~CLKernel(void);
	const cl_kernel& handle()const { return _kernel; }

	size_t globalWorkSize(){ return _globalWorkSize; }
	size_t localWorkSize(){ return _localWorkSize; }
	cl_uint workDim() { return _workDim; }

	void setGlobalWorkSize( size_t );
	void setLocalWorkSize( size_t);
	void setWorkDim( cl_uint);

	CLEvent run_after( const std::list<CLEvent>& );
	CLEvent run_after( const CLEvent& );
	CLEvent run();

	template< typename... ArgTypes> 
	int operator()( ArgTypes&... args);

	template<typename ArgType>
	inline void setArg(cl_uint idx, ArgType& a){
		clSetKernelArg( m_hKernel, idx, sizeof(ArgType), &a);
	}
  	template<typename FirstArgType, typename ...ArgTypes> 
	inline void setArg(cl_uint idx, FirstArgType& a, ArgTypes&... rest_args ){
		clSetKernelArg( m_hKernel, idx++, sizeof(FirstArgType), &a);
		setArg( idx, rest_args... );
	}
private:
	cl_kernel _kernel;
	CLContext* _contextPtr;
	cl_uint _workDim;
	size_t _globalWorkSize;
	size_t _localWorkSize;
};

#endif