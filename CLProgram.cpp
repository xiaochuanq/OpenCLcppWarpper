#include "CLContext.h"
#include "CLException.h"
#include "CLProgram.h"
#include <memory>
#include <vector>

CLProgram::CLProgram( CLContext& context, const char* src )
{
	//src must be null terminated
	cl_int err;
	_programHandle = clCreateProgramWithSource( context.handle(), 1, &src, NULL, &err);
	if( err != CL_SUCCESS)
		throw CLProgramException( err );
}


CLProgram::~CLProgram(void)
{
	clReleaseProgram( _programHandle );
}

cl_int CLProgram::build(const std::list<CLDevice>& dev_list, const std::string& option)
{
	cl_int err;
	if (dev_list.empty()) {
        err = clBuildProgram( _programHandle, 
			                  0, 
							  NULL, 
							  option.empty() ? NULL : option.c_str(), 
							  NULL, 
							  NULL );
    } else {
		std::vector<cl_device_id> devs( dev_list.size() );
		for( auto iter = dev_list.begin(); iter != dev_list.end(); ++iter)
		devs.push_back( iter->handle() );
        err = clBuildProgram( _programHandle, 
							  devs.size(), 
							  &devs[0], 
							  option.empty() ? NULL : option.c_str(), 
							  NULL, 
							  NULL);
    }
	return err;
}

cl_int CLProgram::build(const CLDevice& dev, const std::string& option )
{
	return clBuildProgram( _programHandle,
						   1, 
						   &dev.handle(), 
						   option.empty() ? NULL : option.c_str(), 
						   NULL, 
						   NULL );
}

cl_int CLProgram::build( const std::string& option )
{
	return clBuildProgram( _programHandle, 
			               0, 
						  NULL, 
						  option.empty() ? NULL : option.c_str(), 
						  NULL, 
						  NULL );
}

CLKernel CLProgram::createKernel( const char* name)
{
	return CLKernel( *this, name);
}

void CLProgram::createKernels( std::vector<CLKernel>& kv, size_t max_kernels)
{
	std::unique_ptr<cl_kernel> buffer( new cl_kernel[ max_kernels ]);
	cl_uint nk = 0;
	cl_int err = clCreateKernelsInProgram( this->handle(), 
			max_kernels,
			buffer.get(),
			&nk);
	if( err != CL_SUCCESS)
		throw CLKernelException( err );
	for( cl_uint i = 0; i < nk; ++i) 
			kv.push_back( CLKernel( buffer.get()[i]) );
}