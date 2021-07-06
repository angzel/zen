
#include "zen_vap_doodle.h"
#include "zen_vap_root.h"
#include "zen_exception.h"

namespace Zen { namespace Vap {
	
	FreeDoodle::FreeDoodle()
	{
	}
	
	void FreeDoodle::addPoint(Zen::Point2 p, Zen::Color4f c)
	{
		m_dots.push_back({ {p.x / m_scale2.x, p.y / m_scale2.y }, c });
		this->m_RBuf.is_dirty = true;
	}
	
	size_t FreeDoodle::getPointsCount()
	{
		return m_dots.size();
	}
	
	void FreeDoodle::removeAllPoints()
	{
		m_dots.clear();
		this->m_RBuf.is_dirty = true;
	}
	
	void FreeDoodle::setPointPosition(int index, Zen::Point2 p)
	{
		m_dots[index].coord = Point2Make(p.x / m_scale2.x, p.y / m_scale2.y);
		this->m_RBuf.is_dirty = true;
	}
	
	void FreeDoodle::setPointColor(int index, Zen::Color4f c)
	{
		m_dots[index].color = c;
		this->m_RBuf.is_dirty = true;
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

namespace Zen { namespace Vap {
	ColoredRect::ColoredRect()
	{
		m_is_anchor_valid = true;
		m_mode = eVertexMode::TriangleStrip;
		m_dots.push_back({ { 0, 0 }, Zen::Color4f(0xff88ffff) });
		m_dots.push_back({ { 1, 0 }, Zen::Color4f(0x88ffffff) });
		m_dots.push_back({ { 0, 1 }, Zen::Color4f(0x8888ffff) });
		m_dots.push_back({ { 1, 1 }, Zen::Color4f(0x88ff88ff) });
		
		m_RBuf.is_dirty = true;
	}
	void ColoredRect::setRectSize(Size2 s)
	{
		setScale2_(s.w, s.h);
	}
	void ColoredRect::setCoordColor(int index, Zen::Color4f color)
	{
		m_dots[index].color = color;
		m_RBuf.is_dirty = true;
	}
	Zen::Color4f ColoredRect::getCoordColor(int index)
	{
		return m_dots[index].color;
	}
	
	ColoredLine::ColoredLine()
	{
		m_mode = eVertexMode::Line;
		m_dots.push_back({ { 0, 0 }, eColor::Black });
		m_dots.push_back({ { 1, 1 }, eColor::White });
		
		m_RBuf.is_dirty = true;
	}
	void ColoredLine::setBeginColor(Zen::Color4f color)
	{
		m_dots[0].color = color;
		m_RBuf.is_dirty = true;
		
	}
	void ColoredLine::setEndColor(Zen::Color4f color)
	{
		m_dots[1].color = color;
		m_RBuf.is_dirty = true;
		
	}
	void ColoredLine::setBeginCoord(Zen::Point2 coord)
	{
		m_dots[1].coord = coord;
		m_RBuf.is_dirty = true;
	}
	void ColoredLine::setEndCoord(Zen::Point2 coord)
	{
		m_dots[1].coord = coord;
		m_RBuf.is_dirty = true;
		
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

namespace Zen { namespace Vap {
	
	Doodle::Doodle(std::string const & name)
	: Node2d(name), View2d(false)
	{
		auto vs = Root::Get()->getRealViewSize();
		this->setScale2_(vs.w, vs.h);
		m_view_scale = { 1/vs.w, 1/vs.h };
		initR_();
	}
	
	void Doodle::draw()
	{
		if(m_dots.empty()) return;
		this->updateMatrix();
		this->drawR_();
	}
}}
namespace Zen { namespace Vap {
#if ZEN_DRAW_GLES
	void Doodle::initR_()
	{
		
	}
	void Doodle::drawR_()
	{
		if(m_RBuf.is_dirty)
		{
			m_RBuf.is_dirty = false;
			if(m_RBuf.size != m_dots.size())
			{
				m_RBuf.size = m_dots.size();
				m_RBuf.v.create(sizeof(VertexData)* m_dots.size(), m_dots.data());
			}
			else
			{
				m_RBuf.v.update(0, sizeof(VertexData)* m_dots.size(), m_dots.data());
			}
		}
		
		auto alpha = DrawStack::Get()->getTopAlpha() * this->getAlpha();
		
		auto sha = GL::ShaderPrograms::Get()
		->getShaderC(m_is_grey_mode);
		
		GLHelper::PerformBlend(m_blend);
		
		auto s = GLHelper::GetGLRender();
		
		s->activeProgram(sha->program.getID());
		s->enableBuffer(m_RBuf.v.getID());
		
		s->setVertexBuffer(sha->a_color, 4, GL::eType::Float, false, sizeof(VertexData), 2*sizeof(float));
		s->setVertexBuffer(sha->a_coord, 2, GL::eType::Float, false, sizeof(VertexData), 0);
		
		s->setUniform(sha->u_color, m_color.red, m_color.green, m_color.blue, m_color.alpha*alpha);
		s->setUniform(sha->u_transform, getMatrix());
		
		s->drawArray((GL::eVertexMode)(int)m_mode, 0, m_dots.size());
	}
#elif ZEN_DRAW_METAL
	
	void Doodle::initR_()
	{
	}
	void Doodle::drawR_()
	{
//		if(m_dots.size() == 0) return;
		if(m_RBuf.is_dirty)
		{
			m_RBuf.is_dirty = false;
			
			must(sizeof(VertexData) == sizeof(VertexIN_ShaderC));
			if(m_RBuf.v_count() == m_dots.size())
			{
				m_RBuf.updateVertexBuffer((VertexIN_ShaderC*)(void*)m_dots.data(), m_dots.size(), 0);
			}
			else
			{
				m_RBuf.createVertexBuffer((VertexIN_ShaderC*)(void*)m_dots.data(), m_dots.size());
				
			}
		}
		
		m_RBuf.updateMatrix(getMatrix(), getMatrixVersion(), false);
		
		float alpha = getAlpha() * DrawStack::Get()->getTopAlpha();
		m_RBuf.updateColor(getColor(), alpha, m_is_color_dirty);
		m_is_color_dirty = false;
		
		auto r = Metal::Render::Get();
		
		auto blends = MetalHelper::GetBlends(m_blend);
		
		auto pipe = Metal::PipeC::GetShared
		(m_is_grey_mode, blends.first, blends.second);
		
		r->activePipe(pipe->getID());
		
		r->setVertexBuffer(pipe->VertAttribIndex, m_RBuf.v().getID(), 0);
		r->setVertexBuffer(pipe->UniformIndex, m_RBuf.u().getID(), 0);
		
		r->drawPrimitives(m_mode, 0, m_RBuf.v_count());
	}
	
#endif
	
}}
