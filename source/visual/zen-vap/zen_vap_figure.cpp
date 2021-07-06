
#include "zen_vap_figure.h"
#include "zen_vap_root.h"

using namespace std;
namespace Zen { namespace Vap {
	
	FigurePoints::FigurePoints(std::string const & name)
	: FigureBase_<VertexDataPoints>(name)
	{
		this->initR_();
	}
	
	void FigurePoints::setTexture(TextureShared texture)
	{
		m_texture = texture;
		m_is_texture_dirty = true;
	}
	TextureShared FigurePoints::getTexture()
	{
		return m_texture;
	}
	void FigurePoints::updateSize_()
	{
		auto vs = Root::Get()->getRealViewSize();
		if(this->m_scale2.x == vs.w) return;
		/**
		 把 ViewSize 设置为屏幕大小，传入shader的顶点值则会控制在0-1范围。
		 否则会因精度损失，造成画面抖动
		 */
		this->setScale2_(vs.w, vs.h);
		/**
		 final render x,y range is [-1, 1], width=2
		 */
		m_size_ratio = { 0.5f, 0.5f*vs.h/vs.w, 0.f, 0.f };
		
		m_RBuf.is_dirty = true;
	}
	void FigurePoints::draw()
	{
		if(m_dots.empty()) return;
		
		if(m_is_vertex_dirty) {
			m_is_vertex_dirty = false;
			m_RBuf.is_dirty = true;
		}
		this->updateMatrix();
		
		this->updateSize_();
		
		this->drawR_();
	}
#if ZEN_DRAW_GLES
	void FigurePoints::initR_()
	{
		
	}
	void FigurePoints::drawR_()
	{
		
		if(m_RBuf.is_dirty)
		{			
			auto & s = this->m_scale2;
			float sx = 1 / s.x;
			float sy = 1 / s.y;
			std::vector<VertexData> buf(m_dots.size());
			for(size_t i = 0; i < m_dots.size(); ++i)
			{
				auto dst = m_dots[i];
				auto & src = buf[i];
				src.coord.x = dst.coord.x * sx;
				src.coord.y = dst.coord.y * sy;
				src.color = dst.color;
				src.psize = dst.psize;
			}
			
			m_RBuf.is_dirty = false;
			
			auto step = sizeof(VertexData);
			if(m_RBuf.size != buf.size())
			{
				m_RBuf.size = buf.size();
				m_RBuf.v.create(step * buf.size(), buf.data());
			}
			else
			{
				m_RBuf.v.update(0, step * buf.size(), buf.data());
			}
		}
		
		auto texture = (m_texture?m_texture:Textures::Get()->getParticleTexture());

		auto alpha = DrawStack::Get()->getTopAlpha() * this->getAlpha();
		
		auto sha = GL::ShaderPrograms::Get()->getShaderP(m_is_grey_mode, texture->format());
		
		GLHelper::PerformBlend(m_blend);
		
		auto s = GLHelper::GetGLRender();
		
		s->activeProgram(sha->program.getID());
		
		s->enableBuffer(m_RBuf.v.getID());
		
		s->setVertexBuffer(sha->a_coord, 2, GL::eType::Float, false, sizeof(VertexData), 0);
		s->setVertexBuffer(sha->a_color, 4, GL::eType::Float, false, sizeof(VertexData), 2*sizeof(float));
		s->setVertexBuffer(sha->a_size,  1, GL::eType::Float, false, sizeof(VertexData), 6*sizeof(float));
		
		s->setUniform(sha->u_size_ratio, m_size_ratio);
		
		s->setUniform(sha->u_color, m_color.red, m_color.green, m_color.blue, m_color.alpha*alpha);
		if(m_texture) s->setSampler(sha->u_sampler, 1, texture->buffer().gl_buffer.getID());
		else s->setSampler(sha->u_sampler, 1, 0);
		s->setUniform(sha->u_transform, getMatrix());
		s->drawArray(GL::eVertexMode::Point, 0, m_dots.size());
	}
	
#elif ZEN_DRAW_METAL
	void FigurePoints::initR_()
	{
		
	}
	void FigurePoints::drawR_()
	{
		if(m_RBuf.is_dirty)
		{
			m_RBuf.u().update(UniformIN_ShaderP_S_OFF, sizeof(m_size_ratio), &m_size_ratio);
			
			auto & s = this->m_scale2;
			float sx = 1 / s.x;
			float sy = 1 / s.y;
			std::vector<VertexData> buf(m_dots.size());
			for(size_t i = 0; i < m_dots.size(); ++i)
			{
				auto dst = m_dots[i];
				auto & src = buf[i];
				src.coord.x = dst.coord.x * sx;
				src.coord.y = dst.coord.y * sy;
				src.color = dst.color;
				src.psize = dst.psize;
			}
			
			m_RBuf.is_dirty = false;
			
			if(m_RBuf.v_count() == m_dots.size())
			{
				m_RBuf.updateVertexBuffer((VertexIN_ShaderP*)(void*)buf.data(), buf.size(), 0);
			}
			else
			{
				m_RBuf.createVertexBuffer((VertexIN_ShaderP*)(void*)buf.data(), buf.size());
			}
		}
		
		/// update uniform {
		m_RBuf.updateSizeRatio(m_size_ratio, m_scale2.x);
		
		m_RBuf.updateMatrix(getMatrix(), getMatrixVersion(), false);
		
		float alpha = getAlpha() * DrawStack::Get()->getTopAlpha();
		m_RBuf.updateColor(getColor(), alpha, m_is_color_dirty);
		m_is_color_dirty = false;
		/// } update uniform
		
		auto r = Metal::Render::Get();
		
		auto blends = MetalHelper::GetBlends(m_blend);
		
		if(m_texture)
		{
			auto pipe = Metal::PipePT::GetShared
			(m_is_grey_mode, m_texture->format(), blends.first, blends.second);
			r->activePipe(pipe->getID());
			r->setVertexBuffer(pipe->VertAttribIndex, m_RBuf.v().getID(), 0);
			r->setVertexBuffer(pipe->UniformIndex, m_RBuf.u().getID(), 0);
			
			r->bindTexture(pipe->TextureIndex, m_texture->buffer().mt_buffer.getID());
			
		}
		else {
			
			auto pipe = Metal::PipeP::GetShared(m_is_grey_mode, blends.first, blends.second);
			r->activePipe(pipe->getID());
			r->setVertexBuffer(pipe->VertAttribIndex, m_RBuf.v().getID(), 0);
			r->setVertexBuffer(pipe->UniformIndex, m_RBuf.u().getID(), 0);
		}
		
		r->drawPrimitives(eVertexMode::Point, 0, m_RBuf.v_count());
	}
#endif
}}


namespace Zen { namespace Vap {
	
	FigureColors::FigureColors(std::string const & name)
	: FigureBase_<VertexDataColors>(name)
	{
		
	}
	void FigureColors::setStrip(bool s) {
		m_is_strip = s;
	}

	bool FigureColors::isStrip()
	{
		return m_is_strip;
	}
	
	void FigureColors::draw()
	{
		if(m_dots.empty()) return;
		
		if(m_is_vertex_dirty) {
			m_is_vertex_dirty = false;
			m_RBuf.is_dirty = true;
		}
		this->updateMatrix();
		
		this->drawR_();
	}
	
#if ZEN_DRAW_GLES
	void FigureColors::initR_()
	{
		
	}
	void FigureColors::drawR_()
	{
		if(m_RBuf.is_dirty)
		{
			auto & s = this->m_scale2;
			float sx = 1 / s.x;
			float sy = 1 / s.y;
			std::vector<VertexData> buf(m_dots.size());
			for(size_t i = 0; i < m_dots.size(); ++i)
			{
				auto dst = m_dots[i];
				auto & src = buf[i];
				src.coord.x = dst.coord.x * sx;
				src.coord.y = dst.coord.y * sy;
				src.color = dst.color;
			}
			
			m_RBuf.is_dirty = false;
			
			auto step = sizeof(VertexData);
			if(m_RBuf.size != buf.size())
			{
				m_RBuf.size = buf.size();
				m_RBuf.v.create(step * buf.size(), buf.data());
			}
			else
			{
				m_RBuf.v.update(0, step * buf.size(), buf.data());
			}
		}
		
		auto alpha = DrawStack::Get()->getTopAlpha() * this->getAlpha();
		
		auto sha = GL::ShaderPrograms::Get()->getShaderP(m_is_grey_mode, ePixel::Grey);
		
		GLHelper::PerformBlend(m_blend);
		
		auto s = GLHelper::GetGLRender();
		
		s->activeProgram(sha->program.getID());
		
		s->enableBuffer(m_RBuf.v.getID());
		
		s->setVertexBuffer(sha->a_coord, 2, GL::eType::Float, false, sizeof(VertexData), 0);
		s->setVertexBuffer(sha->a_color, 4, GL::eType::Float, false, sizeof(VertexData), 2*sizeof(float));
		s->setVertexBuffer(sha->a_size,  1, GL::eType::Float, false, sizeof(VertexData), 6*sizeof(float));
				
		s->setUniform(sha->u_color, m_color.red, m_color.green, m_color.blue, m_color.alpha*alpha);
		s->setUniform(sha->u_transform, getMatrix());
		s->drawArray(GL::eVertexMode::Triangle, 0, m_dots.size());
	}
	
#elif ZEN_DRAW_METAL
	void FigureColors::initR_()
	{
		
	}
	void FigureColors::drawR_()
	{
		if(m_RBuf.is_dirty)
		{
			auto & s = this->m_scale2;
			float sx = 1 / s.x;
			float sy = 1 / s.y;
			std::vector<VertexData> buf(m_dots.size());
			for(size_t i = 0; i < m_dots.size(); ++i)
			{
				auto dst = m_dots[i];
				auto & src = buf[i];
				src.coord.x = dst.coord.x * sx;
				src.coord.y = dst.coord.y * sy;
				src.color = dst.color;
			}
			
			m_RBuf.is_dirty = false;
			
			if(m_RBuf.v_count() == m_dots.size())
			{
				m_RBuf.updateVertexBuffer((VertexIN_ShaderP*)(void*)buf.data(), buf.size(), 0);
			}
			else
			{
				m_RBuf.createVertexBuffer((VertexIN_ShaderP*)(void*)buf.data(), buf.size());
			}
		}
		
		/// update uniform {
		m_RBuf.updateSizeRatio(m_size_ratio, m_scale2.x);
		
		m_RBuf.updateMatrix(getMatrix(), getMatrixVersion(), false);
		
		float alpha = getAlpha() * DrawStack::Get()->getTopAlpha();
		m_RBuf.updateColor(getColor(), alpha, m_is_color_dirty);
		m_is_color_dirty = false;
		/// } update uniform
		
		auto r = Metal::Render::Get();
		
		auto blends = MetalHelper::GetBlends(m_blend);
		
		if(m_texture)
		{
			auto pipe = Metal::PipePT::GetShared
			(m_is_grey_mode, m_texture->format(), blends.first, blends.second);
			r->activePipe(pipe->getID());
			r->setVertexBuffer(pipe->VertAttribIndex, m_RBuf.v().getID(), 0);
			r->setVertexBuffer(pipe->UniformIndex, m_RBuf.u().getID(), 0);
			
			r->bindTexture(pipe->TextureIndex, m_texture->buffer().mt_buffer.getID());
			
		}
		else {
			
			auto pipe = Metal::PipeP::GetShared(m_is_grey_mode, blends.first, blends.second);
			r->activePipe(pipe->getID());
			r->setVertexBuffer(pipe->VertAttribIndex, m_RBuf.v().getID(), 0);
			r->setVertexBuffer(pipe->UniformIndex, m_RBuf.u().getID(), 0);
		}
		
		r->drawPrimitives(eVertexMode::Point, 0, m_RBuf.v_count());
	}
#endif
}}


namespace Zen { namespace Vap {
}}
