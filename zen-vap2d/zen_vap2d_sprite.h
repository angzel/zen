#pragma once

#include "zen_vap2d_node.h"
#include "zen_gles2_texture.h"
#include "zen_rect.h"
#include "zen_gles2_shader_sampler.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap2d {

	class Sprite : public BNode, public SizeView {
	protected:
		Zen::GL::SharedTexture m_texture_src;
		Zen::Rect m_texture_rect;
		Zen::GL::Buffer m_texture_buffer;
		bool m_is_texture_dirty = false;
		Zen::Color4f m_color;
		Zen::GL::ShaderSampler const * m_shader = nullptr;
		bool m_is_flip_x = false;
		bool m_is_flip_y = false;
	public:
		Sprite();

		void setTexture(Zen::GL::SharedTexture texture);

		void setTextureRect(Zen::Rect rect);

		Zen::GL::SharedTexture getTexture();

		Zen::Rect getTextureRect();

		void setTextureColor(Zen::Color4f color);

		Zen::Color4f getTextureColor();

		void setTextureFlipX(bool x);

		void setTextureFlipY(bool y);

		bool isTextureFlipX();

		bool isTextureFlipY();

		void setEffect(Zen::GL::ShaderSampler const * shader);
		
		virtual void draw() override;
	};
}}
