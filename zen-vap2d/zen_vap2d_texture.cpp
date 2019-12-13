#include "zen_vap2d_texture.h"
#include "zen_file.h"
#include "zen_numerical.h"
#include "zen_system.h"

namespace Zen { namespace Vap2d {

	SharedTexture Texture::Create()
	{
		return SharedTexture(new Texture);
	}

	void Texture::set(Image const * image)
	{
		this->set(image->width(), image->height(), image->format(), image->bytes());
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

	Textures * Textures::S()
	{
		static auto single = new Textures;
		return single;;
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

		auto image = Image::Create(ePixel::Grey, size, size);
		auto data = image->bytes();
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

	SharedTexture Textures::getCrossTexture()
	{
		static SharedTexture texture;
		if(texture) return texture;

		int size = 256;
		int r = (size/2);

		auto image = Image::Create(ePixel::Grey, size, size);
		auto data = image->bytes();
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
	SharedTexture Textures::getCircleTexture()
	{
		static SharedTexture texture;
		if(texture) return texture;

		int size = 256;
		float r = (float)size/2;

		auto image = Image::Create(ePixel::Grey, size, size);
		auto data = image->bytes();

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
