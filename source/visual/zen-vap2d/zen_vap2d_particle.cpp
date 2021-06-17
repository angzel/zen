/*
 Copyright (c) 2013 Tianj. Guo


 */

#include "zen_vap2d_particle.h"
#include "zen_vap2d_root.h"

namespace Zen { namespace Vap2d {

	static Zen::Random S_rand;

	static float DefaultLerp(float s, float e, float v)
	{
		return s + (e - s) * v;
	}
	static float Rf0(float center, float radius)
	{
		float r = center + (cosf(S_rand.nextf() * ZEN_F_PI)) * radius;
		return r<0?0.f:r;
	}
	static float Rf01(float center, float radius)
	{
		float r = center + (cosf(S_rand.nextf() * ZEN_F_PI)) * radius;
		return r<0?0.f:(r>1.f?1.f:r);
	}
	static float Rf(float center, float radius)
	{
		float r = center + (cosf(S_rand.nextf() * ZEN_F_PI)) * radius;
		return r;
	}
	Particle::Particle(std::string const & name)
	: FinalNode(name)
	{
		m_color_lerp = &DefaultLerp;
		m_size_lerp = &DefaultLerp;
		m_blend = eBlend::Add;
		m_color = Zen::Color4f(0xff8822ff);
		_initParticle();
	}

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
	void Particle::setColorLerpFunction(std::function<float(float s, float e, float v)> lerp)
	{
		if(lerp) m_color_lerp = lerp;
		else m_color_lerp = &DefaultLerp;
	}
	void Particle::setSizeLerpFunction(std::function<float(float s, float e, float v)> lerp)
	{
		if(lerp) m_size_lerp = lerp;
		else m_size_lerp = &DefaultLerp;
	}
	void Particle::start()
	{
		if(m_update_action)
		{
			this->stopAction(m_update_action);
		}
		this->initDots();

		auto call = [this](float interval)->bool {
			this->updateDots(interval);
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

	void Particle::setTexture(SharedTexture texture)
	{
		m_texture = texture;
	}
	void Particle::setParticleScale(float scale)
	{
		m_point_scale = scale;
	}

	SharedTexture Particle::getTexture()
	{
		return m_texture;
	}

	void Particle::setDotsDirty()
	{
		m_is_buffer_dirty = true;
	}
	void Particle::draw()
	{
		if(!isRunning() || !m_dots.size()) return;

		if(!m_texture) m_texture = Textures::Get()->getParticleTexture();

		this->_drawParticle();
	}
}}

namespace Zen { namespace Vap2d {

	GravityParticle::GravityParticle(std::string const & name)
	: Particle(name)
	{
	}

	void GravityParticle::setParticleEmitDirect(float randians)
	{
		m_gravity_radians = randians;
	}
	void GravityParticle::setParticleEmitDirectVarying(float var)
	{
		m_gravity_radians_var = var;
	}
	void GravityParticle::setParticleGravityAccelerate(float acc)
	{
		m_gravity_acc = acc;
	}
	void GravityParticle::setParticleGravityAccelerateVarying(float acc_var)
	{
		m_gravity_acc_var = acc_var;
	}

	void GravityParticle::setParticleGravityDirect(float r)
	{
		m_gravity_radians = r;
	}
	void GravityParticle::setParticleGravityDirectVarying(float r_var)
	{
		m_gravity_radians_var = r_var;
	}
	void GravityParticle::setParticleEmitSpeed(float speed)
	{
		m_emit_speed = speed;
	}
	void GravityParticle::setParticleEmitSpeedVarying(float speed_var)
	{
		m_emit_speed_var = speed_var;
	}
	void GravityParticle::setParticleEmitPosition(Point2 pos)
	{
		m_emit_pos = pos;
	}
	void GravityParticle::setParticleEmitPositionVarying(Point2 pos_var)
	{
		m_emit_pos_var = pos_var;
	}
	void GravityParticle::_init_a_particle(SrcItem & i)
	{
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

		i.start_size = Rf0(m_start_size, m_start_size_var) * m_point_scale / Root::Get()->getViewScale();
		i.end_size = Rf0(m_end_size, m_end_size_var) * m_point_scale / Root::Get()->getViewScale();
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
		i.life = Rf(m_emit_life, m_emit_life_var);
	}

	void GravityParticle::_update_a_particle(size_t i)
	{
		auto & dot = m_dots[i];
		auto & s = m_particles[i];
		float v = s.age / s.life;
		auto & sc = s.start_color;
		auto & ec = s.end_color;
		dot.red = m_color_lerp(sc.red, ec.red, v);
		dot.green = m_color_lerp(sc.green, ec.green, v);
		dot.blue = m_color_lerp(sc.blue, ec.blue, v);
		dot.alpha = m_color_lerp(sc.alpha, ec.alpha, v) * m_color_lerp(s.start_alpha, s.end_alpha, v);
		dot.x = s.emit_position.x + s.emit_speed.x * s.age + s.gravity.x * s.age * s.age * 0.5f;
		dot.y = s.emit_position.y + s.emit_speed.y * s.age + s.gravity.y * s.age * s.age * 0.5f;
		dot.size = m_size_lerp(s.start_size, s.end_size, v);
	}
	void GravityParticle::initDots()
	{
		m_dots.resize(m_total_count);
		m_particles.resize(m_total_count);
		for(size_t i = 0; i < m_total_count; ++i)
		{
			auto & s = m_particles[i];
			this->_init_a_particle(s);
			s.age = S_rand.nextf() * s.life;
			this->_update_a_particle(i);
		}
		setDotsDirty();
	}
	void GravityParticle::updateDots(float interval)
	{
		for(size_t i = 0; i < m_dots.size(); ++i)
		{
			auto & s = m_particles[i];
			s.age += interval;
			if(s.age > s.life)
			{
				this->_init_a_particle(s);
				s.age = 0;
			}
			this->_update_a_particle(i);
		}
		setDotsDirty();
	}
}}

namespace Zen { namespace Vap2d {
	GalaxyParticle::GalaxyParticle(std::string const & name)
	: Particle(name)
	{
		m_radius_lerp = &DefaultLerp;
	}

	void GalaxyParticle::setParticleEmitAngle(float rad)
	{
		m_emit_angle = rad;
	}
	void GalaxyParticle::setParticleEmitAngularVelocity(float av)
	{
		m_emit_av = av;
	}

	void GalaxyParticle::setParticleStartRadius(float r)
	{
		m_start_radius = r;
	}

	void GalaxyParticle::setParticleEndRadius(float r)
	{
		m_end_radius = r;
	}

	void GalaxyParticle::setParticleAngularAccelerate(float acc)
	{
		m_emit_acc = acc;
	}


	void GalaxyParticle::setParticleEmitAngleVar(float rad_var)
	{
		m_emit_angle_var = rad_var;
	}

	void GalaxyParticle::setParticleEmitAngularVelocityVar(float av_var)
	{
		m_emit_av_var = av_var;
	}

	void GalaxyParticle::setParticleStartRadiusVar(float r_var)
	{
		m_start_radius_var = r_var;
	}

	void GalaxyParticle::setParticleEndRadiusVar(float r_var)
	{
		m_end_radius_var = r_var;
	}

	void GalaxyParticle::setParticleAngularAccelerateVar(float acc_var)
	{
		m_emit_acc_var = acc_var;
	}

	void GalaxyParticle::setRadiusLerpFunction(std::function<float (float, float, float)> lerp)
	{
		if(!lerp) m_radius_lerp = &DefaultLerp;
		else m_radius_lerp = lerp;
	}

	void GalaxyParticle::_init_a_particle(SrcItem & i)
	{
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

		i.start_size = Rf0(m_start_size, m_start_size_var) * m_point_scale / Root::Get()->getViewScale();
		i.end_size = Rf0(m_end_size, m_end_size_var) * m_point_scale / Root::Get()->getViewScale();
		i.life = Rf(m_emit_life, m_emit_life_var);

		i.start_radius = Rf(m_start_radius, m_start_radius_var);
		i.end_radius = Rf(m_end_radius, m_end_radius_var);
		i.emit_av = Rf(m_emit_av, m_emit_av_var);
		i.emit_acc = Rf(m_emit_acc, m_emit_acc_var);
		i.emit_angle = Rf(m_emit_angle, m_emit_angle_var);
	}
	void GalaxyParticle::_update_a_particle(size_t i)
	{
		auto & dot = m_dots[i];
		auto & s = m_particles[i];
		float v = s.age / s.life;
		auto & sc = s.start_color;
		auto & ec = s.end_color;
		dot.red = m_color_lerp(sc.red, ec.red, v);
		dot.green = m_color_lerp(sc.green, ec.green, v);
		dot.blue = m_color_lerp(sc.blue, ec.blue, v);
		dot.alpha = m_color_lerp(sc.alpha, ec.alpha, v) * m_color_lerp(s.start_alpha, s.end_alpha, v);
		float r = Zen::Lerp(s.start_radius, s.end_radius, v);
		float rad = s.emit_angle + s.emit_av * s.age + s.emit_acc * s.age * s.age * 0.5f;
		dot.x = r * cosf(rad);
		dot.y = r * sinf(rad);
		dot.size = m_size_lerp(s.start_size, s.end_size, v);
	}
	void GalaxyParticle::initDots()
	{
		m_dots.resize(m_total_count);
		m_particles.resize(m_total_count);

		for(size_t i = 0; i < m_dots.size(); ++i)
		{
			auto & s = m_particles[i];
			this->_init_a_particle(s);
			s.age = S_rand.nextf() * s.life;
			this->_update_a_particle(i);
		}
		Particle::setDotsDirty();
	}
	void GalaxyParticle::updateDots(float interval)
	{
		for(size_t i = 0; i < m_dots.size(); ++i)
		{
			auto & s = m_particles[i];
			s.age += interval;
			if(s.age > s.life)
			{
				this->_init_a_particle(s);
				s.age = 0;
			}
			this->_update_a_particle(i);
		}
		Particle::setDotsDirty();
	}
}}
