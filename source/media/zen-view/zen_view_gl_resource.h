#pragma once
#include "zen_gl.h"
#include "zen_image.h"
#include "zen_view_resource.h"
#include <vector>
#include <map>

namespace Zen { namespace Vap2d {
	class GLImage : public Resource
	{
	protected:
		Zen::GL::Texture mGLImage;
		int mWidth;
		int mHeight;
		float mGLWidth;
		float mGLHeight;
	public:
		static std::shared_ptr<Zen::Vap2d::Resource> Create(Zen::Image const & image, std::string const & name);

		GLImage();

		virtual ~GLImage();

		void load(Zen::GL::ETextureFmt fmt, int width, int height, int sample_bytes, const void *data);

		void useAsSampler(int nth);

		int getWidth();

		int getHeight();

		float getGLWidth();

		float getGLHeight();

		void _setName(std::string name);

		virtual ResourceType getType() override;

		virtual int getID() override;

		virtual std::string getName() override;

	protected:
		int mID = 0;
		std::string mName;
	};
	class GLResourceManager : public ResourceManager
	{
	public:
		static GLResourceManager * GetDefault();

		GLResourceManager();

		virtual std::shared_ptr<Resource> getImage(std::string const & path) override;

		virtual void clearUnused(ResourceType type) override;

	protected:
		std::map<std::string, std::shared_ptr<Resource> > mTextureCache;
	};
}}
