#ifndef CLPROGRAM_H
#define CLPROGRAM_H
#include <string>
#include <list>
#include "CL/cl.h"
#include "CLDevice.h"
#include "CLKernel.h"

class CLContext;

class CLProgram
{
public:
	CLProgram( CLContext&, const char *);
	~CLProgram( void);
	cl_program handle(){ return _programHandle; }
	const cl_program handle() const { return _programHandle; }

	cl_int build( const std::list<CLDevice>&, const std::string& option = "" );
	cl_int build( const CLDevice&, const std::string& option = "" );
	cl_int build( const std::string& option = "" );

	CLKernel createKernel(const char *name);
	void createKernels( std::vector<CLKernel>&, size_t max_kernels = 64 );

	void setGlobalWorkItemSize();
	void setLocalWorkItemSize();
private:
	cl_program _programHandle;
	CLContext* _contextPtr;
};

#endif