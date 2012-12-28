#include "CLDevice.h"
#include "CLException.h"
#include <vector>

CLDevice::CLDevice( CLDeviceType type ) : _deviceId( NULL)
{
	cl_int err = clGetDeviceIDs(NULL, type, 1, &_deviceId, NULL);
	if( err != CL_SUCCESS)
		throw CLDeviceException( err );
}

CLDeviceType CLDevice::type()
{
	cl_device_type type;
	cl_int err = clGetDeviceInfo( _deviceId, CL_DEVICE_TYPE,
								  sizeof(cl_device_type), &type, 0);
	if( err == CL_SUCCESS)
		return static_cast<CLDeviceType>(type);
	throw CLDeviceException( err );
}

const std::list<CLDevice>& CLDevice::getAllDevices()
{
	static std::list<CLDevice>* ptr_all_device_list = NULL;
	if( !ptr_all_device_list ){
		cl_uint size;
		cl_int err = clGetDeviceIDs(0, CL_DEVICE_TYPE_ALL,0, 0, &size);
		if( err != CL_SUCCESS)
			throw CLDeviceException( err );
		ptr_all_device_list = new std::list<CLDevice>();
		std::vector<cl_device_id> buffer( size, NULL );
		clGetDeviceIDs( 0, CL_DEVICE_TYPE_ALL, size, buffer.data(), &size);
		for( auto iter = buffer.begin(); iter != buffer.end(); ++iter )
			ptr_all_device_list->push_back( CLDevice( *iter ));
	}
	return *ptr_all_device_list;
}

CLDevice CLDevice::getDefaultDevice()
{
	
}