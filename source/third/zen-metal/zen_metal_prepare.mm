
#include "zen_metal_id.h"
#include "zen_metal_type.h"
#include "zen_metal_prepare.h"
#include <vector>

namespace Zen { namespace Metal {

	SharedPipe ShaderCreate
	(VertexShader const & vertex, FragmentShader const & fragment,
	eBlendFactor src, eBlendFactor dst)
	{
		auto a = new Pipe;
		a->getID()->create(vertex, fragment, src, dst);
		return std::shared_ptr<Pipe>(a);
	}

	SharedPipe ShaderCreate
	(VertexShader const & vertex, FragmentShader const & fragment)
	{
		auto a = new Pipe;
		a->getID()->create(vertex, fragment);
		return std::shared_ptr<Pipe>(a);
	}
}}

namespace Zen { namespace Metal {

	static const int S_MaxBlend = (int)eBlendFactor::_Count;
	static const int S_ColorShaderCount = 2;
	static const int S_SamplerShaderCount = 3;
	static const int S_ParticleShaderCount = 3;

	static std::vector<SharedPipe> S_color_shaders[S_ColorShaderCount];
	static std::vector<SharedPipe> S_sampler_shaders[S_SamplerShaderCount];
	static std::vector<SharedPipe> S_particle_shaders[S_ParticleShaderCount];

	static SharedPipe S_color_shaders_no_blend[S_ColorShaderCount];
	static SharedPipe S_sampler_shaders_no_blend[S_SamplerShaderCount];
	static SharedPipe S_particle_shaders_no_blend[S_ParticleShaderCount];

	static std::string S_color_functions[S_ColorShaderCount][2] = {
		{ "VertexShaderColor", "FragmentShaderColor" },
		{ "VertexShaderColor", "FragmentShaderColorGrey" },
	};

	static std::string S_sampler_functions[S_SamplerShaderCount][2] = {
		{ "VertexShaderSampler", "FragmentShaderSampler" },
		{ "VertexShaderSampler", "FragmentShaderSamplerGrey" },
		{ "VertexShaderSampler", "FragmentShaderSamplerAlpha" },
	};


	static std::string S_particle_functions[S_ParticleShaderCount][2] = {
		{ "VertexShaderParticle", "FragmentShaderParticle" },
		{ "VertexShaderParticle", "FragmentShaderParticleGrey" },
		{ "VertexShaderParticle", "FragmentShaderParticleAlpha" },
	};


	SharedPipe Pipes::getShaderColor(bool to_grey, eBlendFactor src, eBlendFactor dst)
	{
		auto index = (int)src * S_MaxBlend + (int)dst;
		auto i2 = to_grey?1:0;
		auto & sh = S_color_shaders[i2][index];
		if(sh == nullptr)
		{
			sh = ShaderCreate(S_color_functions[i2][0], S_color_functions[i2][1], src, dst);
		}
		return sh;
	}

	SharedPipe Pipes::getShaderSampler(bool to_grey, bool alpha_tex, eBlendFactor src, eBlendFactor dst)
	{
		auto index = (int)src * S_MaxBlend + (int)dst;
		auto i2 = alpha_tex?2 :(to_grey?1:0);
		auto & sh = S_sampler_shaders[i2][index];
		if(sh == nullptr)
		{
			sh = ShaderCreate(S_sampler_functions[i2][0], S_sampler_functions[i2][1], src, dst);
		}
		return sh;
	}

	SharedPipe Pipes::getShaderParticle(bool to_grey, bool alpha_tex, eBlendFactor src, eBlendFactor dst)
	{
		auto index = (int)src * S_MaxBlend + (int)dst;
		auto i2 = alpha_tex?2 :(to_grey?1:0);
		auto & sh = S_particle_shaders[i2][index];
		if(sh == nullptr)
		{
			sh = ShaderCreate(S_particle_functions[i2][0], S_particle_functions[i2][1], src, dst);
		}
		return sh;
	}


	SharedPipe Pipes::getShaderColor(bool to_grey)
	{
		auto i2 = to_grey?1:0;
		auto & sh = S_color_shaders_no_blend[i2];
		if(sh == nullptr)
		{
			sh = ShaderCreate(S_color_functions[i2][0], S_color_functions[i2][1]);
		}
		return sh;
	}

	SharedPipe Pipes::getShaderSampler(bool to_grey, bool alpha_tex)
	{
		auto i2 = alpha_tex?2 :(to_grey?1:0);
		auto & sh = S_sampler_shaders_no_blend[i2];
		if(sh == nullptr)
		{
			sh = ShaderCreate(S_sampler_functions[i2][0], S_sampler_functions[i2][1]);
		}
		return sh;
	}

	SharedPipe Pipes::getShaderParticle(bool to_grey, bool alpha_tex)
	{
		auto i2 = alpha_tex?2 :(to_grey?1:0);
		auto & sh = S_particle_shaders_no_blend[i2];
		if(sh == nullptr)
		{
			sh = ShaderCreate(S_particle_functions[i2][0], S_particle_functions[i2][1]);
		}
		return sh;
	}

	Pipes::Pipes()
	{
		auto s = S_MaxBlend * S_MaxBlend;
		for(auto & i : S_color_shaders) i.resize(s);
		for(auto & i : S_particle_shaders) i.resize(s);
		for(auto & i : S_sampler_shaders) i.resize(s);
	}

	Pipes * Pipes::S()
	{
		static auto single = new Pipes;
		return single;;
	}

}}
