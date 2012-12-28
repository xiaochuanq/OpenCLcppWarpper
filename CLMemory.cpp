#include "CLContext.h"
#include "CLException.h"
#include "CLMemory.h"

#include <iostream>

/*********************** CLMemObject ******************************************/

CLMemObject::CLMemObject( CLContext *pc, cl_mem hm )
	: _contextPtr(pc), _memHandle(hm)
{
}

CLMemObject::CLMemObject(const CLMemObject& mem )
	: _contextPtr(mem._contextPtr), _memHandle( mem._memHandle )
{
}

CLMemObject::~CLMemObject(void)
{
	if( !_memHandle)
		return;
	int err = clReleaseMemObject( _memHandle );
	if( err != CL_SUCCESS )
		std::cerr << "Errors in releasing CL memory Object. Error code " << err << std::endl;
}

size_t CLMemObject::size()
{
	size_t size;
	if (clGetMemObjectInfo( _memHandle, 
		CL_MEM_SIZE,
		sizeof(size), 
		&size,
		0) != CL_SUCCESS)
		return -1;
	else
		return size;
}

/*************** CLBuffer **********************************************/

CLBuffer::CLBuffer( CLContext& context, cl_mem_flags flags, 
	size_t size, void* data)
{
	int err;
	_memHandle = clCreateBuffer(context.handle(),  
		flags,
		size,
		data,
		&err );
	if( err != CL_SUCCESS )
		throw CLBufferException( err );
}

CLEvent CLBuffer::write( CLCmdQueue& cmdq, bool blocking,
						 const void* data_src, size_t size, size_t offset,
						 const std::list<CLEvent>& event_wait_list )
{
	cl_event evnt;
	cl_int err;
	if( event_wait_list.empty() )
	{
		err = clEnqueueWriteBuffer ( cmdq.handle(),
  										_memHandle,
										blocking ? CL_TRUE : CL_FALSE,
										offset,
										size,
										data_src,
										0,
										NULL,
										&evnt);
	}
	else{
		std::vector<cl_event> events( event_wait_list.size() );
		err = clEnqueueWriteBuffer ( cmdq.handle(),
  										_memHandle,
										blocking ? CL_TRUE : CL_FALSE,
										offset,
										size,
										data_src,
										events.size(),
										events.data(),
										&evnt);
		
	}
	if( err != CL_SUCCESS)
		throw CLBufferException( err );
	return CLEvent( evnt);
}

CLEvent CLBuffer::read( CLCmdQueue& cmdq, bool blocking, void* data_dst, 
					 size_t size, size_t offset,
					 const std::list<CLEvent>& event_wait_list )
{
	cl_int err;
	cl_event evnt;
	if( event_wait_list.empty() )
	{
		err = clEnqueueWriteBuffer ( cmdq.handle(),
  										_memHandle,
										blocking ? CL_TRUE : CL_FALSE,
										offset,
										size,
										data_dst,
										0,
										NULL,
										&evnt);
	}
	else{
		std::vector<cl_event> events( event_wait_list.size() );
		for( auto iter = event_wait_list.begin(); iter != event_wait_list.end(); ++iter)
			events.push_back( iter->handle() );
		err = clEnqueueWriteBuffer ( cmdq.handle(),
  										_memHandle,
										blocking? CL_TRUE : CL_FALSE,
										offset,
										size,
										data_dst,
										events.size(),
										events.data(),
										&evnt);
	}
	if( err != CL_SUCCESS)
		throw CLBufferException( err );
	return CLEvent( evnt);
}

/******************** CLImage *****************************************/
CLEvent CLImage::copyFromBuffer(CLCmdQueue& cmdq, 
		const CLBuffer& buffer, 
		const CLCoordinate& origin, 
		const CLRegion& region, 
		const size_t src_offset,
		const std::list<CLEvent>& event_wait_list )
{
	cl_int err;
	cl_event evnt;
	if( event_wait_list.empty() )
	{
		err = clEnqueueCopyBufferToImage( cmdq.handle(),
			buffer.handle(),
			_memHandle,
			src_offset,
			origin.asArray(),
			region.asArray(),
			0,
			NULL,
			&evnt );
	}
	else{
		std::vector<cl_event> events( event_wait_list.size() );
		for( auto iter = event_wait_list.begin(); iter != event_wait_list.end(); ++iter)
			events.push_back( iter->handle() );
		err = clEnqueueCopyBufferToImage( cmdq.handle(),
			buffer.handle(),
			_memHandle,
			src_offset,
			origin.asArray(),
			region.asArray(),
			events.size(),
			events.data(),
			&evnt );
	}
	if( err != CL_SUCCESS)
		throw CLImageException( err );
	return CLEvent( evnt);
}

CLEvent CLImage::copyToBuffer(CLCmdQueue& cmdq,
		CLBuffer& buffer, 
		const CLCoordinate& origin, 
		const CLRegion& region, 
		const size_t dst_offset,
		const std::list<CLEvent>& event_wait_list )
{
	cl_int err;
	cl_event evnt;
	if( event_wait_list.empty() )
	{
		err = clEnqueueCopyImageToBuffer( cmdq.handle(),
			buffer.handle(),
			_memHandle,
			origin.asArray(),
			region.asArray(),
			dst_offset,
			0,
			NULL,
			&evnt );
	}
	else{
		std::vector<cl_event> events( event_wait_list.size() );
		for( auto iter = event_wait_list.begin(); iter != event_wait_list.end(); ++iter)
			events.push_back( iter->handle() );
		err = clEnqueueCopyImageToBuffer( cmdq.handle(),
			buffer.handle(),
			_memHandle,
			origin.asArray(),
			region.asArray(),
			dst_offset,
			events.size(),
			events.data(),
			&evnt );
	}
	if( err != CL_SUCCESS)
		throw CLImageException( err );
	return CLEvent( evnt);
}

/******************** CLImage2D ***************************************/

CLImage2D::CLImage2D( CLContext& context, CLMemFlag flags, 
		const CLImageFormat& image_format, 
		size_t width, size_t height, 
		void* data) : 
_format(image_format), _width( width), _height( height)
{	
	size_t image_row_pitch;
	if( !data)
		image_row_pitch = 0;
	int err;
	_memHandle = clCreateImage2D ( 
							context.handle(),
  							flags,
  							&image_format,
  							width,
  							height,
  							image_row_pitch,
  							data,
  							&err );
	if( err != CL_SUCCESS )
		throw CLImage2DException( err );
}


CLImage2D::~CLImage2D(void)
{
}

/********************* CLImage3D *********************************************/