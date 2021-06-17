/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
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
		static ShaderPrograms * Get();

		ShaderColor::Shared getShaderColor(bool to_grey);

		ShaderSampler::Shared getShaderSampler(bool to_grey, bool only_alpha);

		ShaderParticle::Shared getShaderParticle(bool to_grey, bool only_alpha);

	protected:
		ShaderPrograms();
		ShaderPrograms(ShaderPrograms&) = delete;
		virtual ~ShaderPrograms() = default;
		void operator = (int) = delete;
	};
}}
