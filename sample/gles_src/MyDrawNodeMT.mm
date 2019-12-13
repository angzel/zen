#include "MyDrawNodeMT.h"

#import <simd/simd.h>
#import <ModelIO/ModelIO.h>

#include "zen_matrix.h"
#include "zen_random.h"

	// Include header shared between C code here, which executes Metal API commands, and .metal files
#import "ShaderTypes.h"

#import <MetalKit/MetalKit.h>
#import "MetalViewController.h"

#include "zen_metal_id.h"
#include "zen_metal_prepare.h"
using namespace Zen::Metal;
static const NSUInteger MaxBuffersInFlight = 3;
class MyDrawNodeMTData
{
public:
	dispatch_semaphore_t _inFlightSemaphore;
//	id <MTLDevice> _device;
//	id <MTLCommandQueue> _commandQueue;

	id <MTLBuffer> _dynamicUniformBuffer[MaxBuffersInFlight];
	id <MTLDepthStencilState> _depthState;
	id <MTLTexture> _colorMap;
	MTLVertexDescriptor *_mtlVertexDescriptor;

	uint8_t _uniformBufferIndex;

	matrix_float4x4 _projectionMatrix;

	float _rotation;
	
	MTKMesh *_mesh;
	PipeID * _pipe;
public:
	void load()
	{
		auto device_id = Device::S()->getID();
		auto _device = Device::S()->getID()->device;

		_inFlightSemaphore = dispatch_semaphore_create(MaxBuffersInFlight);

//		_mtlVertexDescriptor = [[MTLVertexDescriptor alloc] init];
//
//		_mtlVertexDescriptor.attributes[VertexAttributePosition].format = MTLVertexFormatFloat3;
//		_mtlVertexDescriptor.attributes[VertexAttributePosition].offset = 0;
//		_mtlVertexDescriptor.attributes[VertexAttributePosition].bufferIndex = 0;
//
//		_mtlVertexDescriptor.attributes[VertexAttributeTexcoord].format = MTLVertexFormatFloat2;
//		_mtlVertexDescriptor.attributes[VertexAttributeTexcoord].offset = 0;
//		_mtlVertexDescriptor.attributes[VertexAttributeTexcoord].bufferIndex = 1;

		_pipe = Pipes::S()->getShaderSampler(0, 0, eBlendFactor::SourceAlpha, eBlendFactor::OneMinusSourceAlpha)
		->pipe.getID();

		new PipeID;
		_pipe->create(@"VertexShaderSampler", @"FragmentShaderSamplerGrey", nil,
//						   _pipe = new PipeID(@"vertexShader1", @"fragmentShader1", nil,
						   MTLBlendFactorSourceAlpha,
						   MTLBlendFactorOneMinusSourceAlpha);

		setupVertex();


	}


	void loadAsset()
	{

		auto device = Device::S()->getID()->device;
		using namespace Zen;

		int size = 256;

		uint8_t data[size*size*4];

		for(int i = 3; i < size*size*4; i+=4)
		{
			data[i] = (uint8_t)i;
		}

		MTLTextureDescriptor *textureDescriptor = [MTLTextureDescriptor new];
		textureDescriptor.pixelFormat = MTLPixelFormatRGBA8Unorm;
		textureDescriptor.width = size;
		textureDescriptor.height = size;
		textureDescriptor.storageMode = MTLStorageModeShared;

		_colorMap = [device newTextureWithDescriptor:textureDescriptor];
		MTLRegion region =
		{
			{ 0, 0, 0}, //Origin
			{ (NSUInteger)size, (NSUInteger)size, 1 } // Size
		};
		NSUInteger bytesPerRow = size * 4;

		[_colorMap replaceRegion:region mipmapLevel:0 withBytes:data bytesPerRow:bytesPerRow];
	}

	Zen::Random r;
	void draw()
	{
		auto view = Device::S()->getID()->view;
		view.backgroundColor = UIColor.whiteColor;

		dispatch_semaphore_wait(_inFlightSemaphore, DISPATCH_TIME_FOREVER);

		_uniformBufferIndex = (_uniformBufferIndex + 1) % MaxBuffersInFlight;

		auto did = Device::S()->getID();
		id <MTLCommandBuffer> commandBuffer = [did->command_queue commandBuffer];
		commandBuffer.label = @"MyCommand";

		__block dispatch_semaphore_t block_sema = _inFlightSemaphore;
		[commandBuffer addCompletedHandler:^(id<MTLCommandBuffer> buffer)
		 {
			dispatch_semaphore_signal(block_sema);
		}];

			/// Delay getting the currentRenderPassDescriptor until absolutely needed. This avoids
			///   holding onto the drawable and blocking the display pipeline any longer than necessary
		MTLRenderPassDescriptor* rpd = view.currentRenderPassDescriptor;

		rpd.colorAttachments[0].clearColor = MTLClearColorMake(1, 1, 1, 0.2);
//		rpd.colorAttachments[0].clearAc = MTLLoadActionLoad;
		Matrix4 m4[420] = {
			Matrix4MakeScale(-0.5, 0.5, 1),
			Matrix4MakeScale(0.6, 0.8, 1),
		};
		for(auto & m : m4)
		{
			m = Matrix4MakeScale(r.nextf(), r.nextf(), 1);
		}
				/// Final pass rendering code here

		id <MTLRenderCommandEncoder> re =
		[commandBuffer renderCommandEncoderWithDescriptor:rpd];
		re.label = @"MyRenderEncoder";

		[re setFrontFacingWinding:MTLWindingCounterClockwise];
		[re setCullMode:MTLCullModeNone];

		auto device = Device::S()->getID()->device;
//		Buf3 buf;
		for(auto & m : m4)
		{
			[re setRenderPipelineState:_pipe->state];


			auto v2 = m_vertexes[2];
			auto s = (Buf3*)v2.contents;
			s->m = m;
			s->c.set(r.nextf(), r.nextf(), r.nextf(), 1);

			[re setVertexBuffer:m_vertexes[0] offset:0 atIndex:0];

			[re setVertexBuffer:m_vertexes[1] offset:0 atIndex:1];
			[re setVertexBuffer:v2 offset:0 atIndex:2];
			[re setFragmentBuffer:v2 offset:sizeof(Buf3::m) atIndex:0];

			[re setFragmentTexture:_colorMap atIndex:0];

			[re drawPrimitives:MTLPrimitiveTypeTriangleStrip vertexStart:0 vertexCount:4];
		}

		[re endEncoding];
		[commandBuffer presentDrawable:view.currentDrawable];
		[commandBuffer commit];

		[commandBuffer waitUntilCompleted];

	}

	void resize(Zen::Size2 size)
	{
			/// Respond to drawable size or orientation changes here

		float aspect = (float)size.w / (float)size.h;
		_projectionMatrix = matrix_perspective_right_hand(65.0f * (M_PI / 180.0f), aspect, 0.1f, 100.0f);
	}

	static matrix_float4x4 matrix4x4_translation(float tx, float ty, float tz)
	{
		return (matrix_float4x4) {{
			{ 1,   0,  0,  0 },
			{ 0,   1,  0,  0 },
			{ 0,   0,  1,  0 },
			{ tx, ty, tz,  1 }
		}};
	}

	static matrix_float4x4 matrix4x4_rotation(float radians, vector_float3 axis)
	{
		axis = vector_normalize(axis);
		float ct = cosf(radians);
		float st = sinf(radians);
		float ci = 1 - ct;
		float x = axis.x, y = axis.y, z = axis.z;
		Zen::Matrix4 m4 = Zen::Matrix4MakeRotation(radians, x, y, z);
		return (matrix_float4x4) {{
			{ m4.m[0], m4.m[1], m4.m[2], m4.m[3] },
			{ m4.m[4], m4.m[5], m4.m[6], m4.m[7] },
			{ m4.m[8], m4.m[9], m4.m[10], m4.m[11] },
			{ m4.m[12], m4.m[13], m4.m[14], m4.m[15] },
		}};
		return (matrix_float4x4) {{
			{ ct + x * x * ci,     y * x * ci + z * st, z * x * ci - y * st, 0},
			{ x * y * ci - z * st,     ct + y * y * ci, z * y * ci + x * st, 0},
			{ x * z * ci + y * st, y * z * ci - x * st,     ct + z * z * ci, 0},
			{                   0,                   0,                   0, 1}
		}};
	}

	static matrix_float4x4 matrix_perspective_right_hand(float fovyRadians, float aspect, float nearZ, float farZ)
	{
		float ys = 1 / tanf(fovyRadians * 0.5);
		float xs = ys / aspect;
		float zs = farZ / (nearZ - farZ);

		return (matrix_float4x4) {{
			{ xs,   0,          0,  0 },
			{  0,  ys,          0,  0 },
			{  0,   0,         zs, -1 },
			{  0,   0, nearZ * zs,  0 }
		}};
	}
	struct Buf3 {
		Zen::Matrix4 m;
		Zen::Color4f c;
	};
	id <MTLBuffer> m_vertexes[3];
	void setupVertex() {
		static vector_float2 cvs[4] = {
			{ 0, 0, },
			{ 1, 0, },
			{ 0, 1, },
			{ 1, 1, },
		};
		static vector_float2 tvs[4] = {
			{ 0, 0, },
			{ 1, 0, },
			{ 0, 1, },
			{ 1, 1, },
		};

		auto device = Device::S()->getID()->device;
		m_vertexes[0] = [device newBufferWithBytes:cvs length:sizeof(cvs) options:MTLResourceStorageModeShared];
		m_vertexes[1] = [device newBufferWithBytes:tvs length:sizeof(tvs) options:MTLResourceStorageModeShared];
		m_vertexes[2] = [device newBufferWithLength:sizeof(Buf3) options:MTLResourceStorageModeShared];
	}

};

MyDrawNodeMT::MyDrawNodeMT()
{
	m_data = new MyDrawNodeMTData;
	m_data->load();
	m_data->loadAsset();
	m_data->resize({1080, 1920});
}

void MyDrawNodeMT::draw()
{
	m_data->draw();
}
