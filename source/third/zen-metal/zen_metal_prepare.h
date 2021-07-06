#pragma once

#include <string>
#include <memory>
#include "zen_metal_prepared_shader.h"
#include "zen_metal_type.h"

namespace Zen { namespace Metal {
	class PipeC : public Pipe
	{
	public:
		static
		std::shared_ptr<PipeC>
		GetShared(bool grayfy, eBF src, eBF dst, eBF srcA = eBF::None, eBF dstA = eBF::None);
		
		static const int VertAttribIndex = 0;
		static const int UniformIndex = 1;
	};
	
	class PipeT : public Pipe
	{
	public:
		static
		std::shared_ptr<PipeT>
		GetShared(bool grayfy, ePixel fomrat, eBF src, eBF dst, eBF srcA = eBF::None, eBF dstA = eBF::None);
		
		static const int VertAttribIndex = 0;
		static const int UniformIndex = 1;
		static const int TextureIndex = 0;
	};
	
	class PipePT : public Pipe
	{
	public:
		static
		std::shared_ptr<PipePT>
		GetShared(bool grayfy, ePixel fomrat, eBF src, eBF dst, eBF srcA = eBF::None, eBF dstA = eBF::None);
		
		static const int VertAttribIndex = 0;
		static const int UniformIndex = 1;
		static const int TextureIndex = 0;
	};
	
	class PipeP : public Pipe
	{
	public:
		static
		std::shared_ptr<PipeP>
		GetShared(bool grayfy, eBF src, eBF dst, eBF srcA = eBF::None, eBF dstA = eBF::None);
		
		static const int VertAttribIndex = 0;
		static const int UniformIndex = 1;
	};

}}
