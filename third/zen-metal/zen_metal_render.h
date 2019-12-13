// objective-c support needed


#ifdef ZEN_METAL_CPP
#include <string>
#include "zen_color.h"

namespace Zen { namespace Metal {
	struct RenderID;

	class Render
	{
	protected:
		Render();
		virtual ~Render();
	public:
		static Render * S();

		void setClearColor(Zen::Color4f);

		void start();

		void end();

		void activePipe(PipeID * pipe_id);

		void setVertexBuffer(int index, BufferID * buffer, size_t off);

		void setFragmentBuffer(int index, BufferID * buffer, size_t off);

		void bindTexture(int index, TextureID * texture);

		void drawPrimitives(eMode mode, size_t start, size_t count);

//		void drawIndexedPrimitives(eMode mode, size_t indexCount, BufferID * buffer, size_t off_bytes, size_t instanceCount);

		RenderID * getID() const;
	public:
		RenderID * m_id = nullptr;
		Zen::Color4f m_clear_color = Zen::eColor::Black;
	};
}}
#endif

#ifdef ZEN_METAL_OC

#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>


namespace Zen { namespace Metal {
	struct RenderID
	{
		id <MTLRenderCommandEncoder> encoder;
		id <MTLCommandBuffer> command_buffer;
		MTLCullMode cull_mode = MTLCullModeNone;
		MTLWinding front_face = MTLWindingCounterClockwise;
	};
}}

#endif
