
#include "zen_metal_id.h"
#include "zen_metal_type.h"

namespace Zen { namespace Metal {

	Buffer::Buffer()
	{
		m_id = std::shared_ptr<BufferID>(new BufferID);
	}

	void Buffer::create(size_t len, void const * data)
	{
		auto device = Device::Get()->getID()->device;
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

	std::shared_ptr<BufferID> Buffer::getID() const
	{
		return m_id;
	}

	Pipe::Pipe(std::string const & name)
	{
		m_name = name;
		m_id = std::shared_ptr<PipeID>(new PipeID);
	}

	std::string Pipe::getName()
	{
		return m_name;
	}

}}


namespace Zen { namespace Metal {

	void PipeID::create
	(NSString * vertex_func_name,
	 NSString * fragment_func_name)
	{
		auto device_id = Device::Get()->getID();

		id <MTLFunction> vertexFunction = [device_id->library newFunctionWithName:vertex_func_name];

		id <MTLFunction> fragmentFunction = [device_id->library newFunctionWithName:fragment_func_name];

		MTLRenderPipelineDescriptor *desc = [[MTLRenderPipelineDescriptor alloc] init];
		desc.vertexFunction = vertexFunction;
		desc.fragmentFunction = fragmentFunction;
		desc.depthAttachmentPixelFormat = device_id->view.depthStencilPixelFormat;
		desc.stencilAttachmentPixelFormat = device_id->view.depthStencilPixelFormat;
		desc.colorAttachments[0].blendingEnabled = NO;
		desc.colorAttachments[0].pixelFormat = device_id->view.colorPixelFormat;

		state = [device_id->device newRenderPipelineStateWithDescriptor:desc error:nil];
	}


	void PipeID::create
	(NSString * vertex_func_name,
	 NSString * fragment_func_name,
	 MTLBlendFactor src, MTLBlendFactor dst)
	{
		auto device_id = Device::Get()->getID();

		id <MTLFunction> vertexFunction = [device_id->library newFunctionWithName:vertex_func_name];

		id <MTLFunction> fragmentFunction = [device_id->library newFunctionWithName:fragment_func_name];

		MTLRenderPipelineDescriptor *desc = [[MTLRenderPipelineDescriptor alloc] init];
		desc.vertexFunction = vertexFunction;
		desc.fragmentFunction = fragmentFunction;
		desc.depthAttachmentPixelFormat = device_id->view.depthStencilPixelFormat;
		desc.stencilAttachmentPixelFormat = device_id->view.depthStencilPixelFormat;
		auto ca = desc.colorAttachments[0];
		ca.blendingEnabled = YES;
		ca.pixelFormat = device_id->view.colorPixelFormat;
		ca.sourceRGBBlendFactor = src;
		ca.destinationRGBBlendFactor = dst;
		ca.sourceAlphaBlendFactor = src;
		ca.destinationAlphaBlendFactor = dst;

		state = [device_id->device newRenderPipelineStateWithDescriptor:desc error:nil];
	}
	void PipeID::create
	(NSString * vertex_func_name,
	 NSString * fragment_func_name,
	 MTLBlendFactor srcC, MTLBlendFactor dstC,
	 MTLBlendFactor srcA, MTLBlendFactor dstA)
	{
		auto device_id = Device::Get()->getID();

		id <MTLFunction> vertexFunction = [device_id->library newFunctionWithName:vertex_func_name];

		id <MTLFunction> fragmentFunction = [device_id->library newFunctionWithName:fragment_func_name];

		MTLRenderPipelineDescriptor *desc = [[MTLRenderPipelineDescriptor alloc] init];
		desc.vertexFunction = vertexFunction;
		desc.fragmentFunction = fragmentFunction;
		desc.depthAttachmentPixelFormat = device_id->view.depthStencilPixelFormat;
		desc.stencilAttachmentPixelFormat = device_id->view.depthStencilPixelFormat;
		auto ca = desc.colorAttachments[0];
		ca.blendingEnabled = YES;
		ca.pixelFormat = device_id->view.colorPixelFormat;
		ca.sourceRGBBlendFactor = srcC;
		ca.destinationRGBBlendFactor = dstC;
		ca.sourceAlphaBlendFactor = srcA;
		ca.destinationAlphaBlendFactor = dstA;

		state = [device_id->device newRenderPipelineStateWithDescriptor:desc error:nil];
	}
	std::shared_ptr<PipeID> Pipe::getID() const
	{
		return m_id;
	}

	void PipeID::create(std::string const & vertex_func,
						std::string const & fragment_func,
						eBF srcC, eBF dstC, eBF srcA, eBF dstA)
	{
		if(srcC == eBF::None || dstC == eBF::None)
		{
			return this->create(vertex_func, fragment_func);
		}
		else
		{
			this->create
			([NSString stringWithUTF8String:vertex_func.data()],
			 [NSString stringWithUTF8String:fragment_func.data()],
			 (MTLBlendFactor)srcC, (MTLBlendFactor)dstC,
			 (MTLBlendFactor)(srcA==eBF::None ? srcC : srcA),
			 (MTLBlendFactor)(dstA==eBF::None ? dstC : dstA)
			 );
		}
	}
	
	void PipeID::create(std::string const & vertex_func,
						std::string const & fragment_func,
						eBF src, eBF dst)
	{
		if(src == eBF::None || dst == eBF::None)
		{
			return this->create(vertex_func, fragment_func);
		}
		else
		{
			this->create
			([NSString stringWithUTF8String:vertex_func.data()],
			 [NSString stringWithUTF8String:fragment_func.data()],
			 (MTLBlendFactor)src, (MTLBlendFactor)dst
			 );
		}
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
