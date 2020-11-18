/*
 Copyright (c) 2013 MeherTJ G.

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

#pragma once

#include "zen_gles2_shader.h"

namespace Zen { namespace GL {
	class ShaderColor
	{
	public:
		GLint a_coord;
		GLint a_color;
		GLint u_transform;
//		GLint u_point_size;
		GLint u_color;
		Zen::GL::ShaderProgram program;
	public:
		typedef std::shared_ptr<ShaderColor const> Shared;

		static Shared Create(VertexShader const & vertex, FragmentShader const & fragment);

	protected:
		ShaderColor() = default;
		ShaderColor(ShaderColor&) = delete;
		void operator = (int) = delete;

	};
}}

namespace Zen { namespace GL {
	class ShaderSampler
	{
	public:
		GLint a_coord;
		GLint a_sampler_coord;
		GLint u_transform;
		GLint u_sampler;
		GLint u_color;
		Zen::GL::ShaderProgram program;
	public:
		typedef std::shared_ptr<ShaderSampler const> Shared;

		static Shared Create(VertexShader const & vertex, FragmentShader const & fragment);

	protected:
		ShaderSampler() = default;
		ShaderSampler(ShaderSampler&) = delete;
		void operator = (int) = delete;
	};

}}

namespace Zen { namespace GL {
	class ShaderParticle
	{
	public:
		GLint a_coord;
		GLint a_color;
		GLint a_size;
		GLint u_transform;
		GLint u_color;
		GLint u_sampler;
		Zen::GL::ShaderProgram program;

	public:
		typedef std::shared_ptr<ShaderParticle const> Shared;

		static Shared Create(VertexShader const & vertex, FragmentShader const & fragment);

	protected:
		ShaderParticle() = default;
		ShaderParticle(ShaderParticle&) = delete;
		void operator = (int) = delete;
	};
}}


namespace Zen { namespace GL {

	class ShaderPrograms
	{
	public:
		static ShaderPrograms * S();

		ShaderColor::Shared getShaderColor(bool to_grey);

		ShaderSampler::Shared getShaderSampler(bool to_grey, bool alpha_tex);

		ShaderParticle::Shared getShaderParticle(bool to_grey, bool alpha_tex);

	protected:
		ShaderPrograms();
		ShaderPrograms(ShaderPrograms&) = delete;
		virtual ~ShaderPrograms() = default;
		void operator = (int) = delete;
	};
}}
