#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import "zen_metal.h"

namespace Zen { namespace Metal {
	struct DeviceID
	{
		id<MTLDevice> device;

		id<MTLLibrary> library;

		id<MTLCommandQueue> command_queue;
		
		MTKView * view = nil;
		
		void setMTKView(MTKView * view);
	};
	
}}
