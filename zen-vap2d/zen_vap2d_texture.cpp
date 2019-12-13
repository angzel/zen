#include "zen_vap2d_texture.h"
#include "zen_file.h"
#include "zen_system.h"

namespace Zen { namespace Vap2d {

	inline uint32_t GetTextureExtendSize(uint32_t s)
	{
		for (int i = 0; i < 16; ++i) {
			if (s < (1U << i)) return (1U << i);
		}
		return 0;
	}
	void Texture::set(uint32_t width, uint32_t height, EBPP format, void const * data)
	{
		this->m_format = format;
		this->m_texture.create();
		this->m_size = { (float)width, (float)height };
#if ZEN_GL_2D_ANY_SZ
		this->m_texture.bindData(width, height, format, data, 0);
		this->m_using_size = { 1.f, 1.f };
#else
		int bpp = (int)format;

		auto w = Zen::GetMinPowerTwo(width);
		auto h = Zen::GetMinPowerTwo(height);

		if(w == width)
		{
			if(h == height)
			{
				this->m_texture.bindData(width, height, format, data, 0);
			}
			else
			{
				auto src_size = width * height * bpp;
				auto dest_size = w * h * bpp;
				std::vector<char> buf(dest_size, 0);
				::memcpy(buf.data(), data, src_size);
				this->m_texture.bindData(w, h, format, buf.data(), 0);
			}
		}
		else
		{
			auto dest_stride = bpp * w;
			auto src_stride = bpp * width;
			auto dest_size = dest_stride * h;

			std::vector<char> buf;
			buf.resize(dest_size, 0);

			auto src = (char const *)data;
			auto dest = buf.data();

			for (int i = 0; i < height; ++i) {
				::memcpy(dest, src, src_stride);
				dest += dest_stride;
				src += src_stride;
			}
			this->m_texture.bindData(w, h, format, buf.data(), 0);
		}
		this->m_using_size = {(float)width /(float)w, (float)height /(float)h};
#endif
	}

	Textures * Textures::GetDefault()
	{
		static auto me = new Textures;
		return me;
	}
	SharedTexture Textures::CreateTexture(uint32_t width, uint32_t height, EBPP bpp, const void *data)
	{
		auto texture = new Texture;
		texture->set(width, height, bpp, data);
		return SharedTexture(texture);
	}
	SharedTexture Textures::CreateTexture(ImageData const & image)
	{
		if(image.format == EBPP::None) return nullptr;

		if(image.format != EBPP::GA)
		{
			return CreateTexture(image.width, image.height, image.format, image.buffer.data());
		}
		// GA is not supported
		ImageData color;
		Zen::ImageSeparateColor(color, image);
		return CreateTexture(color.width, color.height, color.format, color.buffer.data());

	}
	SharedTexture Textures::loadImage(const std::string & path)
	{
		auto ext = GetFilePathExtention(path);
		auto decoder = this->getImageDecoder(ext);
		if(!decoder)
		{
			decoder = this->getImageDecoder("");
			if(!decoder) return nullptr;
		}

		auto data = Zen::LoadResourceContent(path);
		if(data.empty())
		{
			data = Zen::LoadDocumentContent(path);
			if(data.empty()) return nullptr;
		}
		ImageData image;
		try
		{
			decoder->decode(image, data);
		}
		catch(std::exception & e)
		{
			return nullptr;
		}
		return CreateTexture(image);
	}

	void Textures::setImageDecoder(const std::string &extension, ImageDecoder *decoder)
	{
		if(!decoder)
		{
			m_image_decoders.erase(extension);
		}
		else
		{
			m_image_decoders[extension] = decoder;
		}
	}
	ImageDecoder * Textures::getImageDecoder(std::string const & extension)
	{
		auto iter = m_image_decoders.find(extension);
		if(iter == m_image_decoders.end()) return nullptr;
		return iter->second;
	}
	void Textures::clearImageDecoders()
	{
		m_image_decoders.clear();
	}

	void Textures::addTexture(std::string const & name, SharedTexture texture)
	{
		m_texture_caches[name] = texture;
	}

	SharedTexture Textures::getTexture(std::string const & key)
	{
		auto iter = m_texture_caches.find(key);
		if(iter != m_texture_caches.end()) return iter->second;

		auto texture = this->loadImage(key);
		if(texture) this->addTexture(key, texture);
		return texture;
	}
	void Textures::clearTextures(bool all)
	{
		if(all) return m_texture_caches.clear();

		for(auto iter = m_texture_caches.begin(); iter != m_texture_caches.end();)
		{
			if(iter->second.use_count() == 1)
			{
				iter = m_texture_caches.erase(iter);
			}
			else ++iter;
		}
	}

	Textures::Textures()
	{
	}
}}

/**
 default system texture
 */
namespace Zen { namespace Vap2d {

	SharedTexture Textures::getParticleTexture()
	{
		static SharedTexture texture;
		if(texture) return texture;

		int size = 256;
		float r = (float)size/2;
		ImageData image;
		ImageGenerate(image, EBPP::Grey, size, size);
		auto data = image.buffer.data();
		for(int i = 0; i < size; ++i)
		{
			float x2 = Square(r-i);
			for(int j = 0; j < size; ++j)
			{
				float y2 = Square(r-j);
				float v = (float)(sqrtf(x2+y2) / r);
				if(v > 0.99) *data = 0;
				else *data = (uint8_t)(254.f * (1-v));
				++data;
			}
		}
		texture = CreateTexture(image);
		return texture;
	}

	SharedTexture Textures::getCrossTexture()
	{
		static SharedTexture texture;
		if(texture) return texture;

		int size = 256;
		int r = (size/2);
		ImageData image;
		ImageGenerate(image, EBPP::Grey, size, size);
		auto data = image.buffer.data();
		for(int i = 0; i < size; ++i)
		{
			int x = std::abs(r - i);
			for(int j = 0; j < size; ++j)
			{
				int y = std::abs(r - j);
				if(std::abs(x-y) > 8) *data = 0;
				else *data = 255;
				++data;
			}
		}
		texture = CreateTexture(image);
		return texture;
	}
	SharedTexture Textures::getCircleTexture()
	{
		static SharedTexture texture;
		if(texture) return texture;

		int size = 256;
		float r = (float)size/2;
		ImageData image;
		ImageGenerate(image, EBPP::Grey, size, size);
		auto data = image.buffer.data();
		for(int i = 0; i < size; ++i)
		{
			float x2 = Square(r-i);
			for(int j = 0; j < size; ++j)
			{
				float y2 = Square(r-j);
				float v = (float)(sqrtf(x2+y2) / r);
				if(v >= 1.f) *data = 0;
				else if(v > 0.9f) *data = (uint8_t)(254.f * Square(10.f*(1.f-v)));
				else if(v > 0.8f) *data = (uint8_t)(254.f * Square(10.f*(v-.8f)));
				else *data = 0;
				++data;
			}
		}
		texture = CreateTexture(image);
		return texture;
	}

}}
