#pragma once

#include <string>
#include <memory>
#include "zen_metal_shader.h"
#include "zen_metal_type.h"

namespace Zen { namespace Metal {

	typedef std::string VertexShader;
	typedef std::string FragmentShader;

}}

namespace Zen { namespace Metal {
	typedef std::shared_ptr<Pipe> SharedPipe;

	class Pipes
	{
	public:
		static Pipes * S();

		SharedPipe getShaderColor(bool to_grey, eBlendFactor src, eBlendFactor dst);

		SharedPipe getShaderSampler(bool to_grey, bool alpha_tex, eBlendFactor src, eBlendFactor dst);

		SharedPipe getShaderParticle(bool to_grey, bool alpha_tex, eBlendFactor src, eBlendFactor dst);

		SharedPipe getShaderColor(bool to_grey);

		SharedPipe getShaderSampler(bool to_grey, bool alpha_tex);

		SharedPipe getShaderParticle(bool to_grey, bool alpha_tex);

	protected:
		Pipes();
		Pipes(Pipes&) = delete;
		virtual ~Pipes() = default;
		void operator = (int) = delete;
	};
}}
