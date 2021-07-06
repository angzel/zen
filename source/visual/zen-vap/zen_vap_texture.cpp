#include "zen_vap_texture.h"
#include "zen_file.h"
#include "zen_numerical.h"
#include "zen_utils.h"
#include "zen_exception.h"

namespace Zen { namespace Vap {

	TextureShared Texture::Create()
	{
		return TextureShared(new Texture);
	}

	void Texture::set(Image const * image)
	{
		this->set(image->width(), image->height(), image->format(), image->data());
	}
#if ZEN_DRAW_GLES
	void Texture::set(size_t width, size_t height, ePixel format, void const * data)
	{
		musts(width < 0xffffff && height < 0xffffff, "");
		
		if(format == ePixel::GA)
		{
			auto image = Image::CreateWidthByte(ePixel::RGBA, width, height, 0xff);
			image->copy(format, data);
			this->set(image.get());
		}
		else
		{
			m_format = format;
			m_size = { (float)width, (float)height };
			
#	if ZEN_GL_2D_ANY_SZ
			m_texture.gl_buffer.createWithData(width, height, GL::eFilter::Linear, format, data);
			m_using_size = { 1.0f, 1.0f };
#	else
			auto w = Zen::GetMinPowerTwo((uint32_t)width);
			auto h = Zen::GetMinPowerTwo((uint32_t)height);
			
			if(w == width && h == height)
			{
				m_texture.gl_buffer.createWithData(width, height, GL::eFilter::Linear, format, data);
				m_using_size = { 1.0f, 1.0f };
			}
			else
			{
				m_texture.gl_buffer.create(width, height, GL::eFilter::Linear, format);
				m_texture.gl_buffer.fill(0, 0, width, height, data);
				m_using_size = { (float)width /(float)w, (float)height /(float)h };
			}
#	endif
			
		}
	}
#endif

	TextureShared Textures::loadImage(const std::string & path)
	{
		auto ext = GetFilePathExtention(path);
		auto decoder = this->getImageDecoder(ext);
		if(!decoder)
		{
			decoder = this->getImageDecoder("");
			if(!decoder) return nullptr;
		}

		auto data = Zen::Utils::Get()->loadResource(path);
		if(data.empty())
		{
			data = Zen::Utils::Get()->loadDocument(path);
			if(data.empty()) return nullptr;
		}

		try
		{
			auto image = decoder->decode(data);
			auto texture = Texture::Create();
			texture->set(image.get());
			return texture;
		}
		catch(std::exception & e)
		{
			return nullptr;
		}
	}

	Textures * Textures::Get()
	{
		static auto single = new Textures;
		return single;;
	}

	void Textures::setImageDecoder(const std::string &extension, std::shared_ptr<ImageDecoder> decoder)
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
	std::shared_ptr<ImageDecoder> Textures::getImageDecoder(std::string const & extension)
	{
		auto iter = m_image_decoders.find(extension);
		if(iter == m_image_decoders.end()) return nullptr;
		return iter->second;
	}
	void Textures::clearImageDecoders()
	{
		m_image_decoders.clear();
	}

	void Textures::addTexture(std::string const & name, TextureShared texture)
	{
		m_texture_caches[name] = texture;
	}

	TextureShared Textures::getTexture(std::string const & key)
	{
		auto iter = m_texture_caches.find(key);
		if(iter != m_texture_caches.end()) return iter->second;

		auto texture = this->loadImage(key);
		if(texture) this->addTexture(key, texture);
		musts(texture, "cannot find image");
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
namespace Zen { namespace Vap {

	TextureShared Textures::getParticleTexture()
	{
		static TextureShared texture;
		if(texture) return texture;

		int size = 256;
		float r = (float)size/2;

		auto image = Image::Create(ePixel::Grey, size, size);
		auto data = image->data();
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
		texture = Texture::Create();
		texture->set(image.get());
		return texture;
	}

	TextureShared Textures::getCrossTexture()
	{
		static TextureShared texture;
		if(texture) return texture;

		int size = 256;
		int r = (size/2);

		auto image = Image::Create(ePixel::Grey, size, size);
		auto data = image->data();
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
		texture = Texture::Create();
		texture->set(image.get());
		return texture;
	}
	TextureShared Textures::getCircleTexture()
	{
		static TextureShared texture;
		if(texture) return texture;

		int size = 256;
		float r = (float)size/2;

		auto image = Image::Create(ePixel::Grey, size, size);
		auto data = image->data();

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
		texture = Texture::Create();
		texture->set(image.get());
		return texture;
	}

}}
