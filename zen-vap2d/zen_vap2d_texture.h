#pragma once

#include "zen_gles2.h"
#include "zen_image.h"

namespace Zen { namespace Vap2d {
	class Texture {
	protected:
		Zen::GL::TexImage2D m_texture;
		Zen::Size2 m_size;
		Zen::Size2 m_real_size;
		Zen::Size2 m_gl_size;
		EBPP m_format;
	public:
		Texture() {printf("texture new\n");}
		Texture(Texture&) = delete;
	public:
		Zen::GL::TexImage2D const & image2d() { return m_texture; }
		Zen::Size2 const & size() { return  m_size; }
		Zen::Size2 const & real_size() { return m_real_size; }
		Zen::Size2 const & gl_size() { return m_gl_size; };
		EBPP format() { return m_format; }
	public:
		void set(uint32_t width, uint32_t height, EBPP bpp, void const * data);
	};

	typedef std::shared_ptr<Texture> SharedTexture;

	class Textures
	{
	public:
		static Textures * GetDefault();

		/**
		 @createTexture
		 -create SharedTexture.
		 -not cached.
		 */
		static SharedTexture CreateTexture(uint32_t width, uint32_t height, EBPP bpp, void const * data);

		static SharedTexture CreateTexture(ImageData const & image);

	public:
		void setImageDecoder(std::string const & extension, ImageDecoder * decoder);

		ImageDecoder * getImageDecoder(std::string const & extension);

		void clearImageDecoders();

		/**
		 the texture is not cached.
		 */
		SharedTexture loadImage(const std::string & path);

	public: // cache texture
		/**
		 @addTexture
		 - add texture to cache with key.
		 */
		void addTexture(std::string const & key, SharedTexture texture);

		/**
		 @getTexture
		 - get texture with key.
		 - case1: if found in texture cache, return it. or
		 - case 2: if key found  in file cache, load that file as image, then push to texture cache and return it. or
		 - case 3: treat key as a file path, load that file as image,  then push to cache and return it. or
		 - if load image failed in case2 or case3, return nullptr.
		 */
		SharedTexture getTexture(std::string const & key);

		/**
		 default all = false, remove unused.
		 all = true, remove all
		 */
		void clearTextures(bool all = false);

	public:

		SharedTexture getParticleTexture();

		SharedTexture getCrossTexture();

		SharedTexture getCircleTexture();

	protected:
		Textures();

		Textures(Textures&) = delete;

		std::map<std::string, SharedTexture> m_texture_caches;
		std::map<std::string, ImageDecoder*> m_image_decoders;
		std::map<std::string, SharedTexture> m_default_textures;

	};
}}
