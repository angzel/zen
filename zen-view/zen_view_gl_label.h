#pragma once

#include "zen_view_label.h"
#include "zen_view_gl_node.h"
#include "zen_view_gl_resource.h"
#include "zen_font_render.h"

namespace Zen { namespace Vap2d {

	class GLLabel : public virtual Label, public virtual GLFinalNode, public virtual GLBoundFigure
	{
	public:
		GLLabel();

		virtual void draw() override;
		
		virtual void setFontSize(float size) override;

		virtual float getFontSize() override;

		virtual void setText(std::string const & text) override;

		virtual std::string getText() override;

		virtual void setFont(std::shared_ptr<Zen::Font> font) override;

		virtual std::shared_ptr<Zen::Font> getFont() override;

		virtual void setAlignment(Zen::Vap2d::ETextAlignment) override;

		virtual Zen::Vap2d::ETextAlignment getAlignment() override;

		virtual void setMaxLineWidth(float) override;

		virtual float getMaxLineWidth() override;
	protected:
		void  _updateText();

		std::string mText;
		float mFontSize = 12.f;
		int mMaxLine = 0;
		Zen::Vap2d::ETextAlignment mAlignment = Zen::Vap2d::ETextAlignment::Left;
		bool mIsTextDirty = false;

	public:
		virtual void setAutoResize(bool) override;

		virtual bool isAutoResize() override;

		virtual Zen::Pt2 getSize() override;

		virtual void setSize(Zen::Pt2) override;

		virtual void setFlipX(bool) override;

		virtual void setFlipY(bool) override;

		virtual bool isFlipX() override;

		virtual bool isFlipY() override;
	protected:
		GLImage* _clearImageDirty();
		void _updateCoords(GLImage* texture);
		void _updateSize(GLImage* texture);

		std::shared_ptr<Resource> mTexture;
		std::shared_ptr<Zen::Font> mFont;
		bool mIsImageDirty = false;
		Zen::Pt2 mSamplerCoords[4];
		bool mIsAutoResize = true;
		bool mIsSizeDirty = false;
		bool mIsFlipX = false;
		bool mIsFlipY = false;

		Mat4 mFinalMat;
		int mParentMatVersion = 0;
	};
}}
