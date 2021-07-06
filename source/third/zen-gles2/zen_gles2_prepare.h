/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#pragma once

#include "zen_gles2_shader.h"
#include "zen_color.h"

namespace Zen { namespace GL {
	/**
	 @arg
	 attributes:
	 - a_coord: vertex coords
	 - a_color: vertex colors
	 uniforms:
	 - u_transfrom: matrix transform
	 - u_color: global color
	 */
	class ShaderC
	{
	public:
		GLint a_coord;
		GLint a_color;
		GLint u_transform;
		GLint u_color;
		Zen::GL::ShaderProgram program;
	public:
		typedef std::shared_ptr<ShaderC const> Shared;

		static Shared Create(VShader const & vertex, FShader const & fragment);

	protected:
		ShaderC() = default;
		ShaderC(ShaderC &) = delete;
		void operator = (int) = delete;

	};
}}

namespace Zen { namespace GL {
	/**
	 @arg
	 attributes:
	 - a_coord: vertex coords
	 - a_sampler_coord: coord in texture.
	 uniforms:
	 - u_transfrom: matrix transform
	 - u_color: global color
	 - u_sampler: texture
	 */
	class ShaderT
	{
	public:
		GLint a_coord;
		GLint a_sampler_coord;
		GLint u_transform;
		GLint u_sampler;
		GLint u_color;
		Zen::GL::ShaderProgram program;
	public:
		typedef std::shared_ptr<ShaderT const> Shared;

		static Shared Create(VShader const & vertex, FShader const & fragment);

	protected:
		ShaderT() = default;
		ShaderT(ShaderT&) = delete;
		void operator = (int) = delete;
	};

}}

namespace Zen { namespace GL {
	/**
	 @arg
	 attributes:
	 - a_coord: vertex coord
	 - a_color: vertex color.
	 - a_size: vertex point size.
	 uniforms:
	 - u_transform: transfrom matrix.
	 - u_size_ratio: scaling for size.
	 - u_color: global color.
	 - u_sampler: texture for point.
	 */
	class ShaderP
	{
	public:
		GLint a_coord;
		GLint a_color;
		GLint a_size;
		GLint u_transform;
		GLint u_size_ratio;
		GLint u_color;
		GLint u_sampler;
		Zen::GL::ShaderProgram program;

	public:
		typedef std::shared_ptr<ShaderP const> Shared;

		static Shared Create(VShader const & vertex, FShader const & fragment);

	protected:
		ShaderP() = default;
		ShaderP(ShaderP&) = delete;
		void operator = (int) = delete;
	};
}}


namespace Zen { namespace GL {

	class ShaderPrograms
	{
	public:
		static ShaderPrograms * Get();

		ShaderC::Shared getShaderC(bool gray);

		ShaderT::Shared getShaderT(bool gray, ePixel texture_fmt);

		ShaderP::Shared getShaderP(bool gray, ePixel texture_fmt);

	protected:
		ShaderPrograms();
		ShaderPrograms(ShaderPrograms&) = delete;
		virtual ~ShaderPrograms() = default;
		void operator = (int) = delete;
	};
}}
