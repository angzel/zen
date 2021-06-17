
#include "zen_metal_id.h"
#include "zen_metal_type.h"

namespace Zen { namespace Metal {
	Render * Render::Get()
	{
		static auto single = new Render;
		return single;;
	}

	Render::Render()
	{
		m_id = new RenderID;
	}
	Render::~Render()
	{
		delete m_id;
	}

	RenderID * Render::getID() const
	{
		return m_id;
	}
	void Render::setClearColor(Zen::Color4f c)
	{
		m_clear_color = c;
	}

	void Render::start()
	{
		auto & c = m_clear_color;

		m_id->command_buffer = Device::Get()->getID()->command_queue.commandBuffer;

		auto desc = Device::Get()->getID()->view.currentRenderPassDescriptor;
		desc.colorAttachments[0].clearColor =
		MTLClearColorMake(c.red, c.green, c.blue, c.alpha);
		desc.depthAttachment.clearDepth = 0;
		desc.stencilAttachment.clearStencil = 0;

		m_id->encoder = [m_id->command_buffer renderCommandEncoderWithDescriptor:desc];
		m_id->encoder.label = @"ZenRenderEncoder";

		[m_id->encoder setFrontFacingWinding:m_id->front_face];
		[m_id->encoder setCullMode:m_id->cull_mode];
	}

	void Render::end()
	{
		[m_id->encoder endEncoding];
		[m_id->command_buffer presentDrawable:Device::Get()->getID()->view.currentDrawable];
		[m_id->command_buffer commit];
		[m_id->command_buffer waitUntilCompleted];
		
		m_id->command_buffer = nil;
		m_id->encoder = nil;
	}

	void Render::activePipe(PipeID * pipe_id)
	{
		[m_id->encoder setRenderPipelineState:pipe_id->state];
	}

	void Render::setVertexBuffer(int index, BufferID * buffer, size_t off)
	{
		[m_id->encoder setVertexBuffer:buffer->data
								offset:(NSUInteger)off
							   atIndex:(NSUInteger)index];
	}

	void Render::setFragmentBuffer(int index, BufferID * buffer, size_t off)
	{
		[m_id->encoder setFragmentBuffer:buffer->data
								  offset:(NSUInteger)off
								 atIndex:(NSUInteger)index];
	}

	void Render::setFragmentBytes(int index, const void *bytes, size_t len)
	{
		[m_id->encoder setFragmentBytes:bytes length:(NSInteger)len atIndex:(NSInteger)index];
	}
	void Render::bindTexture(int index, TextureID * texture)
	{
		[m_id->encoder setFragmentTexture:texture->color_map
								  atIndex:(NSUInteger)index];
	}

	void Render::drawPrimitives(eVertexMode mode, size_t start, size_t count)
	{
		[m_id->encoder drawPrimitives:(MTLPrimitiveType)mode
						  vertexStart:(NSUInteger)start
						  vertexCount:(NSUInteger)count];
	}
}}
