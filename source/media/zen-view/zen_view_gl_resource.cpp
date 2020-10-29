
#include "zen_view_gl_resource.h"
#include "zen_system.h"
#include "zen_image.h"
#include "zen_image_png.h"
#include "zen_image_jpg.h"
#include "zen_image_raw.h"

namespace Zen { namespace Vap2d {
	static uint32_t GetExtendSize(uint32_t s)
	{
		for (int i = 0; i < 16; ++i) {
			if (s < (1U << i)) return (1U << i);
		}
		return 0;
	}

	static int sMaxResouceID = 0;
	static Zen::ImagePNG sImageCoderPNG;
	static Zen::ImageJPG sImageCoderJPG;
	static Zen::ImageRaw sImageCoderRaw;

	GLImage::GLImage()
	{
		mID = ++sMaxResouceID;
	}
	GLImage::~GLImage()
	{
		mGLImage.release();
	}

	void GLImage::load(Zen::GL::ETextureFmt fmt, int width, int height, int sample_bytes, const void *data)
	{
			//#if ZEN_GL_2D_ANY_SZ
#if 0
		mGLImage.create();
		mGLImage.bindData(width, height, fmt, data, 0);

		mWidth = width;
		mHeight = height;
		mGLWidth = mGLHeight = 1.f;
#else

		auto w = GetExtendSize(width);
		auto h = GetExtendSize(height);

		if(w == width)
		{
			if(h == height)
			{
				mGLImage.create();
				mGLImage.bindData(width, height, fmt, data, 0);
			}
			else
			{
				auto src_size = width * height * sample_bytes;
				auto dest_size = w * h * sample_bytes;
				std::vector<char> buf(dest_size, 0);
				::memcpy(buf.data(), data, src_size);
				mGLImage.create();
				mGLImage.bindData(w, h, fmt, buf.data(), 0);
			}
		}
		else
		{
			auto dest_stride = sample_bytes * w;
			auto src_stride = sample_bytes * width;
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
			mGLImage.create();
			mGLImage.bindData(w, h, fmt, buf.data(), 0);
		}
		mWidth = width;
		mHeight = height;
		mGLWidth = (float)mWidth /(float)w;
		mGLHeight = (float)mHeight /(float)h;
#endif
	}

	void GLImage::useAsSampler(int nth)
	{
		mGLImage.active(nth);
	}

	int GLImage::getWidth()
	{
		return mWidth;
	}

	int GLImage::getHeight()
	{
		return mHeight;
	}

	float GLImage::getGLWidth()
	{
		return mGLWidth;
	}

	float GLImage::getGLHeight()
	{
		return mGLHeight;
	}

	std::shared_ptr<Zen::Vap2d::Resource> GLImage::Create(Zen::Image const & image, std::string const & name)
	{
		if(image.width() == 0 || image.height() == 0) return nullptr;
		Zen::GL::ETextureFmt fmt = Zen::GL::ETextureFmt::Alpha;
		int sample_types = 0;
		switch (image.format()) {
			case Zen::EImageFormat::RGBA:
				sample_types = 4;
				fmt = Zen::GL::ETextureFmt::RGBA;
				break;
			case Zen::EImageFormat::RGB:
				sample_types = 3;
				fmt = Zen::GL::ETextureFmt::RGB;
				break;
			case Zen::EImageFormat::Grey:
				sample_types = 1;
				fmt = Zen::GL::ETextureFmt::Alpha;
				break;
			default:
				break;
		}
		if(sample_types == 0) return nullptr;
		auto me = new GLImage;
		me->_setName(name);
		me->load(fmt, image.width(), image.height(), sample_types, image.bytes());
		return std::shared_ptr<Zen::Vap2d::Resource>(me);
	}

	std::shared_ptr<Resource> GLResourceManager::getImage(const std::string & path)
	{
		auto iter = mTextureCache.find(path);
		if(iter != mTextureCache.end()) return iter->second;

		auto data = Zen::System::LoadResourceContent(path);
		if(data.size() == 0) return nullptr;

		Zen::Image image;
		int type = 0;
		if(path.size() > 4)
		{
			auto tail = path.substr(path.size()-4);
			if(tail == ".raw") type = 1;
			else if(tail == ".png") type = 2;
			else if(tail == ".jpg" || tail == "jpeg") type = 3;
		}
		do {
			if(type == 1 || type == 0)
			{
				try {
					image.decode(data, sImageCoderRaw);
				} catch(...) {}
				if(image.size()) break;
			}
			if(type == 2 || type == 0)
			{
				try {
					image.decode(data, sImageCoderPNG);
				} catch(...) {}
				if(image.size()) break;
			}
			if(type == 3 || type == 0)
			{
				try {
					image.decode(data, sImageCoderJPG);
				} catch(...) {}
				if(image.size()) break;
			}
			return nullptr;
		} while(false);

		auto resource = GLImage::Create(image, path);
		if(!resource) return nullptr;
		return (mTextureCache[path] = resource);
	}

	ResourceType GLImage::getType()
	{
		return ResourceType::Image;
	}

	int GLImage::getID()
	{
		return mID;
	}

	void GLImage::_setName(std::string name)
	{
		mName = name;
	}
	std::string GLImage::getName()
	{
		return mName;
	}
	GLResourceManager::GLResourceManager()
	{

	}
	void GLResourceManager::clearUnused(ResourceType type)
	{
		if(type == ResourceType::Image)
		{
			for(auto iter = mTextureCache.begin(); iter != mTextureCache.end();)
			{
				if(iter->second.use_count() == 1)
				{
					iter = mTextureCache.erase(iter);
				}
				else ++iter;
			}
		}
	}

	GLResourceManager * GLResourceManager::GetDefault()
	{
		static GLResourceManager * me = new GLResourceManager;
		return me;
	}
	ResourceManager * ResourceManager::GetDefault()
	{
		return GLResourceManager::GetDefault();
	}
}}
