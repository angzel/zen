#pragma once

#include "zen_vap_render.h"
#include "zen_vap_node.h"
#include "zen_vap_texture.h"
#include "zen_types.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap {

	class Sprite : public Node2d, public View2d, public Colorful, public Flipable {
	public:
		Sprite(std::string const & name = "sprite");

		void setTexture(TextureShared texture);
		TextureShared getTexture();

		void setTextureRect(Zen::Rect4f rect);
		Zen::Rect4f getTextureRect();

		void enableFixedSize(bool);
		
		bool isFixedSizeEnabled();
		
		void setFixedSize(Size2 size);
		
		Size2 getFixedSize();

		Size2 getContentSize();

		void updateContent();
		
		virtual void draw() override;
		
	protected:
		TextureShared m_texture;
		Zen::Rect4f m_texture_rect = { 0, 0, 1, 1 };
		Size2 m_fixed_size = { 0, 0 };
		Size2 m_texture_size = { 0, 0 };
		
		bool m_is_texture_dirty = true;
		bool m_is_fixed_sized = false;

	protected:
		
		RBufferTexture m_RBuf;
		
		void initR_();
		void drawR_();
	};
}}
