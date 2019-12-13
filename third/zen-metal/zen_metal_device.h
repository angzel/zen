// objective-c support needed

#ifdef ZEN_METAL_CPP
#include <string>

namespace Zen { namespace Metal {
	struct DeviceID;

	class Device
	{
	protected:
		Device();
		virtual ~Device();
	public:
		static Device * S();

		DeviceID * getID() const;
	protected:
		DeviceID * m_id = nullptr;
	};
}}

#endif

#if defined(ZEN_METAL_OC)

#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>

namespace Zen { namespace Metal {
	struct DeviceID
	{
		id<MTLDevice> device;

		id<MTLLibrary> library;

		id<MTLCommandQueue> command_queue;
		
		MTLPixelFormat depth_pixel_format = MTLPixelFormatDepth32Float_Stencil8;
		
		MTLPixelFormat color_pixel_format = MTLPixelFormatBGRA8Unorm_sRGB;

		MTKView * view = nil;
		
		DeviceID();
	};
}}

#endif
