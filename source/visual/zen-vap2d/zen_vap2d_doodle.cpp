
#include "zen_vap2d_doodle.h"
#include "zen_vap2d_root.h"

namespace Zen { namespace Vap2d {

	Doodle::Doodle(std::string const & name)
	: FinalNode(name)
	{
		_initDoodle();
	}
	
	void Doodle::setDotsDirty()
	{
		m_is_buffer_dirty = true;
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

	FreeDoodle::FreeDoodle()
	{
	}

	void FreeDoodle::addPoint(Zen::Point2 p, Zen::Color4f c)
	{
		m_dots.push_back({ c, p });
		this->setDotsDirty();
	}

	size_t FreeDoodle::getPointsCount()
	{
		return m_dots.size();
	}

	void FreeDoodle::removeAllPoints()
	{
		m_dots.clear();
		this->setDotsDirty();
	}

	void FreeDoodle::setPointPosition(int index, Zen::Point2 p)
	{
		m_dots[index].coord = p;
		this->setDotsDirty();
	}

	void FreeDoodle::setPointColor(int index, Zen::Color4f c)
	{
		m_dots[index].color = c;
		this->setDotsDirty();
	}

	Zen::Point2 FreeDoodle::getPointPosition(int index)
	{
		return m_dots[index].coord;
	}

	Zen::Color4f FreeDoodle::getPointColor(int index)
	{
		return m_dots[index].color;
	}

	void FreeDoodle::setLineMode(bool strip)
	{
		m_mode = (strip? eVertexMode::LineStrip : eVertexMode::Line);
	}

	void FreeDoodle::setTriangleMode(bool strip)
	{
		m_mode = (strip? eVertexMode::TriangleStrip : eVertexMode::Triangle);
	}
}}

namespace Zen { namespace Vap2d {
	ColoredRect::ColoredRect()
	{
		m_mode = eVertexMode::TriangleStrip;
		m_dots.push_back({ Zen::Color4f(0xff88ffff), {0, 0} });
		m_dots.push_back({ Zen::Color4f(0x88ffffff), {1, 0} });
		m_dots.push_back({ Zen::Color4f(0x8888ffff), {0, 1} });
		m_dots.push_back({ Zen::Color4f(0x88ff88ff), {1, 1} });

		setDotsDirty();
	}
	void ColoredRect::setCoordColor(int index, Zen::Color4f color)
	{
		m_dots[index].color = color;
		m_is_buffer_dirty = true;
	}
	Zen::Color4f ColoredRect::getCoordColor(int index)
	{
		return m_dots[index].color;
	}

	ColoredLine::ColoredLine()
	{
		m_mode = eVertexMode::Line;
		m_dots.push_back({ eColor::Black, {0, 0} });
		m_dots.push_back({ eColor::White, {1, 1} });

		setDotsDirty();
	}
	void ColoredLine::setBeginColor(Zen::Color4f color)
	{
		m_dots[0].color = color;
		setDotsDirty();

	}
	void ColoredLine::setEndColor(Zen::Color4f color)
	{
		m_dots[1].color = color;
		setDotsDirty();

	}
	void ColoredLine::setBeginCoord(Zen::Point2 coord)
	{
		m_dots[1].coord = coord;
		setDotsDirty();
	}
	void ColoredLine::setEndCoord(Zen::Point2 coord)
	{
		m_dots[1].coord = coord;
		setDotsDirty();

	}
	Zen::Color4f ColoredLine::getBeginColor()
	{
		return m_dots[0].color;
	}
	Zen::Color4f ColoredLine::getEndColor()
	{
		return m_dots[1].color;
	}
	Zen::Point2 ColoredLine::getBeginCoord()
	{
		return m_dots[0].coord;
	}
	Zen::Point2 ColoredLine::getEndCoord()
	{
		return m_dots[1].coord;
	}
}}
