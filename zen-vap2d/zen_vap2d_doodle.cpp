
#include "zen_vap2d_doodle.h"
#include "zen_vap2d_root.h"

namespace Zen { namespace Vap2d {

	Doodle::Doodle(std::string const & name)
	: FinalNode(name)
	{
		_initDoodle();
	}

	void Doodle::addCoord(Zen::Point2 p, Zen::Color4f c)
	{
		m_dots.push_back( { c, p } );
		m_is_buffer_dirty = true;
	}

	void Doodle::clearCoords()
	{
		m_dots.clear();
		m_is_buffer_dirty = true;
	}


	void Doodle::setLineMode(bool strip)
	{
		m_mode = (strip? eMode::LineStrip : eMode::Line);
	}

	void Doodle::setTriangleMode(bool strip)
	{
		m_mode = (strip? eMode::TriangleStrip : eMode::Triangle);
	}

	size_t Doodle::getCoordsCount()
	{
		return m_dots.size();
	}

	void Doodle::setAreaSize(Size2 size)
	{
		this->_setScale2(size.w, size.h);
	}

	Size2 Doodle::getAreaSize()
	{
		return m_scale2;
	}
	void Doodle::draw()
	{
		if(m_dots.empty()) return;

		this->_drawDoodle();
	}
}}


namespace Zen { namespace Vap2d {
	DoodleColors::DoodleColors()
	{
		this->setTriangleMode(true);

		this->addCoord({0, 0}, Zen::Color4f(0xff88ffff));
		this->addCoord({1, 0}, Zen::Color4f(0x88ffffff));
		this->addCoord({0, 1}, Zen::Color4f(0x8888ffff));
		this->addCoord({1, 1}, Zen::Color4f(0x88ff88ff));
	}
	void DoodleColors::setCoordColor(int index, Zen::Color4f color)
	{
		m_dots[index].color = color;
		m_is_buffer_dirty = true;
	}
	Zen::Color4f DoodleColors::getCoordColor(int index)
	{
		return m_dots[index].color;
	}

	DoodleLine::DoodleLine()
	{
		this->setLineMode(false);
		
		this->addCoord({0, 0}, Zen::eColor::White);
		this->addCoord({1, 1}, Zen::eColor::White);
	}
	void DoodleLine::setBeginColor(Zen::Color4f color)
	{
		m_dots[0].color = color;
		m_is_buffer_dirty = true;
	}
	void DoodleLine::setEndColor(Zen::Color4f color)
	{
		m_dots[1].color = color;
		m_is_buffer_dirty = true;
	}
	void DoodleLine::setBeginCoord(Zen::Point2 coord)
	{
		m_dots[1].coord = coord;
		m_is_buffer_dirty = true;
	}
	void DoodleLine::setEndCoord(Zen::Point2 coord)
	{
		m_dots[1].coord = coord;
		m_is_buffer_dirty = true;
	}
	Zen::Color4f DoodleLine::getBeginColor()
	{
		return m_dots[0].color;
	}
	Zen::Color4f DoodleLine::getEndColor()
	{
		return m_dots[1].color;
	}
	Zen::Point2 DoodleLine::getBeginCoord()
	{
		return m_dots[0].coord;
	}
	Zen::Point2 DoodleLine::getEndCoord()
	{
		return m_dots[1].coord;
	}
}}
