
#include "zen_vap2d_sprite.h"
#include "zen_vap2d_root.h"

namespace Zen { namespace Vap2d {

	Sprite::Sprite(std::string const & name)
	: FinalNode(name)
	{
		_initSprite();
	}

	void Sprite::setTexture(SharedTexture texture)
	{
		m_texture = texture;
		m_is_texture_dirty = true;
	}

	void Sprite::setTextureRect(Zen::Rect4f rect)
	{
		m_texture_rect = rect;
		m_is_texture_dirty = true;
	}

	SharedTexture Sprite::getTexture()
	{
		return m_texture;
	}

	Zen::Rect4f Sprite::getTextureRect()
	{
		return m_texture_rect;
	}

	void Sprite::setAreaSize(Size2 size)
	{
		m_size = size;
		if(m_size.w && m_size.h)
		{
			this->_setScale2(m_size.w, m_size.h);
		}
		else if(!m_is_texture_dirty)
		{
			this->_setScale2(m_texture_size.w, m_texture_size.h);
		}
	}

	Size2 Sprite::getAreaSize()
	{
		return m_size;
	}
	void Sprite::draw()
	{
		if(!m_texture) return;

		if(m_is_texture_dirty) {

			m_is_texture_dirty = false;
			m_is_buffer_dirty = true;

			m_texture_size.w = m_texture->size().w * m_texture_rect.w;
			m_texture_size.h = m_texture->size().h * m_texture_rect.h;
			
			if(m_size.w == 0 || m_size.h == 0)
			{
				this->_setScale2(m_texture_size.w, m_texture_size.h);
			}
		}

		this->_drawSprite();
	}
}}

