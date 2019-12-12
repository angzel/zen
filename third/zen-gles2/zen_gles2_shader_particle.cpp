#include "zen_gles2_shader_particle.h"

namespace Zen { namespace GL {

	std::shared_ptr<ShaderParticle const> ShaderParticle::Create(char const * vertext_shader, char const * fragment_shader)
	{
		auto s = new ShaderParticle();
		s->program.makeAttachAndLink(vertext_shader, fragment_shader);
		s->a_coord = s->program.getAttributeLocation("a_coord");
		s->a_size = s->program.getAttributeLocation("a_size");
		s->a_color = s->program.getAttributeLocation("a_color");
		s->u_transform = s->program.getUniformLocation("u_transform");
		s->u_color = s->program.getUniformLocation("u_color");
		s->u_size = s->program.getUniformLocation("u_size");
		s->u_sampler = s->program.getUniformLocation("u_sampler");
		return std::shared_ptr<ShaderParticle const>(s);
	}

	static const char * vertex_shader =
	"precision mediump float;"
	"attribute vec4 a_coord;"
	"attribute float a_size;"
	"attribute vec4 a_color;"
	"uniform mat4 u_transform;"
	"uniform float u_size;"
	"varying vec4 v_color;"
	"void main()"
	"{"
	"   v_color = a_color;"
	"	gl_PointSize = a_size * u_size;"
	"	gl_Position = u_transform * a_coord;"
	"}";
	
	std::shared_ptr<ShaderParticle const> ShaderParticle::GetNormal()
	{
		static const char * fragment =
		"precision mediump float;"
		"varying vec4 v_color;"
		"uniform vec4 u_color;"
		"uniform sampler2D u_sampler;"
		"void main()"
		"{"
		"	gl_FragColor = texture2D(u_sampler, gl_PointCoord) * v_color * u_color;"
		"}";

		static auto me = Create(vertex_shader, fragment);
		return me;
	}
	std::shared_ptr<ShaderParticle const> ShaderParticle::GetAlpha()
	{
		static const char * fragment =
		"precision mediump float;"
		"varying vec4 v_color;"
		"uniform vec4 u_color;"
		"uniform sampler2D u_sampler;"
		"void main()"
		"{"
		"	gl_FragColor = texture2D(u_sampler, gl_PointCoord).w * v_color * u_color;"
		"}";

		static auto me = Create(vertex_shader, fragment);
		return me;
	}
}}
