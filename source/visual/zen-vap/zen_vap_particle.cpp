/*
 Copyright (c) 2013 Tianj. Guo
 
 
 */

#include "zen_vap_particle.h"
#include "zen_vap_root.h"
#include "zen_vap_render.h"
#include "zen_exception.h"

namespace Zen { namespace Vap {
	
	static Zen::Random s_rand;
	
	static inline float sAccMove(float p, float speed, float acc, float time)
	{
		return p + speed * time + acc * time * time * 0.5f;
	}
	static float DefaultLerp(float s, float e, float v)
	{
		return s + (e - s) * v;
	}
	static Color DefaultColorLerp(Color s, Color e, float v)
	{
		return Color4fLerp(s, e, v);
	}
	/// @function Rf0 Rf01 Rf
	/// @brief return a number in the range [center-var, center+var].
	static float Rf0(float center, float var)
	{
		float r = center + (cosf(s_rand.nextf() * ZEN_F_PI)) * var;
		return r<0?0.f:r;
	}
	static float Rf01(float center, float var)
	{
		float r = center + (cosf(s_rand.nextf() * ZEN_F_PI)) * var;
		return r<0?0.f:(r>1.f?1.f:r);
	}
	static float Rf(float center, float var)
	{
		float r = center + (cosf(s_rand.nextf() * ZEN_F_PI)) * var;
		return r;
	}
}}

namespace Zen { namespace Vap {
	
	void Particle::setParticleTotalCount(size_t count)
	{
		m_total_count = count;
	}
	void Particle::setParticleStartColors(Zen::Color4f start)
	{
		m_start_color = start;
	}
	void Particle::setParticleStartColorsVarying(Zen::Color4f start_var)
	{
		m_start_color_var = start_var;
	}
	void Particle::setParticleEndColors(Zen::Color4f end)
	{
		m_end_color = end;
	}
	void Particle::setParticleEndColorsVarying(Zen::Color4f end_var)
	{
		m_end_color_var = end_var;
	}
	void Particle::setParticleStartSize(float start)
	{
		m_start_size = start;
	}
	void Particle::setParticleStartSizeVarying(float start_var)
	{
		m_start_size_var = start_var;
	}
	void Particle::setParticleEndSize(float end)
	{
		m_end_size = end;
	}
	void Particle::setParticleEndSizeVarying(float end_var)
	{
		m_end_size_var = end_var;
	}
	void Particle::setParticleLife(float life)
	{
		m_emit_life = life;
	}
	void Particle::setParticleLifeVarying(float life_var)
	{
		m_emit_life_var = life_var;
	}
	
	void Particle::setParticleStartAlpah(float start)
	{
		m_start_alpha = start;
	}
	void Particle::setParticleStartAlpahVarying(float start_var)
	{
		m_start_alpha_var = start_var;
	}
	void Particle::setParticleEndAlpah(float end)
	{
		m_end_alpha = end;
	}
	void Particle::setParticleEndAlpahVarying(float end_var)
	{
		m_end_alpha_var = end_var;
	}
	void Particle::setUpdateInterval(float interval)
	{
		m_update_interval = interval;
	}
	
	void Particle::setColorLerpFunction(std::function<Color(Color s, Color e, float v)> lerp)
	{
		if(lerp) m_color_lerp = lerp;
		else m_color_lerp = &DefaultColorLerp;
	}
	
	void Particle::setAlphaLerpFunction(std::function<float(float s, float e, float v)> lerp)
	{
		if(lerp) m_alpha_lerp = lerp;
		else m_alpha_lerp = &DefaultLerp;
	}
	void Particle::setSizeLerpFunction(std::function<float(float s, float e, float v)> lerp)
	{
		if(lerp) m_size_lerp = lerp;
		else m_size_lerp = &DefaultLerp;
	}
	
	void  Particle::setEmitterLife(double sec)
	{
		m_emittor_life = sec;
	}
	
	double Particle::getEmitterLife()
	{
		return m_emittor_life;
	}
	void Particle::start()
	{
		if(m_update_action)
		{
			this->stopAction(m_update_action);
		}
		this->resetParticles();
		
		m_running_time = 0;
		
		auto call = [this](float interval)->bool {
			m_running_time += interval;
			this->updateParticles(interval);
			return true;
		};
		m_update_action = ActionCall::Create(call, 0, 0, m_update_interval);
		this->runAction(m_update_action);
	}
	void Particle::stop()
	{
		if(m_update_action != nullptr)
		{
			this->stopAction(m_update_action);
			m_update_action = nullptr;
		}
	}
	bool Particle::isRunning()
	{
		return m_update_action != nullptr;
	}
	
	void Particle::setTexture(TextureShared texture)
	{
		m_texture = texture;
	}
	
	TextureShared Particle::getTexture()
	{
		return m_texture;
	}
}}

namespace Zen { namespace Vap {
	
	ParticleGravity::ParticleGravity(std::string const & name)
	: Particle(name)
	{
	}
	
	void ParticleGravity::setParticleEmitDirect(float randians)
	{
		m_emit_radians_var = randians;
	}
	void ParticleGravity::setParticleEmitDirectVarying(float var)
	{
		m_emit_radians_var = var;
	}
	void ParticleGravity::setParticleGravityValue(float acc)
	{
		m_gravity_acc = acc;
	}
	void ParticleGravity::setParticleGravityAccVarying(float acc_var)
	{
		m_gravity_acc_var = acc_var;
	}
	
	void ParticleGravity::setParticleGravityDirect(float r)
	{
		m_gravity_radians = r;
	}
	void ParticleGravity::setParticleGravityDirectVarying(float r_var)
	{
		m_gravity_radians_var = r_var;
	}
	void ParticleGravity::setParticleEmitSpeed(float speed)
	{
		m_emit_speed = speed;
	}
	void ParticleGravity::setParticleEmitSpeedVarying(float speed_var)
	{
		m_emit_speed_var = speed_var;
	}
	void ParticleGravity::setParticleEmitPosition(Point2 pos)
	{
		m_emit_pos = pos;
	}
	void ParticleGravity::setParticleEmitPositionVarying(Point2 pos_var)
	{
		m_emit_pos_var = pos_var;
	}
}}

namespace Zen { namespace Vap {
	ParticleGalaxy::ParticleGalaxy(std::string const & name)
	: Particle(name)
	{
		m_radius_lerp = &DefaultLerp;
	}
	
	void ParticleGalaxy::setParticleEmitAngle(float rad)
	{
		m_emit_angle = rad;
	}
	void ParticleGalaxy::setParticleEmitAngularSpeed(float av)
	{
		m_emit_av = av;
	}
	
	void ParticleGalaxy::setParticleStartRadius(float r)
	{
		m_start_radius = r;
	}
	
	void ParticleGalaxy::setParticleEndRadius(float r)
	{
		m_end_radius = r;
	}
	
	void ParticleGalaxy::setParticleAngularAcc(float acc)
	{
		m_emit_acc = acc;
	}
	
	
	void ParticleGalaxy::setParticleEmitAngleVar(float rad_var)
	{
		m_emit_angle_var = rad_var;
	}
	
	void ParticleGalaxy::setParticleEmitAngularSpeedVar(float av_var)
	{
		m_emit_av_var = av_var;
	}
	
	void ParticleGalaxy::setParticleStartRadiusVar(float r_var)
	{
		m_start_radius_var = r_var;
	}
	
	void ParticleGalaxy::setParticleEndRadiusVar(float r_var)
	{
		m_end_radius_var = r_var;
	}
	
	void ParticleGalaxy::setParticleAngularAccVar(float acc_var)
	{
		m_emit_acc_var = acc_var;
	}
	
	void ParticleGalaxy::setRadiusLerpFunction(std::function<float (float, float, float)> lerp)
	{
		if(!lerp) m_radius_lerp = &DefaultLerp;
		else m_radius_lerp = lerp;
	}
}}
namespace Zen { namespace Vap {
	
	bool Particle::updateParticles(float interval)
	{
		bool reuse = (m_emittor_life < 0) || (m_emittor_life > m_running_time);
		
		size_t dead = 0;
		for(size_t i = 0; i < m_dots.size(); ++i)
		{
			if(this->updateSingleParticle(i, interval)) continue;
			// else
			if(reuse) this->initSingleParticle(i);
			else ++dead;
		}
		/**
		 if all dead stop.
		 */
		return (dead != m_dots.size());
	}
	
	void Particle::resetParticles()
	{
		m_dots.resize(m_total_count);
		for(size_t i = 0; i < m_total_count; ++i)
		{
			this->initSingleParticle(i);
		}
	}
	
	void Particle::initSingleParticle(size_t index)
	{
		auto & i = m_dots[index];
		i.color.red = Rf01(m_start_color.red, m_start_color_var.red);
		i.color.green = Rf01(m_start_color.green, m_start_color_var.green);
		i.color.blue = Rf01(m_start_color.blue, m_start_color_var.blue);
		i.color.alpha = Rf01(m_start_color.alpha, m_start_color_var.alpha) *
		Rf01(m_start_alpha, m_start_alpha_var);
		
		i.psize = Rf01(m_start_size, m_start_size_var);
		i.coord.x = s_rand.nextf();
		i.coord.y = s_rand.nextf();
		m_RBuf.is_dirty = true;
	}
	bool Particle::updateSingleParticle(size_t index, float)
	{
		return (m_emittor_life < 0 || m_emittor_life > m_running_time);
	}
	
	void ParticleGravity::resetParticles()
	{
		m_particles.resize(m_total_count);
		Particle::resetParticles();
	}
	
	bool ParticleGravity::updateParticles(float interval)
	{
		return Particle::updateParticles(interval);
	}
	void ParticleGravity::initSingleParticle(size_t index)
	{
		auto & i = m_particles[index];
		i.start_color.red = Rf01(m_start_color.red, m_start_color_var.red);
		i.start_color.green = Rf01(m_start_color.green, m_start_color_var.green);
		i.start_color.blue = Rf01(m_start_color.blue, m_start_color_var.blue);
		i.start_color.alpha = Rf01(m_start_color.alpha, m_start_color_var.alpha);
		
		i.end_color.red = Rf01(m_end_color.red, m_end_color_var.red);
		i.end_color.green = Rf01(m_end_color.green, m_end_color_var.green);
		i.end_color.blue = Rf01(m_end_color.blue, m_end_color_var.blue);
		i.end_color.alpha = Rf01(m_end_color.alpha, m_end_color_var.alpha);
		
		i.start_alpha = Rf01(m_start_alpha, m_start_alpha_var);
		i.end_alpha = Rf01(m_end_alpha, m_end_alpha_var);
		
		i.start_size = Rf0(m_start_size, m_start_size_var);
		i.end_size = Rf0(m_end_size, m_end_size_var);
		float r = Rf(m_emit_radians, m_emit_radians_var);
		float s = Rf(m_emit_speed, m_emit_speed_var);
		i.emit_speed.x = cosf(r) * s;
		i.emit_speed.y = sinf(r) * s;
		i.emit_position.x = Rf(m_emit_pos.x, m_emit_pos_var.x);
		i.emit_position.y = Rf(m_emit_pos.y, m_emit_pos_var.y);
		float gr = Rf(m_gravity_radians, m_gravity_radians_var);
		float ga = Rf(m_gravity_acc, m_gravity_acc_var);
		i.gravity.x = cosf(gr) * ga;
		i.gravity.y = sinf(gr) * ga;
		i.life = Rf0(m_emit_life, m_emit_life_var);
		
		i.age = s_rand.nextf() * i.life * 0.8f;
		
		this->updateSingleParticle(index, 0);
	}
	
	bool ParticleGravity::updateSingleParticle(size_t index, float interval)
	{
		auto & s = m_particles[index];
		
		if(s.age > s.life) return false;
		
		s.age += interval;
		float v = (s.age > s.life ? 1.f : s.age / s.life);
		
		auto & dot = m_dots[index];
		dot.coord.x = sAccMove(s.emit_position.x, s.emit_speed.x, s.gravity.x, s.age) / m_scale2.x;
		dot.coord.y = sAccMove(s.emit_position.y, s.emit_speed.y, s.gravity.y, s.age) / m_scale2.y;
		
		// --- set alpha color.size()
		auto c = m_color_lerp(s.start_color, s.end_color, v);
		dot.color.red = c.rf();
		dot.color.green = c.gf();
		dot.color.blue = c.bf();
		dot.color.alpha = c.af() * m_alpha_lerp(s.start_alpha, s.end_alpha, v);
		
		if(s.age > s.life) dot.psize = 0;
		else dot.psize = m_size_lerp(s.start_size, s.end_size, v);
		
		m_RBuf.is_dirty = true;
		return true;
	}
	
	void ParticleGalaxy::initSingleParticle(size_t index)
	{
		auto & info = m_particles[index];
		info.start_color.red = Rf01(m_start_color.red, m_start_color_var.red);
		info.start_color.green = Rf01(m_start_color.green, m_start_color_var.green);
		info.start_color.blue = Rf01(m_start_color.blue, m_start_color_var.blue);
		info.start_color.alpha = Rf01(m_start_color.alpha, m_start_color_var.alpha);
		
		info.end_color.red = Rf01(m_end_color.red, m_end_color_var.red);
		info.end_color.green = Rf01(m_end_color.green, m_end_color_var.green);
		info.end_color.blue = Rf01(m_end_color.blue, m_end_color_var.blue);
		info.end_color.alpha = Rf01(m_end_color.alpha, m_end_color_var.alpha);
		
		info.start_alpha = Rf01(m_start_alpha, m_start_alpha_var);
		info.end_alpha = Rf01(m_end_alpha, m_end_alpha_var);
		
		info.start_size = Rf0(m_start_size, m_start_size_var); //x; // / Root::Get()->getViewScale();
		info.end_size = Rf0(m_end_size, m_end_size_var); // / m_scale2.y; // / Root::Get()->getViewScale();
		info.life = Rf(m_emit_life, m_emit_life_var);
		
		info.start_radius = Rf(m_start_radius, m_start_radius_var);
		info.end_radius = Rf(m_end_radius, m_end_radius_var);
		info.emit_av = Rf(m_emit_av, m_emit_av_var);
		info.emit_acc = Rf(m_emit_acc, m_emit_acc_var);
		info.emit_angle = Rf(m_emit_angle, m_emit_angle_var);
		
		info.age = s_rand.nextf() * info.life * 0.8f;
		
		this->updateSingleParticle(index, 0);
	}
	
	bool ParticleGalaxy::updateSingleParticle(size_t index, float interval)
	{
		auto & s = m_particles[index];
		
		if(s.age > s.life) return false;
		
		s.age += interval;
		float v = (s.age > s.life ? 1.f : s.age / s.life);
		
		auto & dot = m_dots[index];
		
		float r = Zen::Lerp(s.start_radius, s.end_radius, v);
		float rad = s.emit_angle + s.emit_av * s.age + s.emit_acc * s.age * s.age * 0.5f;
		dot.coord.x = r * cosf(rad) / m_scale2.x;
		dot.coord.y = r * sinf(rad) / m_scale2.y;
		
		// --- set alpha color.size()
		auto c = m_color_lerp(s.start_color, s.end_color, v);
		dot.color.red = c.rf();
		dot.color.green = c.gf();
		dot.color.blue = c.bf();
		dot.color.alpha = c.af() * m_alpha_lerp(s.start_alpha, s.end_alpha, v);
		
		if(s.age > s.life) dot.psize = 0;
		else dot.psize = m_size_lerp(s.start_size, s.end_size, v);
		
		m_RBuf.is_dirty = true;
		
		return true;
	}
	void ParticleGalaxy::resetParticles()
	{
		m_particles.resize(m_total_count);
		Particle::resetParticles();
	}
	bool ParticleGalaxy::updateParticles(float interval)
	{
		return Particle::updateParticles(interval);
	}
}}

namespace Zen { namespace Vap {
	
	Particle::Particle(std::string const & name)
	: Node2d(name), View2d(false)
	{
		m_alpha_lerp = &DefaultLerp;
		m_color_lerp = &DefaultColorLerp;
		m_size_lerp = &DefaultLerp;
		m_blend = eBlend::Add;
		m_color = Zen::Color4f(0xff8822ff);
		this->updateSize_();
		initR_();
	}
	void Particle::updateSize_()
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
	}
	
	void Particle::draw()
	{
		if(!isRunning() || !m_dots.size()) return;
		
		this->updateMatrix();
		
		this->updateSize_();
		
		this->drawR_();
	}
#if ZEN_DRAW_GLES
	void Particle::initR_()
	{
	}
	void Particle::drawR_()
	{
		if(m_RBuf.is_dirty)
		{
			m_RBuf.is_dirty = false;
			
			static const size_t step = sizeof(VertexData);
			
			if(m_RBuf.size != m_dots.size())
			{
				m_RBuf.size = m_dots.size();
				m_RBuf.v.create(step * m_dots.size(), m_dots.data());
			}
			else
			{
				m_RBuf.v.update(0, step * m_dots.size(), m_dots.data());
			}
		}
		
		auto alpha = DrawStack::Get()->getTopAlpha() * this->getAlpha();
				
		auto sha = GL::ShaderPrograms::Get()->getShaderP
		(m_is_grey_mode, m_texture? m_texture->format() : ePixel::Grey);
		
		GLHelper::PerformBlend(m_blend);
		
		auto s = GLHelper::GetGLRender();
		
		s->activeProgram(sha->program.getID());
		
		s->enableBuffer(m_RBuf.v.getID());
		
		s->setVertexBuffer(sha->a_color, 4, GL::eType::Float, false, sizeof(VertexData), 2*sizeof(float));
		s->setVertexBuffer(sha->a_coord, 2, GL::eType::Float, false, sizeof(VertexData), 0);
		s->setVertexBuffer(sha->a_size,  1, GL::eType::Float, false, sizeof(VertexData), 6*sizeof(float));
		s->setUniform(sha->u_size_ratio, m_size_ratio);
		
		s->setUniform(sha->u_color, m_color.red, m_color.green, m_color.blue, m_color.alpha*alpha);
		s->setUniform(sha->u_transform, getMatrix());

		s->setSampler(sha->u_sampler, 1, m_texture? m_texture->buffer().gl_buffer.getID() : 0);

		s->drawArray(GL::eVertexMode::Point, 0, m_dots.size());
		
	}
#elif ZEN_DRAW_METAL
	
	void Particle::initR_()
	{
	}
	
	void Particle::drawR_()
	{
		if(m_RBuf.is_dirty || m_dots.size() != m_RBuf.v_count())
		{
			m_RBuf.is_dirty = false;
			
			must(sizeof(VertexData) == sizeof(VertexIN_ShaderP));
			
			if(m_RBuf.v_count() == m_dots.size())
			{
				m_RBuf.updateVertexBuffer((VertexIN_ShaderP*)(void*)m_dots.data(), m_dots.size(), 0);
			}
			else
			{
				m_RBuf.createVertexBuffer((VertexIN_ShaderP*)(void*)m_dots.data(), m_dots.size());
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
		
		r->drawPrimitives(Metal::eVertexMode::Point, 0, m_RBuf.v_count());
	}
	
#endif
}}
