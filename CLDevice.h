#ifndef CLDEVICE_H
#define CLDEVICE_H
#include <list>
#include <CL/cl.h>

enum CLDeviceType
{
	CPU = CL_DEVICE_TYPE_CPU,
	GPU = CL_DEVICE_TYPE_GPU,
	ACCELERATOR = CL_DEVICE_TYPE_ACCELERATOR,
	DEFAULT = CL_DEVICE_TYPE_DEFAULT
};

class CLDevice
{
public:
	static const std::list<CLDevice>& getAllDevices();
	static CLDevice getDefaultDevice();
public:
	CLDevice( CLDeviceType );
	CLDevice( cl_device_id devid ) : m_hDeviceID( devid ) {}
	CLDevice(const CLDevice& dev) : m_hDeviceID( dev._deviceId ) {}
	CLDeviceType type();
	const cl_device_id& handle() const { return _deviceId; }
private:
	cl_device_id _deviceId;
};

#endif
