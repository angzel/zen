#pragma once

#include "zen_vap2d_config.h"
#include "zen_vap2d_node.h"
#include "zen_vap2d_texture.h"
#include "zen_rect.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap2d {

	class Sprite : public FinalNode, public SizeView, public Colorful, public Flipable {
	public:
		Sprite(std::string const & name = "sprite");

		void setTexture(SharedTexture texture);
		SharedTexture getTexture();

		void setTextureRect(Zen::Rect4f rect);
		Zen::Rect4f getTextureRect();

		void setAreaSize(Size2 size);

		Size2 getAreaSize();

		virtual void draw() override;
		
	protected:
		SharedTexture m_texture;
		Zen::Rect4f m_texture_rect = {0, 0, 1, 1};
		Size2 m_size = {0, 0};
		Size2 m_texture_size = {0, 0};
		bool m_is_texture_dirty = true;

		SpriteBuffer m_gpus;
		bool m_is_buffer_dirty = false;
		
	protected:
		void _initSprite();
		void _drawSprite();
	};
}}
