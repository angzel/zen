
#include "zen_metal_id.h"
#include "zen_metal_type.h"
#include "zen_metal_prepare.h"
#include <vector>
#include <map>

namespace Zen { namespace Metal {

	template<typename _TypePipe>
	std::shared_ptr<_TypePipe> ShaderCreate
	(VertexShader const & vertex, FragmentShader const & fragment,
	eBlendFactor src, eBlendFactor dst)
	{
		auto a = std::shared_ptr<_TypePipe>(new _TypePipe);
		a->getID()->create(vertex, fragment, src, dst);
		return a;
	}
	
	template<typename _TypePipe>
	std::shared_ptr<_TypePipe> ShaderCreate
	(VertexShader const & vertex, FragmentShader const & fragment)
	{
		auto a = std::shared_ptr<_TypePipe>(new _TypePipe);
		a->getID()->create(vertex, fragment);
		return a;
	}
}}

namespace Zen { namespace Metal {

	static const int S_ColorShaderCount = 2;
	static const int S_SamplerShaderCount = 4;
	static const int S_ParticleShaderCount = 4;

	static std::map<uint64_t, std::shared_ptr<ColorPipe> > S_color_shaders[S_ColorShaderCount];
	static std::map<uint64_t, std::shared_ptr<SamplerPipe> > S_sampler_shaders[S_SamplerShaderCount];
	static std::map<uint64_t, std::shared_ptr<ParticlePipe> > S_particle_shaders[S_ParticleShaderCount];

	inline static int sGetIndex(bool to_grey)
	{
		return (to_grey?1:0);
	}
	inline static int sGetIndex(bool to_grey, bool only_alpha)
	{
		return (only_alpha?2:0) | (to_grey?1:0);
	}
	inline static uint64_t sGetKey(eBlendFactor src, eBlendFactor dst)
	{
		auto i = (uint64_t)src;
		auto j = (uint64_t)dst;
		return (i<<32) | j;
	}

	static std::string S_color_funs[S_ColorShaderCount][2] = {
		{ "VertexShaderColor", "FragmentShaderColor" },
		{ "VertexShaderColor", "FragmentShaderColorGrey" },
	};

	static std::string S_sam_funs[S_SamplerShaderCount][2] = {
		{ "VertexShaderSampler", "FragmentShaderSampler_RGBA" },
		{ "VertexShaderSampler", "FragmentShaderSampler_RGBA_GA" },
		{ "VertexShaderSampler", "FragmentShaderSampler_A" },
		{ "VertexShaderSampler", "FragmentShaderSampler_A_G" },
	};

	static std::string S_par_funs[S_ParticleShaderCount][2] = {
		{ "VertexShaderParticle", "FragmentShaderParticle_RGBA" },
		{ "VertexShaderParticle", "FragmentShaderParticle_RGBA_GA" },
		{ "VertexShaderParticle", "FragmentShaderParticle_A" },
		{ "VertexShaderParticle", "FragmentShaderParticle_A_G" },
	};
	
	std::shared_ptr<ColorPipe> ColorPipe::GetShared(bool to_grey, eBlendFactor src, eBlendFactor dst)
	{
		auto index = sGetIndex(to_grey);
		auto key = sGetKey(src, dst);
		auto & sh = S_color_shaders[index][key];
		if(sh == nullptr)
		{
			sh = ShaderCreate<ColorPipe>(S_color_funs[index][0], S_color_funs[index][1], src, dst);
		}
		return sh;
	}

	std::shared_ptr<SamplerPipe> SamplerPipe::GetShared(bool to_grey, bool only_alpha, eBlendFactor src, eBlendFactor dst)
	{
		auto index = sGetIndex(to_grey, only_alpha);
		auto key = sGetKey(src, dst);
		auto & sh = S_sampler_shaders[index][key];
		if(sh == nullptr)
		{
			sh = ShaderCreate<SamplerPipe>(S_sam_funs[index][0], S_sam_funs[index][1], src, dst);
		}
		return sh;
	}

	std::shared_ptr<ParticlePipe> ParticlePipe::GetShared(bool to_grey, bool only_alpha, eBlendFactor src, eBlendFactor dst)
	{
		auto index = sGetIndex(to_grey, only_alpha);
		auto key = sGetKey(src, dst);
		auto & sh = S_particle_shaders[index][key];
		if(sh == nullptr)
		{
			sh = ShaderCreate<ParticlePipe>(S_par_funs[index][0], S_par_funs[index][1], src, dst);
		}
		return sh;
	}
}}
