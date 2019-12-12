
#include "zen_vap2d_doodle.h"
#include "zen_gles2_shader_sampler.h"
#include "zen_vap2d_root.h"

using namespace Zen::GL;

namespace Zen { namespace Vap2d {

	Doodle::Doodle(std::string const & name)
	: LNode(name)
	{
		this->setSize(Root::GetDefault()->getViewSize());
	}

	void Doodle::addCoord(Zen::Point2 p, Zen::Color4f c)
	{
		m_buffer.push_back( { c, p } );
		m_is_buffer_dirty = true;
	}

	void Doodle::clearCoords()
	{
		m_buffer.clear();
		m_is_buffer_dirty = true;
	}

	void Doodle::setDrawMode(EDrawMode mode, float point_size)
	{
		m_point_size = point_size;
		m_mode = mode;
	}
	size_t Doodle::getCoordsCount()
	{
		return m_buffer.size();
	}
	void Doodle::setColor(Zen::Color4f color)
	{
		m_color = color;
	}
	Zen::Color4f Doodle::getColor()
	{
		return m_color;
	}
	
	void Doodle::draw()
	{
		if(m_buffer.empty()) return;

		if(m_is_buffer_dirty)
		{
			m_is_buffer_dirty = false;
			m_array_buffer.bindData(m_buffer.size()*sizeof(Dot), m_buffer.data());
		}

		this->updateWorldMatrix();

		auto alpha = RenderStack::GetDefault()->getTopAlpha() * this->getAlpha();

		using namespace Zen::GL;
		auto ss = m_shader;
		if(ss == nullptr) ss = ShaderColor::GetNormal();

		RenderStack::GetDefault()->performBlend(m_blend);

		Render::ActiveProgram(ss->program.getObject());
		Render::BindArrayBuffer(m_array_buffer.getObject());
		Render::EnableVertexAttrib(ss->a_color);
		Render::EnableVertexAttrib(ss->a_coord);
		Render::SetVertexAttribBuffer(ss->a_color, 4, EType::Float, 0, sizeof(Dot), 0);
		Render::SetVertexAttribBuffer(ss->a_coord, 2, EType::Float, 0, sizeof(Dot), 4*sizeof(float));
		Render::SetUniformFloat(ss->u_color, m_color.red, m_color.green, m_color.blue, m_color.alpha*alpha);
		Render::SetUniformMat(ss->u_transform, m_world_matrix);
		switch (m_mode) {
			case EDrawMode::Points:
				Render::SetUniformFloat(ss->u_point_size, m_point_size/Root::GetDefault()->getViewScale());
				break;
			case EDrawMode::Lines:
			case EDrawMode::LineStrip:
			case EDrawMode::LineLoop:
				Render::SetLineWidth(m_point_size/Root::GetDefault()->getViewScale());
				break;
			default:
				break;
		}
		Render::DrawArray(m_mode, 0, m_buffer.size());

		Render::BindArrayBuffer(0);
	}

}}


namespace Zen { namespace Vap2d {
	DoodleColors::DoodleColors()
	{
		setDrawMode(EDrawMode::TriangleStrip, 0);

		this->addCoord({0, 0}, Zen::Color4f(0xff88ffff));
		this->addCoord({1, 0}, Zen::Color4f(0x88ffffff));
		this->addCoord({0, 1}, Zen::Color4f(0x8888ffff));
		this->addCoord({1, 1}, Zen::Color4f(0x88ff88ff));

		this->setSize(10, 10);
	}
	void DoodleColors::setCoordColor(int index, Zen::Color4f color)
	{
		m_buffer[index].color = color;
		m_is_buffer_dirty = true;
	}
	Zen::Color4f DoodleColors::getCoordColor(int index)
	{
		return m_buffer[index].color;
	}

	DoodleLine::DoodleLine()
	{
		this->setSize(1, 1);

		setDrawMode(EDrawMode::Lines, 0);

		this->addCoord({0, 0}, Zen::EColor::White);
		this->addCoord({1, 0}, Zen::EColor::White);
	}
	void DoodleLine::setBeginColor(Zen::Color4f color)
	{
		m_buffer[0].color = color;
	}
	void DoodleLine::setEndColor(Zen::Color4f color)
	{
		m_buffer[1].color = color;
	}
	void DoodleLine::setBeginCoord(Zen::Point2 coord)
	{
		m_buffer[1].coord = coord;
	}
	void DoodleLine::setEndCoord(Zen::Point2 coord)
	{
		m_buffer[1].coord = coord;
	}
	Zen::Color4f DoodleLine::getBeginColor()
	{
		return m_buffer[0].color;
	}
	Zen::Color4f DoodleLine::getEndColor()
	{
		return m_buffer[1].color;
	}
	Zen::Point2 DoodleLine::getBeginCoord()
	{
		return m_buffer[0].coord;
	}
	Zen::Point2 DoodleLine::getEndCoord()
	{
		return m_buffer[1].coord;
	}
	void DoodleLine::setLineWidth(float width)
	{
		m_point_size = width;
	}
}}
