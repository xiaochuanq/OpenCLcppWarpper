#include "CLCmdQueue.h"
#include "CLContext.h"
#include "CLDevice.h"
#include "CLException.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <fstream>
#include <string>
#include <vector>

CLContext::CLContext(const CLDevice& dev)
{
	cl_int err;
	m_hContext = clCreateContext(0, 1, &dev.handle(), NULL, NULL, &err);
	if( CL_SUCCESS != err )
		throw CLContextException( err );
	m_listDevices.push_back( dev );
	m_pCmdQueue = new CLCmdQueue(*this, dev );
}

CLContext::CLContext(const std::list<CLDevice>& devs)
{
	if( devs.empty() )
		throw CLContextException(0);
	cl_int err;
	std::vector<cl_device_id> buf( devs.size() );
	for( auto iter = devs.begin(); iter != devs.end(); ++iter)
		buf.push_back( iter->handle() );
	m_hContext = clCreateContext(0, buf.size(), buf.data(), NULL,NULL, &err);
	if( err != CL_SUCCESS )
		throw CLContextException( err );
	std::copy( devs.begin(), devs.end(), m_listDevices.end() );
}

CLContext::~CLContext(void)
{
	int err = clReleaseContext( m_hContext );
	if( CL_SUCCESS != err )
		std::cerr << "Error in destructing CL context.  Error Code" << err << std::endl;
	delete m_pCmdQueue;
}


CLProgram CLContext::buildProgramFromSourceFile(const char* filename, const std::string& option)
{
	CLProgram program = createProgramFromSourceFile( filename );
	program.build( option );
	return program;
}

CLProgram CLContext::createProgramFromSourceFile(const char* filename )
{
	std::ifstream ifs(filename, std::fstream::in | std::fstream::binary);
	ifs.seekg(std::ifstream::end);
	size_t filesz = ifs.tellg();
	std::vector<char> buffer(filesz + 1);
	ifs.seekg(std::ifstream::beg);
	ifs >> std::noskipws;
	std::copy(  std::istream_iterator< char >( ifs ),
				std::istream_iterator< char >(),
				std::back_inserter(buffer));
	buffer[filesz] = 0;
	return CLProgram(*this, &( buffer[0]) );
}

CLCmdQueue CLContext::createCmdQueue( CLDevice& dev, cl_command_queue_properties properties = 0)
{
	CLCmdQueue cmdq( *this, dev.handle() , properties );
	m_CmdQueue = cmdq;
	return cmdq;
}