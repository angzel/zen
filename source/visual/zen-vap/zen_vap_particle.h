#pragma once

#include "zen_vap_render.h"
#include "zen_vap_node.h"
#include "zen_vap_action.h"
#include "zen_vap_texture.h"
#include "zen_types.h"
#include "zen_random.h"
#include "zen_numerical.h"
#include <vector>
#include <mutex>
#include <functional>

namespace Zen { namespace Vap {

	class Particle : public Node2d, public View2d, public Colorful {
	public:
		typedef VertexDataPoints VertexData;

	public:
		Particle(std::string const & name = "particle");

		virtual void start();
		virtual void stop();
		virtual bool isRunning();
		
		void setUpdateInterval(float interval);
		
		/**
		 if < 0, unlimited life.
		 */
		void setEmitterLife(double sec);
		
		double getEmitterLife();
		/**
		 @function setParticleTotalCount
		 - effect on next ::start().
		 */
		void setParticleTotalCount(size_t count);
		
	protected:
		virtual void resetParticles();
		virtual bool updateParticles(float);
		virtual void initSingleParticle(size_t index);
		virtual bool updateSingleParticle(size_t index, float);

	public: /// set particle attributes.
		
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

		void setColorLerpFunction(std::function<Color(Color s, Color e, float v)>);
		
		void setSizeLerpFunction(std::function<float(float s, float e, float v)>);
		
		void setAlphaLerpFunction(std::function<float(float s, float e, float v)>);
		
	public: // texture

		void setTexture(TextureShared texture);

		TextureShared getTexture();

		virtual void draw() override;

	protected:
		std::shared_ptr<Action> m_update_action = nullptr;

		std::vector<VertexData> m_dots;
		size_t m_total_count = 128;
		
		double m_emittor_life = -1;
		double m_running_time = 0;
		float m_update_interval = 0;
	public:

		/// particle attributes {
		Zen::Color4f m_start_color = { 1, 1, 1, 0 };
		Zen::Color4f m_start_color_var = { 0, 0, 0, 0 };
		Zen::Color4f m_end_color = { 1, 1, 1, 1 };
		Zen::Color4f m_end_color_var = { 0, 0, 0, 0 };
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
		std::function<Color(Color s, Color e, float v)> m_color_lerp;
		std::function<float(float s, float e, float v)> m_size_lerp;
		std::function<float(float s, float e, float v)> m_alpha_lerp;
		/// }
		
		TextureShared m_texture;
		
	protected:
		RBufferPoints m_RBuf;
		Vector4 m_size_ratio;
		
	protected:
		void initR_();
		void drawR_();
		void updateSize_();
	};

	class ParticleGravity : public Particle {
	protected:
		struct ParticleInfo
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
		ParticleGravity(std::string const & name = "gravity particle");

		void setParticleEmitDirect(float randians);
		void setParticleEmitDirectVarying(float var);
		void setParticleEmitSpeed(float speed);
		void setParticleEmitSpeedVarying(float speed_var);
		void setParticleEmitPosition(Point2 pos);
		void setParticleEmitPositionVarying(Point2 pos_var);
		void setParticleGravityDirect(float r);
		void setParticleGravityDirectVarying(float r_var);
		void setParticleGravityValue(float acc);
		void setParticleGravityAccVarying(float acc_var);

	protected:
		virtual void resetParticles() override;
		virtual bool updateParticles(float) override;

		virtual void initSingleParticle(size_t index) override;
		virtual bool updateSingleParticle(size_t index, float) override;

		float m_emit_radians = ZEN_F_PI2;
		float m_emit_radians_var = 0;
		float m_emit_speed = 10;
		float m_emit_speed_var = 0;

		float m_gravity_radians = ZEN_F_PI2;
		float m_gravity_radians_var = 0.f;
		float m_gravity_acc = 10;
		float m_gravity_acc_var = 0;

		Zen::Point2 m_emit_pos = { 0, 0 };
		Zen::Point2 m_emit_pos_var = { 0, 0 };

		std::vector<ParticleInfo> m_particles;
	};
}}

namespace Zen { namespace Vap {

	class ParticleGalaxy : public Particle
	{
	public:
		ParticleGalaxy(std::string const & name = "gravity particle");

		void setParticleEmitAngle(float rad);
		void setParticleEmitAngularSpeed(float av);
		void setParticleStartRadius(float r);
		void setParticleEndRadius(float r);
		void setParticleAngularAcc(float acc);

		void setParticleEmitAngleVar(float rad_var);
		void setParticleEmitAngularSpeedVar(float av_var);
		void setParticleStartRadiusVar(float r_var);
		void setParticleEndRadiusVar(float r_var);
		void setParticleAngularAccVar(float acc_var);

		void setRadiusLerpFunction(std::function<float(float s, float e, float v)> lerp);
		
	protected:
		struct ParticleInfo
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
			bool alive = false;
		};
	protected:
		virtual void resetParticles() override;
		virtual bool updateParticles(float) override;

		virtual void initSingleParticle(size_t index) override;
		virtual bool updateSingleParticle(size_t index, float) override;
		
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

		std::vector<ParticleInfo> m_particles;

		std::function<float(float s, float e, float v)> m_radius_lerp;
	};
}}
