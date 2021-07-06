
#include "zen_metal_id.h"
#include "zen_metal_type.h"
#include "zen_metal_prepare.h"
#include <vector>
#include <map>

namespace Zen { namespace Metal {
	
	typedef std::string VShader;
	typedef std::string FShader;
	
	template<typename _TypePipe>
	std::shared_ptr<_TypePipe> sCreatePipe
	(VShader const & vertex, FShader const & fragment,
	 eBF src, eBF dst, eBF srcA, eBF dstA)
	{
		auto a = std::shared_ptr<_TypePipe>(new _TypePipe);
		a->getID()->create(vertex, fragment, src, dst, srcA, dstA);
		return a;
	}
	
	template<typename _TypePipe>
	std::shared_ptr<_TypePipe> sCreatePipe
	(VShader const & vertex, FShader const & fragment,
	 eBF src, eBF dst)
	{
		auto a = std::shared_ptr<_TypePipe>(new _TypePipe);
		a->getID()->create(vertex, fragment, src, dst);
		return a;
	}
	
	template<typename _TypePipe>
	std::shared_ptr<_TypePipe> sCreatePipe
	(VShader const & vertex, FShader const & fragment)
	{
		auto a = std::shared_ptr<_TypePipe>(new _TypePipe);
		a->getID()->create(vertex, fragment);
		return a;
	}
}}

namespace Zen { namespace Metal {
	
	static std::map<uint64_t, std::shared_ptr<PipeC> > s_pipeC_map[2];
	static std::map<uint64_t, std::shared_ptr<PipeT> > s_pipeT_map[4];
	static std::map<uint64_t, std::shared_ptr<PipeP> > s_pipeP_map[2];
	static std::map<uint64_t, std::shared_ptr<PipePT> > s_pipePT_map[4];
	
	inline static int sGetIndex(bool grayfy)
	{
		return (grayfy?1:0);
	}
	inline static int sGetIndex(bool grayfy, ePixel fomrat)
	{
		return (fomrat==ePixel::Grey?2:0) | (grayfy?1:0);
	}
	inline static uint64_t sGetBFKey(eBF src, eBF dst, eBF srcA, eBF dstA)
	{
		if(src == eBF::None || dst == eBF::None) return (uint64_t)-1;
		
		if(srcA == eBF::None || dstA == eBF::None)
			return ((uint64_t)src << 48) | ((uint64_t)dst << 32) | 0xffffffffULL;
		
		return ((uint64_t)src << 48) | ((uint64_t)dst << 32) |
		((uint64_t)srcA << 16) | (uint64_t)dstA;
	}
	
	std::shared_ptr<PipeC> PipeC::GetShared(bool grayfy, eBF src, eBF dst, eBF srcA, eBF dstA)
	{
		static std::string s_pipe_funcs[2][2] = {
			{ "VShaderC", "FShaderC" },
			{ "VShaderC", "FShaderC_G" },
		};
		
		auto index = sGetIndex(grayfy);
		auto key = sGetBFKey(src, dst, srcA, dstA);
		auto & sh = s_pipeC_map[index][key];
		if(sh == nullptr)
		{
			sh = sCreatePipe<PipeC>(s_pipe_funcs[index][0], s_pipe_funcs[index][1], src, dst, srcA, dstA);
		}
		return sh;
	}
	
	std::shared_ptr<PipeT> PipeT::GetShared(bool grayfy, ePixel fomrat, eBF src, eBF dst, eBF srcA, eBF dstA)
	{
		static std::string s_pipe_funcs[4][2] = {
			{ "VShaderT", "FShaderT_RGBA" },
			{ "VShaderT", "FShaderT_RGBA_GA" },
			{ "VShaderT", "FShaderT_A" },
			{ "VShaderT", "FShaderT_A_G" },
		};
		
		auto index = sGetIndex(grayfy, fomrat);
		auto key = sGetBFKey(src, dst, srcA, dstA);
		auto & sh = s_pipeT_map[index][key];
		if(sh == nullptr)
		{
			sh = sCreatePipe<PipeT>(s_pipe_funcs[index][0], s_pipe_funcs[index][1], src, dst, srcA, dstA);
		}
		return sh;
	}
	
	std::shared_ptr<PipePT> PipePT::GetShared(bool grayfy, ePixel fomrat, eBF src, eBF dst, eBF srcA, eBF dstA)
	{
		static std::string s_pipe_funcs[4][2] = {
			{ "VShaderP", "FShaderPT_RGBA" },
			{ "VShaderP", "FShaderPT_RGBA_GA" },
			{ "VShaderP", "FShaderPT_A" },
			{ "VShaderP", "FShaderPT_A_G" },
		};
		auto index = sGetIndex(grayfy, fomrat);
		auto key = sGetBFKey(src, dst, srcA, dstA);
		auto & sh = s_pipePT_map[index][key];
		if(sh == nullptr)
		{
			sh = sCreatePipe<PipePT>(s_pipe_funcs[index][0], s_pipe_funcs[index][1], src, dst, srcA, dstA);
		}
		return sh;
	}
	
	
	std::shared_ptr<PipeP> PipeP::GetShared(bool grayfy, eBF src, eBF dst, eBF srcA, eBF dstA)
	{
		static std::string s_pipe_funcs[2][2] = {
			{ "VShaderP", "FShaderP" },
			{ "VShaderP", "FShaderP_G" },
		};
		auto index = sGetIndex(grayfy);
		auto key = sGetBFKey(src, dst, srcA, dstA);
		auto & sh = s_pipeP_map[index][key];
		if(sh == nullptr)
		{
			sh = sCreatePipe<PipeP>(s_pipe_funcs[index][0], s_pipe_funcs[index][1], src, dst, srcA, dstA);
		}
		return sh;
	}

}}
