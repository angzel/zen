#pragma once

#include "zen_vap2d_node.h"
#include "zen_vap2d_event.h"
#include "zen_vap2d_texture.h"
#include "zen_rect.h"
#include "zen_gles2_shader_particle.h"
#include "zen_random.h"
#include "zen_numerical.h"
#include <vector>
#include <mutex>
#include <functional>

namespace Zen { namespace Vap2d {

	class Particle : public LNode, public View {
	protected:
		struct Dot {
			Color4f color;
			Point2 coord;
			float size;
		};
	public:
		Particle(std::string const & name = "particle");

		virtual void start();
		virtual void stop();
		virtual bool isRunning();
		/**
		 @function setParticleTotalCount
		 - after <start>. setParticleTotalCount is not effective utill next start.
		 */
		void setParticleTotalCount(size_t count);

		void setParticleStartColors(Zen::Color4f start);
		void setParticleStartColorsVarying(Zen::Color4f start_var);
		void setParticleEndColors(Zen::Color4f end);
		void setParticleEndColorsVarying(Zen::Color4f end_var);
		void setParticleStartSize(float start);
		void setParticleStartSizeVarying(float start_var);
		void setParticleEndSize(float end);
		void setParticleEndSizeVarying(float end_var);
		void setParticleStartAlpah(float start);
		void setParticleStartAlpahVarying(float start_var);
		void setParticleEndAlpah(float end);
		void setParticleEndAlpahVarying(float end_var);
		void setParticleLife(float life);
		void setParticleLifeVarying(float life_var);

		void setUpdateInterval(float interval);

		void setParticleScale(float scale);

		void setColorLerpFunction(std::function<float(float s, float e, float v)> lerp);
		void setSizeLerpFunction(std::function<float(float s, float e, float v)> lerp);

	public: // texture
		/**
		 
		 */
		void setTexture(SharedTexture texture);

		SharedTexture getTexture();

		void setColor(Zen::Color4f color);

		Zen::Color4f getColor();

		void setEffect(std::shared_ptr<Zen::GL::ShaderParticle const>);

		virtual void draw() override;

	protected:
		virtual void initParticles() = 0;
		virtual void updateParticles(float) = 0;

		std::shared_ptr<Action> m_update_action = nullptr;

		Zen::Color4f m_start_color = {1, 1, 1, 0};
		Zen::Color4f m_start_color_var = {0, 0, 0, 0};
		Zen::Color4f m_end_color = {1, 1, 1, 1};
		Zen::Color4f m_end_color_var = {0, 0, 0, 0};
		float m_start_size = 10;
		float m_end_size = 20;
		float m_start_size_var = 5;
		float m_end_size_var = 5;
		float m_start_alpha = 1;
		float m_start_alpha_var = 0;
		float m_end_alpha = 0;
		float m_end_alpha_var = 0;
		float m_emit_life = 2;
		float m_emit_life_var = 1;

		float m_update_interval = 0;

		size_t m_total_count = 128;

		std::function<float(float s, float e, float v)> m_color_lerp;
		std::function<float(float s, float e, float v)> m_size_lerp;

		std::shared_ptr<Zen::GL::ShaderParticle const> m_shader = nullptr;
		SharedTexture m_texture;
		Zen::Color4f m_color = Zen::Color4f(0xff8822ff);
		float m_point_scale = 1.f;
		
		Zen::GL::ArrayBuffer m_texture_buffer;
		std::vector<Dot> m_buffer;
		bool m_buffer_dirty = false;
	};

	class GravityParticle : public Particle {
	protected:
		struct SrcItem
		{
			float life;
			float age;
			Zen::Color4f start_color;
			Zen::Color4f end_color;
			float start_size;
			float end_size;
			float start_alpha;
			float end_alpha;
			Zen::Point2 emit_speed;
			Zen::Point2 gravity;
			Zen::Point2 emit_position;
		};
	public: // gravity
		GravityParticle(std::string const & name = "gravity particle");

		void setParticleEmitDirect(float randians);
		void setParticleEmitDirectVarying(float var);
		void setParticleEmitSpeed(float speed);
		void setParticleEmitSpeedVarying(float speed_var);
		void setParticleEmitPosition(Point2 pos);
		void setParticleEmitPositionVarying(Point2 pos_var);
		void setParticleGravityDirect(float r);
		void setParticleGravityDirectVarying(float r_var);
		void setParticleGravityAccelerate(float acc);
		void setParticleGravityAccelerateVarying(float acc_var);

	protected:
		virtual void initParticles() override;
		virtual void updateParticles(float) override;

		void _init_a_particle(SrcItem & i);
		void _update_a_particle(Dot & b, SrcItem & i);

		float m_emit_radians = ZEN_F_PI2;
		float m_emit_radians_var = 0;
		float m_emit_speed = 10;
		float m_emit_speed_var = 0;

		float m_gravity_radians = ZEN_F_PI2;
		float m_gravity_radians_var = 0.f;
		float m_gravity_acc = 10;
		float m_gravity_acc_var = 0;
		Zen::Point2 m_emit_pos = {0, 0};
		Zen::Point2 m_emit_pos_var = {0, 0};

		std::vector<SrcItem> m_particles;
	};
}}

namespace Zen { namespace Vap2d {

	class GalaxyParticle : public Particle
	{
	protected:
		struct SrcItem
		{
			float life;
			float age;
			Zen::Color4f start_color;
			Zen::Color4f end_color;
			float start_size;
			float end_size;
			float start_alpha;
			float end_alpha;
			float emit_av; // radians
			float emit_acc; // radians
			float emit_angle;
			float start_radius;
			float end_radius;
		};
	public:
		GalaxyParticle(std::string const & name = "gravity particle");

		void setParticleEmitAngle(float rad);
		void setParticleEmitAngularVelocity(float av);
		void setParticleStartRadius(float r);
		void setParticleEndRadius(float r);
		void setParticleAngularAccelerate(float acc);

		void setParticleEmitAngleVar(float rad_var);
		void setParticleEmitAngularVelocityVar(float av_var);
		void setParticleStartRadiusVar(float r_var);
		void setParticleEndRadiusVar(float r_var);
		void setParticleAngularAccelerateVar(float acc_var);

		void setRadiusLerpFunction(std::function<float(float s, float e, float v)> lerp);
	protected:
		virtual void initParticles() override;
		virtual void updateParticles(float) override;

		void _init_a_particle(SrcItem & i);
		void _update_a_particle(Dot & b, SrcItem & i);
		
		float m_start_radius = 0;
		float m_start_radius_var = 0;
		float m_end_radius = 50;
		float m_end_radius_var = 0;
		float m_emit_angle = 0;
		float m_emit_angle_var = ZEN_F_PI;
		float m_emit_av = ZEN_F_PI/4;
		float m_emit_av_var = 0;
		float m_emit_acc = ZEN_F_PI/8;
		float m_emit_acc_var = ZEN_F_PI/20;

		std::vector<SrcItem> m_particles;

		std::function<float(float s, float e, float v)> m_radius_lerp;
	};
}}
