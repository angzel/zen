#pragma once

#include "zen_vap2d_node.h"
#include "zen_vap2d_texture.h"
#include "zen_rect.h"
#include "zen_gles2_shader_sampler.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap2d {

	class Sprite : public LNode, public SizeView {
	protected:
		SharedTexture m_texture;
		Zen::Rect m_texture_rect = {0, 0, 1, 1};
		Zen::GL::ArrayBuffer m_texture_buffer;
		bool m_is_texture_dirty = false;
		Zen::Color4f m_color = Zen::EColor::White;
		std::shared_ptr<Zen::GL::ShaderSampler const> m_shader = nullptr;
		bool m_is_flip_x = false;
		bool m_is_flip_y = false;
	public:
		Sprite(std::string const & name = "sprite");

		void setTexture(SharedTexture texture);
		SharedTexture getTexture();

		void setTextureRect(Zen::Rect rect);
		Zen::Rect getTextureRect();

		void setColor(Zen::Color4f color);
		Zen::Color4f getColor();

		void setTextureFlipX(bool x);

		void setTextureFlipY(bool y);

		bool isTextureFlipX();

		bool isTextureFlipY();

		void setEffect(std::shared_ptr<Zen::GL::ShaderSampler const> shader);
		
		virtual void draw() override;
	};
}}
