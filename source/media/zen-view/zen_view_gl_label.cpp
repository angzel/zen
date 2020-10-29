#include "zen_view_gl_label.h"
#include "zen_gl_shader_sampler.h"
#include "zen_system.h"
#include "zen_utf8.h"

namespace Zen { namespace Vap2d {

	static std::shared_ptr<Zen::Font> sDefaultFont;

	static std::shared_ptr<Zen::Font> GetDefaultFont()
	{
		if(!sDefaultFont)
		{
			sDefaultFont = Zen::FontCache::GetDefault()->getFont("default");
		}
		return sDefaultFont;
	}

	GLLabel::GLLabel()
	{
		mFont = GetDefaultFont();
	}

	void GLLabel::setFont(std::shared_ptr<Zen::Font> font)
	{
		mFont =font;
		mIsTextDirty = true;
	}
	std::shared_ptr<Zen::Font> GLLabel::getFont()
	{
		return mFont;
	}
	void GLLabel::setText(const std::string &text)
	{
		mText = text;
		mIsTextDirty = true;
	}
	void GLLabel::setFontSize(float size)
	{
		if(mFontSize == size) return;
		mFontSize = size;
		mIsTextDirty = true;
	}
	void GLLabel::setAlignment(Zen::Vap2d::ETextAlignment mode)
	{
		if(mAlignment == mode) return;
		mAlignment = mode;
		mIsTextDirty = true;
	}
	std::string GLLabel::getText()
	{
		return mText;
	}
	float GLLabel::getFontSize()
	{
		return mFontSize;
	}
	Zen::Vap2d::ETextAlignment GLLabel::getAlignment()
	{
		return mAlignment;
	}

	void GLLabel::_updateText()
	{
		if(mText.size() == 0)
		{
			mTexture = nullptr;
			return;
		}
		Zen::FontConfig config;
		config.charW = config.charH = mFontSize;
		config.isVerticalWriting = 0;

		Zen::Image image;
		auto text = Zen::UTF8ToUnicode(mText);
		float alignment = 0;
		switch (mAlignment) {
			case ETextAlignment::Left:
				alignment = 0.f;
				break;
			case ETextAlignment::Center:
				alignment = 0.5f;
				break;
			case ETextAlignment::Right:
				alignment = 1.f;
				break;
		}

		auto render = Zen::FontRender::Create(mFont, config, text, alignment, 0, mMaxLine, (bool)mMaxLine);
		render->renderToImage(image, 0, 0, 0, 0);

		if(image.size() == 0)
		{
			mTexture = nullptr;
			return;
		}

		mTexture = Zen::Vap2d::GLImage::Create(image, "temp-label-texture");
		mIsImageDirty = true;
	}
	static Zen::Pt4 sCoords[4] = {
		{0, 0, 0, 1},
		{1, 0, 0, 1},
		{1, 1, 0, 1},
		{0, 1, 0, 1},
	};
	void GLLabel::_updateSize(GLImage* texture)
	{
		float x = texture->getWidth();
		float y = texture->getHeight();
		GLBoundFigure::setSize({x, y});
	}
	void GLLabel::_updateCoords(GLImage* texture)
	{
		float x = 0;
		float y = 0;
		float x2 = texture->getGLWidth();
		float y2 = texture->getGLHeight();
		if(mIsFlipX) std::swap(x, x2);
		if(mIsFlipY) std::swap(y, y2);
		mSamplerCoords[0] = { x, y2 };
		mSamplerCoords[1] = { x2, y2 };
		mSamplerCoords[2] = { x2, y };
		mSamplerCoords[3] = { x, y };

		mIsSizeDirty = true;
	}

	GLImage* GLLabel::_clearImageDirty()
	{
		if(mIsTextDirty)
		{
			this->_updateText();
			mIsTextDirty = false;
		}
		if(mTexture == nullptr) return nullptr;

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
	void GLLabel::draw()
	{
		if(!this->isVisible()) return;

		auto texture = this->_clearImageDirty();
		if(!texture) return;

#include "zen_view_gl_update_final_mat_part.h"
		/** begin draw */

		_GLNodeDrawingHelper helper(this);

		texture->useAsSampler(1);
		auto p = Zen::GL::ShaderSampler::GetAlpha();
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

	void GLLabel::setAutoResize(bool v)
	{
		if(v == mIsAutoResize) return;
		mIsAutoResize = v;
		if(v) mIsSizeDirty = true;
	}
	void GLLabel::setSize(Zen::Pt2 size)
	{
		if(mIsAutoResize) return;
		GLBoundFigure::setSize(size);
	}
	Zen::Pt2 GLLabel::getSize()
	{
		if(mIsAutoResize)
		{
			this->_clearImageDirty();
		}
		return GLBoundFigure::getSize();
	}

	void GLLabel::setMaxLineWidth(float v)
	{
		auto iv = (int)v;
		if(iv < 1) iv = 0;
		if(mMaxLine == iv) return;
		mMaxLine = iv;
		mIsTextDirty = true;
	}

	float GLLabel::getMaxLineWidth()
	{
		return mMaxLine;
	}
	bool GLLabel::isAutoResize()
	{
		return mIsAutoResize;
	}
	void GLLabel::setFlipX(bool x)
	{
		if(mIsFlipX == x) return;
		mIsFlipX = x;
		mIsImageDirty = true;
	}
	void GLLabel::setFlipY(bool y)
	{
		if(mIsFlipY == y) return;
		mIsFlipY = y;
		mIsImageDirty = true;
	}
	bool GLLabel::isFlipX()
	{
		return mIsFlipX;
	}

	bool GLLabel::isFlipY()
	{
		return mIsFlipY;
	}

	Label * Label::Create()
	{
		auto r = new GLLabel;
		return r;
	}

}}
