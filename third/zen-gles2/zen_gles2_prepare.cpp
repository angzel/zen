/*
 Copyright (c) 2013 ClearSky G.
 
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
	static const char * S_vertex_particle =
	"precision mediump float;"
	"attribute vec4 a_coord;"
	"attribute float a_size;"
	"attribute vec4 a_color;"
	"uniform mat4 u_transform;"
	"varying vec4 v_color;"
	"void main()"
	"{"
	" v_color = a_color;"
	"gl_PointSize = a_size;"
	"gl_Position = u_transform * a_coord;"
	"}";
	
	static const char * S_fragment_particle =
	"precision mediump float;"
	"varying vec4 v_color;"
	"uniform vec4 u_color;"
	"uniform sampler2D u_sampler;"
	"void main()"
	"{"
	"gl_FragColor = texture2D(u_sampler, gl_PointCoord) * v_color * u_color;"
	"}";

	static const char * S_fragment_particle_grey =
	"precision mediump float;"
	"varying vec4 v_color;"
	"uniform vec4 u_color;"
	"uniform sampler2D u_sampler;"
	"const mediump vec3 grey = vec3(0.21, 0.71, 0.08);"
	"void main()"
	"{"
	"vec4 c = texture2D(u_sampler, gl_PointCoord) * v_color;"
	"float i = dot(c.rgb, grey);"
	"gl_FragColor = vec4(i, i, i, c.w) * u_color;"
	"}";

	static const char * S_fragment_particle_alpha =
	"precision mediump float;"
	"varying vec4 v_color;"
	"uniform vec4 u_color;"
	"uniform sampler2D u_sampler;"
	"void main()"
	"{"
	"gl_FragColor = texture2D(u_sampler, gl_PointCoord).w * v_color * u_color;"
	"}";
	
	
	/**
	 color
	 */
	static const char * S_vertex_color =
	"precision mediump float;"
	"attribute vec4 a_coord;"
	"attribute vec4 a_color;"
	"uniform mat4 u_transform;"
	"varying vec4 v_color;"
	"void main()"
	"{"
	"v_color = a_color;"
//	"gl_PointSize = 100.0;"
	"gl_Position = u_transform * a_coord;"
	"}";
	
	static const char * S_fragment_color =
	"precision mediump float;"
	"varying vec4 v_color;"
	"uniform vec4 u_color;"
	"void main()"
	"{"
	"gl_FragColor = v_color * u_color;"
	"}";
	
	static const char * S_fragment_color_grey =
	"precision mediump float;"
	"varying vec4 v_color;"
	"uniform vec4 u_color;"
	"const mediump vec3 grey = vec3(0.21, 0.71, 0.08);"
	"void main()"
	"{"
	"float i = dot(v_color.rgb, grey);"
	"gl_FragColor = vec4(i, i, i, v_color.w) * u_color;"
	"}";

	/**
	 sampler
	 */
	static const char * S_vertex_sampler =
	"precision mediump float;"
	"attribute vec4 a_coord;"
	"attribute vec2 a_sampler_coord;"
	"uniform mat4 u_transform;"
	"varying vec2 v_sample_coord;"
	"void main()"
	"{"
	"v_sample_coord = a_sampler_coord;"
	"gl_Position = u_transform * a_coord;"
	"}";

	static const char * S_fragment_sampler =
	"precision mediump float;"
	"uniform sampler2D u_sampler;"
	"uniform vec4 u_color;"
	"varying vec2 v_sample_coord;"
	"void main()"
	"{"
	"gl_FragColor = texture2D(u_sampler, v_sample_coord) * u_color;"
	"}";
	
	static const char * S_fragment_sampler_grey =
	"precision mediump float;"
	"uniform sampler2D u_sampler;"
	"uniform vec4 u_color;"
	"varying vec2 v_sample_coord;"
	"const mediump vec3 grey = vec3(0.21, 0.71, 0.08);"
	"void main()"
	"{"
	"vec4 c = texture2D(u_sampler, v_sample_coord);"
	"float i = dot(c.rgb, grey);"
	"gl_FragColor = vec4(i, i, i, c.w) * u_color;"
	"}";
	
	static const char * S_fragment_sampler_alpha =
	"precision mediump float;"
	"uniform sampler2D u_sampler;"
	"uniform vec4 u_color;"
	"varying vec2 v_sample_coord;"
	"void main()"
	"{"
	"gl_FragColor = vec4(u_color.rgb, u_color.w * texture2D(u_sampler, v_sample_coord).w);"
	"}";

	static ShaderColor::Shared S_shader_colors[2] = { nullptr, nullptr };
	static ShaderSampler::Shared S_shader_samplers[3] = { nullptr, nullptr, nullptr };
	static ShaderParticle::Shared S_shader_particles[3] = { nullptr, nullptr, nullptr };

	ShaderPrograms * ShaderPrograms::S()
	{
		static auto single = new ShaderPrograms;
		return single;;
	}
	
	ShaderPrograms::ShaderPrograms()
	{
		VertexShader vs_color, vs_sampler, vs_particle;
		vs_color.compile(S_vertex_color);
		vs_sampler.compile(S_vertex_sampler);
		vs_particle.compile(S_vertex_particle);
		FragmentShader fs_color, fs_color_grey;
		fs_color.compile(S_fragment_color);
		fs_color_grey.compile(S_fragment_color_grey);
		FragmentShader fs_sampler, fs_sampler_grey, fs_sampler_alpha;
		fs_sampler.compile(S_fragment_sampler);
		fs_sampler_grey.compile(S_fragment_sampler_grey);
		fs_sampler_alpha.compile(S_fragment_sampler_alpha);
		FragmentShader fs_particle, fs_particle_grey, fs_particle_alpha;
		fs_particle.compile(S_fragment_particle);
		fs_particle_grey.compile(S_fragment_particle_grey);
		fs_particle_alpha.compile(S_fragment_particle_alpha);

		S_shader_colors[0] = ShaderColor::Create(vs_color, fs_color);
		S_shader_colors[1] = ShaderColor::Create(vs_color, fs_color_grey);

		S_shader_particles[0] = ShaderParticle::Create(vs_particle, fs_particle);
		S_shader_particles[1] = ShaderParticle::Create(vs_particle, fs_particle_grey);
		S_shader_particles[2] = ShaderParticle::Create(vs_particle, fs_particle_alpha);

		S_shader_samplers[0] = ShaderSampler::Create(vs_sampler, fs_sampler);
		S_shader_samplers[1] = ShaderSampler::Create(vs_sampler, fs_sampler_grey);
		S_shader_samplers[2] = ShaderSampler::Create(vs_sampler, fs_sampler_alpha);
	}

	ShaderColor::Shared ShaderPrograms::getShaderColor(bool to_grey)
	{
		return S_shader_colors[to_grey?1:0];
	}

	ShaderSampler::Shared ShaderPrograms::getShaderSampler(bool to_grey, bool alpha_tex)
	{
		return S_shader_samplers[alpha_tex?2:(to_grey?1:0)];
	}

	ShaderParticle::Shared ShaderPrograms::getShaderParticle(bool to_grey, bool alpha_tex)
	{
		return S_shader_particles[alpha_tex?2:(to_grey?1:0)];
	}
	
}}
