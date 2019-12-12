/*
 Copyright (c) 2013 Tianj. Guo

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
	: LNode(name)
	{
		m_color_lerp = &DefaultLerp;
		m_size_lerp = &DefaultLerp;
		m_blend = EBlend::Add;
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
		this->initParticles();
		auto action = new ActionCall(0, m_update_interval, "action update " + m_vap_name);
		auto call = [this](float interval)->bool {
			this->updateParticles(interval);
			return true;
		};
		action->init(call, 0);
		m_update_action = std::shared_ptr<Action>(action);
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

	void Particle::setEffect(std::shared_ptr<Zen::GL::ShaderParticle const> shader)
	{
		m_shader = shader;
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

	void Particle::setColor(Zen::Color4f color)
	{
		m_color = color;
	}
	Zen::Color4f Particle::getColor()
	{
		return m_color;
	}

	void Particle::draw()
	{
		if(!isRunning() || m_buffer.empty()) return;

		using namespace Zen::GL;
		if(m_buffer_dirty)
		{
			m_texture_buffer.updateData(0, m_buffer.size() * sizeof(Dot), m_buffer.data());
			m_buffer_dirty = false;
		}

		this->updateWorldMatrix();

		auto alpha = RenderStack::GetDefault()->getTopAlpha() * this->getAlpha();

		auto texture = m_texture;
		if(texture == nullptr) texture = Textures::GetDefault()->getParticleTexture();

		auto ss = m_shader;
		if(ss == nullptr)
		{
			if(texture->format() == EBPP::Grey) ss = ShaderParticle::GetAlpha();
			else ss = ShaderParticle::GetNormal();
		}

		RenderStack::GetDefault()->performBlend(m_blend);

		Render::BindTexImage2D(texture->image2d().getObject(), 1);
		Render::ActiveProgram(ss->program.getObject());

		Render::BindArrayBuffer(m_texture_buffer.getObject());

		Render::SetVertexAttribBuffer(ss->a_color, 4, EType::Float, true, sizeof(Dot), 0);
		Render::SetVertexAttribBuffer(ss->a_coord, 2, EType::Float, false, sizeof(Dot), 4*sizeof(float));
		Render::SetVertexAttribBuffer(ss->a_size, 1, EType::Float, false, sizeof(Dot), 6*sizeof(float));
		Render::EnableVertexAttrib(ss->a_coord);
		Render::EnableVertexAttrib(ss->a_color);
		Render::EnableVertexAttrib(ss->a_size);
		Render::SetUniformFloat(ss->u_color, m_color.red, m_color.green, m_color.blue, m_color.alpha*alpha);

		Render::SetUniformFloat(ss->u_size, m_point_scale/Root::GetDefault()->getViewScale());
		Render::SetUniformInt(ss->u_sampler, 1);
		Render::SetUniformMat(ss->u_transform, m_world_matrix);
		Render::DrawArray(EDrawMode::Points, 0, m_buffer.size());

		Render::BindArrayBuffer(0);
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
		i.life = Rf(m_emit_life, m_emit_life_var);
	}

	void GravityParticle::_update_a_particle(Dot & b, SrcItem & i)
	{
		float v = i.age / i.life;
		auto & sc = i.start_color;
		auto & ec = i.end_color;
		b.color.red = m_color_lerp(sc.red, ec.red, v);
		b.color.green = m_color_lerp(sc.green, ec.green, v);
		b.color.blue = m_color_lerp(sc.blue, ec.blue, v);
		b.color.alpha = m_color_lerp(sc.alpha, ec.alpha, v) * m_color_lerp(i.start_alpha, i.end_alpha, v);
		b.coord.x = i.emit_position.x + i.emit_speed.x * i.age + i.gravity.x * i.age * i.age * 0.5f;
		b.coord.y = i.emit_position.y + i.emit_speed.y * i.age + i.gravity.y * i.age * i.age * 0.5f;
		b.size = m_size_lerp(i.start_size, i.end_size, v);
	}
	void GravityParticle::initParticles()
	{
		m_buffer.resize(m_total_count);
		m_particles.resize(m_total_count);
		auto b = m_buffer.data();
		for(auto & i : m_particles)
		{
			this->_init_a_particle(i);
			i.age = S_rand.nextf() * i.life;
			this->_update_a_particle(*b, i);
			++b;
		}
		m_texture_buffer.bindData(m_buffer.size() * sizeof(Dot), m_buffer.data());
	}
	void GravityParticle::updateParticles(float interval)
	{
		auto b = m_buffer.data();
		for(auto & i : m_particles)
		{
			i.age += interval;
			if(i.age > i.life)
			{
				this->_init_a_particle(i);
				i.age = 0;
			}
			this->_update_a_particle(*b, i);
			++b;
		}
		m_buffer_dirty = true;
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

	void GalaxyParticle::_init_a_particle(SrcItem &i)
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

		i.start_size = Rf0(m_start_size, m_start_size_var);
		i.end_size = Rf0(m_end_size, m_end_size_var);
		i.life = Rf(m_emit_life, m_emit_life_var);

		i.start_radius = Rf(m_start_radius, m_start_radius_var);
		i.end_radius = Rf(m_end_radius, m_end_radius_var);
		i.emit_av = Rf(m_emit_av, m_emit_av_var);
		i.emit_acc = Rf(m_emit_acc, m_emit_acc_var);
		i.emit_angle = Rf(m_emit_angle, m_emit_angle_var);
	}
	void GalaxyParticle::_update_a_particle(Dot & b, SrcItem &i)
	{
		float v = i.age / i.life;
		auto & sc = i.start_color;
		auto & ec = i.end_color;
		b.color.red = m_color_lerp(sc.red, ec.red, v);
		b.color.green = m_color_lerp(sc.green, ec.green, v);
		b.color.blue = m_color_lerp(sc.blue, ec.blue, v);
		b.color.alpha = m_color_lerp(sc.alpha, ec.alpha, v) * m_color_lerp(i.start_alpha, i.end_alpha, v);
		float r = Zen::Lerp(i.start_radius, i.end_radius, v);
		float rad = i.emit_angle + i.emit_av * i.age + i.emit_acc * i.age * i.age * 0.5f;
		b.coord.x = r * cosf(rad);
		b.coord.y = r * sinf(rad);
		b.size = m_size_lerp(i.start_size, i.end_size, v);
	}
	void GalaxyParticle::initParticles()
	{
		m_buffer.resize(m_total_count);
		m_particles.resize(m_total_count);
		auto b = m_buffer.data();
		for(auto & i : m_particles)
		{
			this->_init_a_particle(i);
			i.age = S_rand.nextf() * i.life;
			this->_update_a_particle(*b, i);
			++b;
		}
		m_texture_buffer.bindData(m_buffer.size() * sizeof(Dot), m_buffer.data());
	}
	void GalaxyParticle::updateParticles(float interval)
	{
		auto b = m_buffer.data();
		for(auto & i : m_particles)
		{
			i.age += interval;
			if(i.age > i.life)
			{
				this->_init_a_particle(i);
				i.age = 0;
			}
			this->_update_a_particle(*b, i);
			++b;
		}
		m_buffer_dirty = true;
	}
}}
