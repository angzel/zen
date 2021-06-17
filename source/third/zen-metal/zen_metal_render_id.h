
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import "zen_metal.h"

namespace Zen { namespace Metal {
	struct RenderID
	{
		id <MTLRenderCommandEncoder> encoder;
		id <MTLCommandBuffer> command_buffer;
		MTLCullMode cull_mode = MTLCullModeNone;
		MTLWinding front_face = MTLWindingCounterClockwise;
	};
}}
