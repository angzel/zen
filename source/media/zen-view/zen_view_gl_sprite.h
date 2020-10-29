#pragma once

#include "zen_view_sprite.h"
#include "zen_view_gl_node.h"
#include "zen_view_gl_resource.h"

namespace Zen { namespace Vap2d {
	class GLSprite : public virtual Sprite, public virtual GLFinalNode, public virtual GLBoundFigure {
	public:
		GLSprite();

		virtual void draw() override;

		virtual void setImage(std::shared_ptr<Resource>) override;

		virtual std::shared_ptr<Resource> getImage() override;

		virtual void setImageWithRect(std::shared_ptr<Resource>, Zen::Rect rect) override;

		virtual void setImageRect(Zen::Rect rect) override;

		virtual Zen::Rect getImageRect() override;

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

		Mat4 mFinalMat;
		int mParentMatVersion = 0;
		
		Zen::Rect mImageRect = Zen::Rect(0, 0, 1, 1);

		std::shared_ptr<Resource> mTexture;
		Zen::Pt2 mSamplerCoords[4];
		bool mIsAutoResize = true;
		bool mIsFlipX = false;
		bool mIsFlipY = false;
		bool mIsSizeDirty = false;
		bool mIsImageDirty = false;

	};
}}
