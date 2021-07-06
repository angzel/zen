#pragma once

#include "zen_vap_render.h"
#include "zen_image.h"
#include <map>

namespace Zen { namespace Vap {
	
	class Texture {
	public:
		Texture() {printf("texture new\n");}
		virtual ~Texture() = default;
		Texture(Texture&) = delete;
	public:
		TextureBuffer & buffer() { return m_texture; }

		Zen::Size2 const & size() { return  m_size; }
		Zen::Size2 const & using_size() { return m_using_size; };
		ePixel format() { return m_format; }

	public:
		static std::shared_ptr<Texture> Create();

		void set(size_t width, size_t height, ePixel bpp, void const * data);

		void set(Image const * image);

	protected:
		TextureBuffer m_texture;
		Zen::Size2 m_size = { 0, 0 };
		Zen::Size2 m_using_size = { 1, 1 };
		ePixel m_format = ePixel::None;
	};

	typedef std::shared_ptr<Texture> TextureShared;

	class Textures
	{
	public:
		static Textures * Get();

	public:
		void setImageDecoder(std::string const & extension, std::shared_ptr<ImageDecoder> decoder);

		std::shared_ptr<ImageDecoder> getImageDecoder(std::string const & extension);

		void clearImageDecoders();

		/**
		 warning: just load . the texture not cached.
		 */
		TextureShared loadImage(const std::string & path);

	public: // cache texture
		/**
		 @addTexture
		 - add texture to cache with key.
		 */
		void addTexture(std::string const & key, TextureShared texture);

		/**
		 @getTexture
		 - get texture with key.
		 - case1: if found in texture cache, return it. or
		 - case 2: if key found  in file cache, load that file as image, then push to texture cache and return it. or
		 - case 3: treat key as a file path, load that file as image,  then push to cache and return it. or
		 - if load image failed in case2 or case3, return nullptr.
		 */
		TextureShared getTexture(std::string const & key);

		/**
		 default all = false, remove unused.
		 all = true, remove all
		 */
		void clearTextures(bool all = false);

	public:

		TextureShared getParticleTexture();

		TextureShared getCrossTexture();

		TextureShared getCircleTexture();

	protected:
		Textures();

		Textures(Textures&) = delete;

		std::map<std::string, TextureShared> m_texture_caches;
		std::map<std::string, std::shared_ptr<ImageDecoder> > m_image_decoders;
	};
}}
