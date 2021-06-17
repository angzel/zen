
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import "zen_metal.h"

namespace Zen { namespace Metal {
	struct BufferID
	{
		id <MTLBuffer> data;
	};
	
	struct PipeID
	{
		id <MTLRenderPipelineState> state;
		
		void create
		(std::string const & vertex_func,
		 std::string const & fragment_func);
		
		void create
		(std::string const & vertex_func,
		 std::string const & fragment_func,
		 eBlendFactor src_factor,
		 eBlendFactor dst_factor);
		
		void create
		(NSString * vertex_func_name,
		 NSString * fragment_func_name);
		
		void create
		(NSString * vertex_func_name,
		 NSString * fragment_func_name,
		 MTLBlendFactor src_factor,
		 MTLBlendFactor dst_factor);
	};
	
}}
