#ifndef CLEXCEPTION_H
#define CLEXCEPTION_H
#include <exception>
#include "CL/cl.h"

class CLException : public std::exception
{
public:
	CLException( cl_int err ) : m_nError( err) {}
	int error_code(){ return m_nError; }
private:
	cl_int m_nError;
};

class CLDeviceException : public CLException
{
public:
	CLDeviceException( cl_int err) : CLException( err ){}
	const char* what(){ return "CL device exception."; }
};

class CLContextException : public CLException
{
public:
	CLContextException( cl_int err) : CLException( err){}
	const char* what(){ return "CL context exception."; }
};

class CLProgramException : public CLException
{
public:
	CLProgramException(cl_int err) : CLException(err){}
	const char* what(){ return "CL program exception."; }
};

class CLKernelException : public CLException{
public:
	CLKernelException(cl_int err) : CLException(err){}
	const char* what(){ return "CL kernel exception."; }
};

class CLCmdQueueException : public CLException{
public:
	CLCmdQueueException( cl_int err) : CLException(err){}
	const char* what(){ return "CL command queue exception."; }
};

class CLMemObjectException : public CLException{
public:
	CLMemObjectException( cl_int err ) : CLException( err ){}
	const char* what(){ return "CL memory object exception."; }
};

class CLBufferException : public CLMemObjectException{
public:
	CLBufferException(cl_int err) : CLMemObjectException(err){}
	const char* what(){ return "CL memory buffer exceptioin."; }
};


class CLImageException : public CLMemObjectException{
public:
	CLImageException(cl_int err) : CLMemObjectException(err){}
	const char* what(){ return "CL memory image exceptioin."; }
};


#endif