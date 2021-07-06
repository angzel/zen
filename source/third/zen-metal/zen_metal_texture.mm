
#import "zen_metal_texture_id.h"
#import "zen_metal_device_id.h"

namespace Zen { namespace Metal {
	Texture::Texture()
	{
		m_id.reset(new TextureID);
	}
	Texture::~Texture()
	{
	}
	
	void Texture::create(size_t width, size_t height, bool is_a8, void const * data, int level)
	{
		MTLPixelFormat format = is_a8?MTLPixelFormatA8Unorm : MTLPixelFormatRGBA8Unorm;
		MTLTextureDescriptor *desc = [MTLTextureDescriptor new];
		desc.textureType = MTLTextureType2D;
		desc.pixelFormat = format;
		desc.width = (NSUInteger)width;
		desc.height = (NSUInteger)height;
//		desc.storageMode = MTLStorageModeShared;
		desc.cpuCacheMode = MTLCPUCacheModeWriteCombined;

		m_id->mtl_texture = [Device::Get()->getID()->device newTextureWithDescriptor:desc];
//		id<MTLDevice> d;
//		[d newTextureWithDescriptor:nil];
		
		if(data)
		{
			MTLRegion region = MTLRegionMake2D
			(0, 0, (NSUInteger)width, (NSUInteger)height);

			[m_id->mtl_texture replaceRegion:region
							   mipmapLevel:level
								 withBytes:data
							   bytesPerRow:(is_a8? width: width*4)];
		}
	}
	
	std::shared_ptr<TextureID> Texture::getID() const
	{
		return m_id;
	}
}}
