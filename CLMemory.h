#ifndef CLMEMOBJECT_H
#define CLMEMOBJECT_H
#include <list>
#include <vector>
#include <string>
#include "CL/cl.h"
#include "CLVector3.h"


class CLContext;

typedef cl_mem_flags CLMemFlag;

class CLMemObject
{
protected:
	CLMemObject(CLContext *pc = NULL, cl_mem hm = NULL);
	CLMemObject(const CLMemObject& mem );
	~CLMemObject();
public:
	size_t size(); // size in bytes
	cl_mem handle(){ return _memHandle; }
	const cl_mem handle() const { return _memHandle; }
protected:
	CLContext* _contextPtr;
	cl_mem _memHandle;
};

class CLBuffer : public CLMemObject
{
public:
	CLBuffer( CLContext&, CLMemFlag, size_t, void* data = NULL);
public:
	CLEvent write( CLCmdQueue&, bool bBlocking, const void* data_src, size_t size, size_t offset=0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>() );
	CLEvent blocking_write( CLCmdQueue& cmdq, const void* data_src, size_t size, size_t offset=0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>() )
	{
		return write( cmdq, true, data_src, size, offset, wait_list );
	}
	CLEvent nonblocking_write( CLCmdQueue& cmdq, const void* data_src, size_t size, size_t offset=0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>() )
	{
		return write( cmdq, false, data_src, size, offset, wait_list );
	}

	CLEvent write( bool bBlocking, const void* data_src, size_t size, size_t offset=0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>() )
	{
		write( _contextPtr->cmd_queue(), bBlocking, data_src, size, offset, wait_list);
	}
	CLEvent blocking_write(  const void* data_src, size_t size, size_t offset=0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>() )
	{
		return write( true, data_src, size, offset, wait_list );
	}

	CLEvent nonblocking_write(  const void* data_src, size_t size, size_t offset=0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>() )
	{
		return write( false, data_src, size, offset, wait_list );
	}

	CLEvent read( CLCmdQueue&, bool bBlocking, void* data_dst, size_t size, size_t offset=0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>() );
	CLEvent blocking_read( CLCmdQueue& cmdq, void* data_dst, size_t size, size_t offset=0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>() )
	{
		return read( cmdq, true, data_dst, size, offset, wait_list );
	}
	CLEvent nonblocking_read( CLCmdQueue& cmdq, void* data_dst, size_t size, size_t offset=0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>() )
	{
		return read( cmdq, false, data_dst, size, offset, wait_list );
	}

	CLEvent read( bool bBlocking, void* data_dst, size_t size, size_t offset=0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>() )
	{
		return read( _contextPtr->cmd_queue(), bBlocking, data_dst, size, offset, wait_list );
	}
	CLEvent blocking_read( void* data_dst, size_t size, size_t offset=0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>() )
	{
		return read( true, data_dst, size, offset, wait_list );
	}
	CLEvent nonblocking_read( void* data_dst, size_t size, size_t offset=0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>() )
	{
		return read( false, data_dst, size, offset, wait_list );
	}
};

typedef cl_image_format CLImageFormat;

class CLImage : public CLMemObject
{
protected:
	CLImage(CLContext&, cl_mem_flags,
		const CLImageFormat& image_format,  
		size_t width, size_t height, 
		void* data = NULL);
public:
	~CLImage();
	CLImageFormat format(){ return _format; }
	virtual size_t width();
	virtual size_t height();
	virtual size_t size();
	virtual size_t elem_size();
	virtual size_t row_pitch();
	CLEvent copyFromBuffer(CLCmdQueue&, 
		const CLBuffer&, 
		const CLCoordinate& origin, 
		const CLRegion& region, 
		const size_t src_offset = 0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>() );
	CLEvent copyFromBuffer(
		const CLBuffer& buffer, 
		const CLCoordinate& origin, 
		const CLRegion& region, 
		const size_t src_offset = 0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>() )
	{
		copyFromBuffer( this->_contextPtr->cmd_queue(),
			buffer, origin, region, src_offset, wait_list );
	}
	CLEvent copyToBuffer(CLCmdQueue&,
		CLBuffer& buffer, 
		const CLCoordinate& origin, 
		const CLRegion& region, 
		const size_t src_offset = 0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>() );
	CLEvent copyToBuffer(
		CLBuffer& buffer, 
		const CLCoordinate& origin, 
		const CLRegion& region, 
		const size_t dst_offset = 0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>() )
	{
		copyToBuffer( this->_contextPtr->cmd_queue(),
			buffer, origin, region, dst_offset, wait_list );
	}

	CLEvent copyFromImage(CLCmdQueue&,
		CLBuffer& buffer, 
		const CLCoordinate& origin, 
		const CLRegion& region, 
		const size_t src_offset = 0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>() );

private:
	CLImageFormat _format;
	size_t _width;
	size_t _height;
	size_t _size;
	size_t _elem_size;
	size_t _row_pitch;
};

class CLImage2D : public CLImage
{
public:
	CLImage2D( CLContext&, cl_mem_flags,
		const CLImageFormat& image_format,  
		size_t width, size_t height, 
		void* data = NULL);
	~CLImage2D(void);
	CLEvent write( CLCmdQueue&, bool bBlocking, void* data_dst, size_t size, size_t offset=0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>());
	CLEvent read( CLCmdQueue&, bool bBlocking, void* data_dst, size_t size, size_t offset=0,
		const std::list<CLEvent>& wait_list = std::list<CLEvent>());
};

class CLImage3D : public CLImage
{
public:
	CLImage3D( CLContext&, cl_mem_flags,
		const CLImageFormat& image_format,  
		size_t width, size_t height, size_t depth,
		void* data = NULL);
	~CLImage3D(void);
	size_t width(){ return _width;}
	size_t height(){ return _height; }
	size_t depth(){ return _depth; }
	CLImageFormat format(){ return _format; }
private:
};
#endif

