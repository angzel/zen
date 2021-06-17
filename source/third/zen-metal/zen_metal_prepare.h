#pragma once

#include <string>
#include <memory>
#include "zen_metal_prepared_shader.h"
#include "zen_metal_type.h"

namespace Zen { namespace Metal {

	typedef std::string VertexShader;
	typedef std::string FragmentShader;
	
	class ColorPipe : public Pipe
	{
	public:
		static
		std::shared_ptr<ColorPipe> GetShared(bool to_grey, eBlendFactor src, eBlendFactor dst);
		
		static const int VertexIndex = 0;
		static const int VertexExtraIndex = 1;
	};
	
	class SamplerPipe : public Pipe
	{
	public:
		static
		std::shared_ptr<SamplerPipe> GetShared(bool to_grey, bool only_alpha, eBlendFactor src, eBlendFactor dst);
		
		static const int VertexIndex = 0;
		static const int VertexExtraIndex = 1;
		static const int TextureIndex = 0;
	};
	
	class ParticlePipe : public Pipe
	{
	public:
		static
		std::shared_ptr<ParticlePipe> GetShared(bool to_grey, bool only_alpha, eBlendFactor src, eBlendFactor dst);
		
		static const int VertexIndex = 0;
		static const int VertexExtraIndex = 1;
		static const int TextureIndex = 0;
	};

}}
