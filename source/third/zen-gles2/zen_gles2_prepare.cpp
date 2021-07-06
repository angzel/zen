/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#include "zen_gles2_prepare.h"
#include "zen_color.h"

namespace Zen { namespace GL {
	ShaderC::Shared ShaderC::Create(VShader const & vertex, FShader const & fragment)
	{
		auto s = new ShaderC();
		s->program.makeAttachAndLink(vertex, fragment);
		s->a_coord = s->program.getAttributeLocation("a_coord");
		s->a_color = s->program.getAttributeLocation("a_color");
		s->u_transform = s->program.getUniformLocation("u_transform");
		s->u_color = s->program.getUniformLocation("u_color");
		//		s->u_point_size = s->program.getUniformLocation("u_point_size");
		return std::shared_ptr<ShaderC const>(s);
	}
	ShaderP::Shared ShaderP::Create(VShader const & vertex, FShader const & fragment)
	{
		auto s = new ShaderP();
		s->program.makeAttachAndLink(vertex, fragment);
		s->a_coord = s->program.getAttributeLocation("a_coord");
		s->a_size = s->program.getAttributeLocation("a_size");
		s->a_color = s->program.getAttributeLocation("a_color");
		s->u_transform = s->program.getUniformLocation("u_transform");
		s->u_size_ratio = s->program.getUniformLocation("u_size_ratio");
		s->u_color = s->program.getUniformLocation("u_color");
		s->u_sampler = s->program.getUniformLocation("u_sampler");
		return std::shared_ptr<ShaderP const>(s);
	}
	
	ShaderT::Shared ShaderT::Create(VShader const & vertex, FShader const & fragment)
	{
		auto s = new ShaderT();
		s->program.makeAttachAndLink(vertex, fragment);
		s->a_coord = s->program.getAttributeLocation("a_coord");
		s->a_sampler_coord = s->program.getAttributeLocation("a_sampler_coord");
		s->u_transform = s->program.getUniformLocation("u_transform");
		s->u_color = s->program.getUniformLocation("u_color");
		s->u_sampler = s->program.getUniformLocation("u_sampler");
		return std::shared_ptr<ShaderT const>(s);
	}
}}

namespace Zen { namespace GL {
	
	/**
	 particle
	 */
	static const char * sVertexShaderP = R""(
	precision mediump float;
	attribute vec4 a_coord;
	attribute float a_size;
	attribute vec4 a_color;
	uniform mat4 u_transform;
	uniform vec4 u_size_ratio;
	varying vec4 v_color;
	void main()
	{
	v_color = a_color;
	gl_PointSize = length(u_transform * vec4(a_size, 0.0, 0.0, 0.0) * u_size_ratio);
	gl_Position = u_transform * a_coord;
	}
	)"";
	//	vec2 s = (u_transform * vec4(a_size, 0, 0, 0)).xy;
	
	static const char * sFragShaderP = R""(
	precision mediump float;
	varying vec4 v_color;
	uniform vec4 u_color;
	uniform sampler2D u_sampler;
	void main()
	{
	gl_FragColor = texture2D(u_sampler, gl_PointCoord) * v_color * u_color;
	}
	)"";
	
	static const char * sFragShaderP_G = R""(
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
	
	static const char * sFragShaderP_A = R""(
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
	
	static const char * sFragShaderP_A_G = R""(
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
	static const char * sVertexShaderC = R""(
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
	
	static const char * sFragShaderC = R""(
	precision mediump float;
	varying vec4 v_color;
	uniform vec4 u_color;
	void main()
	{
	gl_FragColor = v_color * u_color;
	}
	)"";
	
	static const char * sFragShaderC_G = R""(
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
	static const char * sVertexShaderT = R""(
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
	
	static const char * sFragShaderT = R""(
	precision mediump float;
	uniform sampler2D u_sampler;
	uniform vec4 u_color;
	varying vec2 v_sample_coord;
	void main()
	{
	gl_FragColor = texture2D(u_sampler, v_sample_coord) * u_color;
	}
	)"";
	
	static const char * sFragShaderT_G = R""(
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
	
	static const char * sFragShaderT_A = R""(
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
	
	static const char * sFragShaderT_A_G = R""(
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
	
	inline static int sGetIndex(bool gray)
	{
		return (gray?1:0);
	}
	
	inline static int sGetIndex(bool gray, ePixel texture_fmt)
	{
		return (texture_fmt==ePixel::Grey?2:0) | (gray?1:0);
	}
	
	static ShaderC::Shared s_shaders_color[2];
	static ShaderT::Shared s_shaders_texture[4];
	static ShaderP::Shared s_shaders_particle[4];
	
	ShaderPrograms * ShaderPrograms::Get()
	{
		static auto single = new ShaderPrograms;
		return single;;
	}
	
	ShaderPrograms::ShaderPrograms()
	{
		VShader vs_color, vs_sampler, vs_particle;
		vs_color.compile(sVertexShaderC);
		vs_sampler.compile(sVertexShaderT);
		vs_particle.compile(sVertexShaderP);
		
		FShader fs_color[2];
		fs_color[0].compile(sFragShaderC);
		fs_color[1].compile(sFragShaderC_G);
		
		FShader fs_sampler[4];
		fs_sampler[0].compile(sFragShaderT);
		fs_sampler[1].compile(sFragShaderT_G);
		fs_sampler[2].compile(sFragShaderT_A);
		fs_sampler[3].compile(sFragShaderT_A_G);
		
		FShader fs_particle[4];
		fs_particle[0].compile(sFragShaderP);
		fs_particle[1].compile(sFragShaderP_G);
		fs_particle[2].compile(sFragShaderP_A);
		fs_particle[3].compile(sFragShaderP_A_G);
		
		for(int i = 0; i < 2; ++i)
		{
			s_shaders_color[i] = ShaderC::Create(vs_color, fs_color[i]);
		}
		for(int i = 0; i < 4; ++i)
		{
			s_shaders_texture[i] = ShaderT::Create(vs_sampler, fs_sampler[i]);
			s_shaders_particle[i] = ShaderP::Create(vs_particle, fs_particle[i]);
		}
	}
	
	ShaderC::Shared
	ShaderPrograms::getShaderC(bool gray)
	{
		return s_shaders_color[sGetIndex(gray)];
	}
	
	ShaderT::Shared
	ShaderPrograms::getShaderT(bool gray, ePixel texture_fmt)
	{
		return s_shaders_texture[sGetIndex(gray, texture_fmt)];
	}
	
	ShaderP::Shared
	ShaderPrograms::getShaderP(bool gray, ePixel texture_fmt)
	{
		return s_shaders_particle[sGetIndex(gray, texture_fmt)];
	}
}}
