#ifndef CLCONTEXT_H
#define CLCONTEXT_H
#include <list>
#include <string>
#include "CLCmdQueue.h"
#include "CLKernel.h"
#include "CLProgram.h"
class CLDevice;

class CLContext
{
public:
	CLContext(const CLDevice&);
	CLContext(const std::list<CLDevice>&);
	~CLContext(void);
	const cl_context& handle() const { return m_hContext;}
	cl_context& handle() { return m_hContext; }
	void finsh();
public: // Device
	const CLDevice& defaultDevice(){ return m_listDevices.front(); }
	const std::list<CLDevice>& allDevices(){ return m_listDevices; }
public: // Command Queue
	CLCmdQueue createCmdQueue( CLDevice& dev, cl_command_queue_properties = 0);
	CLCmdQueue& cmd_queue(){ return *m_pCmdQueue; }
public: // Memory

public: // Program
	CLProgram createProgramFromSourceFile(const char* filename);
	CLProgram buildProgramFromSourceFile(const char*, const std::string& build_option);
private:
	cl_context m_hContext;
	std::list<CLDevice> m_listDevices;
	CLCmdQueue* m_pCmdQueue;
};

#endif