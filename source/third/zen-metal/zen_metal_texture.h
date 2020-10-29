// objective-c support needed

#ifdef ZEN_METAL_CPP
#include <string>
#include "zen_color.h"

namespace Zen { namespace Metal {
	struct TextureID;

	class Texture
	{
	public:
		Texture();
		virtual ~Texture();
		
		TextureID * getID() const;

		void create
		(size_t width, size_t height,
		 bool is_a8, void const * data_null, int level);
	public:
		TextureID * m_id = nullptr;
		ePixel m_format = ePixel::None;
	};

}}
#endif

#ifdef ZEN_METAL_OC

#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>


namespace Zen { namespace Metal {
	struct TextureID
	{
		id <MTLTexture> color_map;
	};
}}

#endif
