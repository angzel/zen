#pragma once

#include <string>
#include <memory>
#import "zen_metal_shader.h"
#import "zen_metal.h"


namespace Zen { namespace Metal {

	typedef std::string VertexShader;
	typedef std::string FragmentShader;

}}

namespace Zen { namespace Metal {
	typedef std::shared_ptr<Pipe> SharedPipe;

	class Pipes
	{
	public:
		static Pipes * Get();

		SharedPipe getShaderColor(bool to_grey, eBlendFactor src, eBlendFactor dst);

		SharedPipe getShaderSampler(bool to_grey, bool only_alpha, eBlendFactor src, eBlendFactor dst);

		SharedPipe getShaderParticle(bool to_grey, bool only_alpha, eBlendFactor src, eBlendFactor dst);

	protected:
		Pipes();
		Pipes(Pipes&) = delete;
		virtual ~Pipes() = default;
		void operator = (int) = delete;
	};
}}
