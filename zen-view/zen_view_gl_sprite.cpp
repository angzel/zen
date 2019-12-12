
#include "zen_view_gl_sprite.h"
#include "zen_gl_shader_sampler.h"
#include "zen_view_gl_resource.h"

namespace Zen { namespace Vap2d {
	static Zen::Pt4 sCoords[4] = {
		{0, 0, 0, 1},
		{1, 0, 0, 1},
		{1, 1, 0, 1},
		{0, 1, 0, 1},
	};

	GLSprite::GLSprite()
	{
	}
	void GLSprite::_updateSize(GLImage* texture)
	{
		float x = texture->getWidth() * mImageRect.size().x;
		float y = texture->getHeight() * mImageRect.size().y;
		GLBoundFigure::setSize({x, y});
	}
	void GLSprite::_updateCoords(GLImage* texture)
	{
		float x = texture->getGLWidth() * mImageRect.pos().x;
		float y = texture->getGLHeight() * mImageRect.pos().y;
		float x2 = texture->getGLWidth() * mImageRect.size().x + x;
		float y2 = texture->getGLHeight() * mImageRect.size().y + y;
		if(mIsFlipX) std::swap(x, x2);
		if(mIsFlipY) std::swap(y, y2);
		mSamplerCoords[0] = { x, y2 };
		mSamplerCoords[1] = { x2, y2 };
		mSamplerCoords[2] = { x2, y };
		mSamplerCoords[3] = { x, y };

		mIsSizeDirty = true;
	}

	GLImage* GLSprite::_clearImageDirty()
	{
		auto texture = dynamic_cast<GLImage*>(mTexture.get());
		if(!texture) return nullptr;

		if(mIsImageDirty)
		{
			this->_updateCoords(texture);
			mIsImageDirty = false;
		}
		if(mIsAutoResize && mIsSizeDirty)
		{
			this->_updateSize(texture);
			mIsSizeDirty = false;
		}
		return texture;
	}
	void GLSprite::draw()
	{
		if(!this->isVisible()) return;

		auto texture = this->_clearImageDirty();
		if(!texture) return;

#include "zen_view_gl_update_final_mat_part.h"
		/** begin draw */

		_GLNodeDrawingHelper helper(this);

		texture->useAsSampler(1);
		auto p = Zen::GL::ShaderSampler::GetNormal();
		p->active();
		using namespace Zen::GL;
		DrawAPI::EnableVertexAttrib(p->getAttribCoord());
		DrawAPI::SetVertexAttribData(p->getAttribCoord(), 4, EDataType::Float, false, 0, &sCoords);

		DrawAPI::EnableVertexAttrib(p->getAttribSampleCoord());
		DrawAPI::SetVertexAttribData(p->getAttribSampleCoord(), 2, EDataType::Float, true, 0, &mSamplerCoords);

		float a = helper.getFinalOpacity();
		DrawAPI::SetUniformFloat(p->getUniformColor(), mColor.red * a, mColor.green * a, mColor.blue * a, mColor.alpha * a);

		DrawAPI::SetUniformMat(p->getUniformTransform(), mFinalMat);

		DrawAPI::SetUniformInt(p->getUniformSample(), 1);

		DrawAPI::DrawArray(Zen::GL::EDrawMode::TriangleFan, 0, 4);
	}

	void GLSprite::setSize(Zen::Pt2 size)
	{
		if(mIsAutoResize) return;
		GLBoundFigure::setSize(size);
	}
	Zen::Pt2 GLSprite::getSize()
	{
		if(mIsAutoResize)
		{
			this->_clearImageDirty();
		}
		return GLBoundFigure::getSize();
	}

	void GLSprite::setImage(std::shared_ptr<Resource> image)
	{
		mTexture = image;
		mIsImageDirty = true;
	}

	std::shared_ptr<Resource> GLSprite::getImage()
	{
		return mTexture;
	}

	void GLSprite::setImageWithRect(std::shared_ptr<Resource> image, Zen::Rect rect)
	{
		mImageRect = rect;
		mTexture = image;
		mIsImageDirty = true;
	}

	void GLSprite::setImageRect(Zen::Rect rect)
	{
		mImageRect = rect;
		mIsImageDirty = true;
	}

	Zen::Rect GLSprite::getImageRect()
	{
		return mImageRect;
	}
	void GLSprite::setAutoResize(bool v)
	{
		if(v == mIsAutoResize) return;
		mIsAutoResize = v;
		if(v) mIsSizeDirty = true;
	}
	bool GLSprite::isAutoResize()
	{
		return mIsAutoResize;
	}
	void GLSprite::setFlipX(bool x)
	{
		if(mIsFlipX == x) return;
		mIsFlipX = x;
		mIsImageDirty = true;
	}
	void GLSprite::setFlipY(bool y)
	{
		if(mIsFlipY == y) return;
		mIsFlipY = y;
		mIsImageDirty = true;
	}
	bool GLSprite::isFlipX()
	{
		return mIsFlipX;
	}

	bool GLSprite::isFlipY()
	{
		return mIsFlipY;
	}

	Sprite * Sprite::Create()
	{
		auto sp = new GLSprite;
		return sp;
	}

}}
