#pragma once

#include <string>
#include <memory>
#import "zen_metal_shader.h"
#import "zen_metal.h"


namespace Zen { namespace Metal {

	typedef std::string VShader;
	typedef std::string FShader;

}}

namespace Zen { namespace Metal {
	typedef std::shared_ptr<Pipe> SharedPipe;

//	class Pipes
//	{
//	public:
//		static Pipes * Get();
//
//		SharedPipe getShaderC(bool grayfy, eBF src, eBF dst);
//
//		SharedPipe getShaderT(bool grayfy, ePixel fomrat, eBF src, eBF dst);
//
//		SharedPipe getShaderP(bool grayfy, ePixel fomrat, eBF src, eBF dst);
//
//	protected:
//		Pipes();
//		Pipes(Pipes&) = delete;
//		virtual ~Pipes() = default;
//		void operator = (int) = delete;
//	};
}}
