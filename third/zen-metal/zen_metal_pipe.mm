
#include "zen_metal_id.h"
#include "zen_metal_type.h"

namespace Zen { namespace Metal {

	Buffer::Buffer()
	{
		m_id = new BufferID;
	}

	void Buffer::create(size_t len, void const * data)
	{
		auto device = Device::S()->getID()->device;
		if(data)
		{
			m_id->data = [device newBufferWithBytes:data length:len options:MTLResourceStorageModeShared];
		}
		else
		{
			m_id->data = [device newBufferWithLength:len options:MTLResourceStorageModeShared];
		}
	}

	void Buffer::update(size_t offset, size_t size, void const * data)
	{
		auto dst = (char*)m_id->data.contents + offset;
		::memcpy(dst, data, size);
	}

	Buffer::~Buffer()
	{
		delete m_id;
	}
	BufferID * Buffer::getID() const
	{
		return m_id;
	}

}}


namespace Zen { namespace Metal {

	void PipeID::create
	(NSString * vertex_func_name,
	 NSString * fragment_func_name)
	{
		auto device_id = Device::S()->getID();

		id <MTLFunction> vertexFunction = [device_id->library newFunctionWithName:vertex_func_name];

		id <MTLFunction> fragmentFunction = [device_id->library newFunctionWithName:fragment_func_name];

		MTLRenderPipelineDescriptor *desc = [[MTLRenderPipelineDescriptor alloc] init];
		desc.vertexFunction = vertexFunction;
		desc.fragmentFunction = fragmentFunction;
		desc.depthAttachmentPixelFormat = device_id->depth_pixel_format;
		desc.stencilAttachmentPixelFormat = device_id->depth_pixel_format;
		desc.colorAttachments[0].blendingEnabled = NO;
		desc.colorAttachments[0].pixelFormat = device_id->color_pixel_format;

		state = [device_id->device newRenderPipelineStateWithDescriptor:desc error:nil];
	}


	void PipeID::create
	(NSString * vertex_func_name,
	 NSString * fragment_func_name,
	 MTLBlendFactor src_factor,
	 MTLBlendFactor dst_factor)
	{
		auto device_id = Device::S()->getID();

		id <MTLFunction> vertexFunction = [device_id->library newFunctionWithName:vertex_func_name];

		id <MTLFunction> fragmentFunction = [device_id->library newFunctionWithName:fragment_func_name];

		MTLRenderPipelineDescriptor *desc = [[MTLRenderPipelineDescriptor alloc] init];
		desc.vertexFunction = vertexFunction;
		desc.fragmentFunction = fragmentFunction;
		desc.depthAttachmentPixelFormat = device_id->depth_pixel_format;
		desc.stencilAttachmentPixelFormat = device_id->depth_pixel_format;
		auto ca = desc.colorAttachments[0];
		ca.blendingEnabled = YES;
		ca.pixelFormat = device_id->color_pixel_format;
		ca.sourceRGBBlendFactor = src_factor;
		ca.destinationRGBBlendFactor = dst_factor;
		ca.sourceAlphaBlendFactor = src_factor;
		ca.destinationAlphaBlendFactor = dst_factor;

		state = [device_id->device newRenderPipelineStateWithDescriptor:desc error:nil];
	}

	Pipe::Pipe(std::string const & name)
	{
		m_name = name;
		m_id = new PipeID;
	}

	Pipe::~Pipe()
	{
		delete m_id;
	}

	std::string Pipe::getName()
	{
		return m_name;
	}

	PipeID * Pipe::getID() const
	{
		return m_id;
	}

	void PipeID::create(std::string const & vertex_func,
						std::string const & fragment_func,
						eBlendFactor src_factor,
						eBlendFactor dst_factor )
	{
		this->create
		([NSString stringWithUTF8String:vertex_func.data()],
		 [NSString stringWithUTF8String:fragment_func.data()],
		 (MTLBlendFactor)src_factor, (MTLBlendFactor)dst_factor
		 );
	}

	void PipeID::create
	(std::string const & vertex_func,
	 std::string const & fragment_func)
	{
		this->create
		([NSString stringWithUTF8String:vertex_func.data()],
		 [NSString stringWithUTF8String:fragment_func.data()]);
	}
}}
