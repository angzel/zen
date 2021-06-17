/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#include "zen_gles2_prepare.h"

namespace Zen { namespace GL {
	ShaderColor::Shared ShaderColor::Create(VertexShader const & vertex, FragmentShader const & fragment)
	{
		auto s = new ShaderColor();
		s->program.makeAttachAndLink(vertex, fragment);
		s->a_coord = s->program.getAttributeLocation("a_coord");
		s->a_color = s->program.getAttributeLocation("a_color");
		s->u_transform = s->program.getUniformLocation("u_transform");
		s->u_color = s->program.getUniformLocation("u_color");
		//		s->u_point_size = s->program.getUniformLocation("u_point_size");
		return std::shared_ptr<ShaderColor const>(s);
	}
	ShaderParticle::Shared ShaderParticle::Create(VertexShader const & vertex, FragmentShader const & fragment)
	{
		auto s = new ShaderParticle();
		s->program.makeAttachAndLink(vertex, fragment);
		s->a_coord = s->program.getAttributeLocation("a_coord");
		s->a_size = s->program.getAttributeLocation("a_size");
		s->a_color = s->program.getAttributeLocation("a_color");
		s->u_transform = s->program.getUniformLocation("u_transform");
		s->u_color = s->program.getUniformLocation("u_color");
		s->u_sampler = s->program.getUniformLocation("u_sampler");
		return std::shared_ptr<ShaderParticle const>(s);
	}
	
	ShaderSampler::Shared ShaderSampler::Create(VertexShader const & vertex, FragmentShader const & fragment)
	{
		auto s = new ShaderSampler();
		s->program.makeAttachAndLink(vertex, fragment);
		s->a_coord = s->program.getAttributeLocation("a_coord");
		s->a_sampler_coord = s->program.getAttributeLocation("a_sampler_coord");
		s->u_transform = s->program.getUniformLocation("u_transform");
		s->u_color = s->program.getUniformLocation("u_color");
		s->u_sampler = s->program.getUniformLocation("u_sampler");
		return std::shared_ptr<ShaderSampler const>(s);
	}
}}

namespace Zen { namespace GL {

	/**
	 particle
	 */
	static const char * sVertexSParticle = R""(
	precision mediump float;
	attribute vec4 a_coord;
	attribute float a_size;
	attribute vec4 a_color;
	uniform mat4 u_transform;
	varying vec4 v_color;
	void main()
	{
		v_color = a_color;
		gl_PointSize = a_size;
		gl_Position = u_transform * a_coord;
	}
	)"";

	static const char * sFragPar = R""(
	precision mediump float;
	varying vec4 v_color;
	uniform vec4 u_color;
	uniform sampler2D u_sampler;
	void main()
	{
		gl_FragColor = texture2D(u_sampler, gl_PointCoord) * v_color * u_color;
	}
	)"";

	static const char * sFragPar_G = R""(
	precision mediump float;
	varying vec4 v_color;
	uniform vec4 u_color;
	uniform sampler2D u_sampler;
	const mediump vec3 grey = vec3(0.21, 0.71, 0.08);
	void main()
	{
		vec4 c = texture2D(u_sampler, gl_PointCoord) * v_color;
		float i = dot(c.rgb, grey);
		gl_FragColor = vec4(i, i, i, c.w) * u_color;
	}
	)"";

	static const char * sFragPar_A = R""(
	precision mediump float;
	varying vec4 v_color;
	uniform vec4 u_color;
	uniform sampler2D u_sampler;
	void main()
	{
		vec4 c = v_color * u_color;
		float a = texture2D(u_sampler, gl_PointCoord).w;
		gl_FragColor = vec4(c.rgb, c.w * a);
	}
	)"";

	static const char * sFragPar_A_G = R""(
	precision mediump float;
	varying vec4 v_color;
	uniform vec4 u_color;
	uniform sampler2D u_sampler;
	void main()
	{
		float i = texture2D(u_sampler, gl_PointCoord).w;
		gl_FragColor = vec4(i, i, i, 1) * v_color * u_color;
	}
	)"";


	/**
	 color
	 */
	static const char * sVertexSColor = R""(
	precision mediump float;
	attribute vec4 a_coord;
	attribute vec4 a_color;
	uniform mat4 u_transform;
	varying vec4 v_color;
	void main()
	{
		v_color = a_color;
		gl_Position = u_transform * a_coord;
	}
	)"";

	static const char * sFragSColor = R""(
	precision mediump float;
	varying vec4 v_color;
	uniform vec4 u_color;
	void main()
	{
		gl_FragColor = v_color * u_color;
	}
	)"";

	static const char * sFragSColor_G = R""(
	precision mediump float;
	varying vec4 v_color;
	uniform vec4 u_color;
	const mediump vec3 grey = vec3(0.21, 0.71, 0.08);
	void main()
	{
		float i = dot(v_color.rgb, grey);
		gl_FragColor = vec4(i, i, i, v_color.w) * u_color;
	}
	)"";

	/**
	 sampler
	 */
	static const char * sVertexSSampler = R""(
	precision mediump float;
	attribute vec4 a_coord;
	attribute vec2 a_sampler_coord;
	uniform mat4 u_transform;
	varying vec2 v_sample_coord;
	void main()
	{
		v_sample_coord = a_sampler_coord;
		gl_Position = u_transform * a_coord;
	}
	)"";

	static const char * sFragSamp = R""(
	precision mediump float;
	uniform sampler2D u_sampler;
	uniform vec4 u_color;
	varying vec2 v_sample_coord;
	void main()
	{
		gl_FragColor = texture2D(u_sampler, v_sample_coord) * u_color;
	}
	)"";

	static const char * sFragSamp_G = R""(
	precision mediump float;
	uniform sampler2D u_sampler;
	uniform vec4 u_color;
	varying vec2 v_sample_coord;
	const mediump vec3 grey = vec3(0.21, 0.71, 0.08);
	void main()
	{
		vec4 c = texture2D(u_sampler, v_sample_coord);
		float i = dot(c.rgb, grey);
		gl_FragColor = vec4(i, i, i, c.w) * u_color;
	}
	)"";

	static const char * sFragSamp_A = R""(
	precision mediump float;
	uniform sampler2D u_sampler;
	uniform vec4 u_color;
	varying vec2 v_sample_coord;
	void main()
	{
		float a = texture2D(u_sampler, v_sample_coord).w;
		gl_FragColor = vec4(u_color.rgb, u_color.w * a);
	}
	)"";

	static const char * sFragSamp_A_G = R""(
	precision mediump float;
	uniform sampler2D u_sampler;
	uniform vec4 u_color;
	varying vec2 v_sample_coord;
	void main()
	{
		float i = texture2D(u_sampler, v_sample_coord).w;
		gl_FragColor = vec4(i, i, i, 1) * u_color;
	}
	)"";

	inline static int sGetIndex(bool to_grey)
	{
		return (to_grey?1:0);
	}
	inline static int sGetIndex(bool to_grey, bool only_alpha)
	{
		return (only_alpha?2:0) | (to_grey?1:0);
	}

	static ShaderColor::Shared sShaColorList[2];
	static ShaderSampler::Shared sShaSamplerList[4];
	static ShaderParticle::Shared sShaParticleList[4];

	ShaderPrograms * ShaderPrograms::Get()
	{
		static auto single = new ShaderPrograms;
		return single;;
	}
	
	ShaderPrograms::ShaderPrograms()
	{
		VertexShader vs_color, vs_sampler, vs_particle;
		vs_color.compile(sVertexSColor);
		vs_sampler.compile(sVertexSSampler);
		vs_particle.compile(sVertexSParticle);

		FragmentShader fs_color[2];
		fs_color[0].compile(sFragSColor);
		fs_color[1].compile(sFragSColor_G);

		FragmentShader fs_sampler[4];
		fs_sampler[0].compile(sFragSamp);
		fs_sampler[1].compile(sFragSamp_G);
		fs_sampler[2].compile(sFragSamp_A);
		fs_sampler[3].compile(sFragSamp_A_G);

		FragmentShader fs_particle[4];
		fs_particle[0].compile(sFragPar);
		fs_particle[1].compile(sFragPar_G);
		fs_particle[2].compile(sFragPar_A);
		fs_particle[3].compile(sFragPar_A_G);

		for(int i = 0; i < 2; ++i)
		{
			sShaColorList[i] = ShaderColor::Create(vs_color, fs_color[i]);
		}
		for(int i = 0; i < 4; ++i)
		{
			sShaSamplerList[i] = ShaderSampler::Create(vs_sampler, fs_sampler[i]);
			sShaParticleList[i] = ShaderParticle::Create(vs_particle, fs_particle[i]);
		}
	}

	ShaderColor::Shared
	ShaderPrograms::getShaderColor(bool to_grey)
	{
		return sShaColorList[sGetIndex(to_grey)];
	}

	ShaderSampler::Shared
	ShaderPrograms::getShaderSampler(bool to_grey, bool only_alpha)
	{
		return sShaSamplerList[sGetIndex(to_grey, only_alpha)];
	}

	ShaderParticle::Shared
	ShaderPrograms::getShaderParticle(bool to_grey, bool only_alpha)
	{
		return sShaParticleList[sGetIndex(to_grey, only_alpha)];
	}
	
}}
