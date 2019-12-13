
#include "zen_metal_id.h"
#include "zen_metal_type.h"

namespace Zen { namespace Metal {
	Texture::Texture()
	{
		m_id = new TextureID;
	}
	Texture::~Texture()
	{
		delete m_id;
	}
	TextureID * Texture::getID() const
	{
		return m_id;
	}
	void Texture::create(size_t width, size_t height, bool is_a8, void const * data, int level)
	{
		MTLPixelFormat format = is_a8?MTLPixelFormatA8Unorm : MTLPixelFormatRGBA8Unorm;
		MTLTextureDescriptor *desc = [MTLTextureDescriptor new];
		desc.pixelFormat = format;
		desc.width = (NSUInteger)width;
		desc.height = (NSUInteger)height;
		desc.storageMode = MTLStorageModeShared;

		m_id->color_map = [Device::S()->getID()->device newTextureWithDescriptor:desc];

		if(data)
		{
			MTLRegion region = MTLRegionMake2D
			(0, 0, (NSUInteger)width, (NSUInteger)height);

			[m_id->color_map replaceRegion:region
							   mipmapLevel:level
								 withBytes:data
							   bytesPerRow:(is_a8? width: width*4)];
		}
	}
}}
