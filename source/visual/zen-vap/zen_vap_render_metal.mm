
#include "zen_vap_render.h"

#if ZEN_DRAW_METAL

#include "zen_metal_prepare.h"
#include "zen_metal_id.h"

#include "zen_vap.h"

namespace Zen { namespace Vap {
		
	std::pair<Metal::eBF, Metal::eBF> MetalHelper::GetBlends(eBlend blend)
	{
		if(blend == eBlend::Inherit)
		{
			blend = DrawStack::Get()->getTopBlend();
		}
		switch (blend) {
			case eBlend::Add:
				return {
					Metal::eBF::SourceAlpha,
					Metal::eBF::One
				};
				break;
			case eBlend::Inverse:
				return {
					Metal::eBF::One,
					Metal::eBF::SourceAlpha
				};
				break;
			case eBlend::Normal:
				return {
					Metal::eBF::SourceAlpha,
					Metal::eBF::OneMinusSourceAlpha
				};
				break;
			case eBlend::None:
			default:
				return {
					Metal::eBF::None,
					Metal::eBF::None
				};
		}
	}
	
	void RBufferTexture::initBufferV4()
	{
		VertexIN_ShaderT coords[] = {
			{ { 0, 0, }, { 0, 1, }, },
			{ { 1, 0, }, { 1, 1, }, },
			{ { 0, 1, }, { 0, 0, }, },
			{ { 1, 1, }, { 1, 0, }, },
		};
		this->createVertexBuffer(coords, 4);
	}
	void RBufferTexture::updateBufferV4(float x0, float y0, float x1, float y1)
	{
		VertexIN_ShaderT coords[] = {
			{ { 0, 0, }, { x0, y1, }, },
			{ { 1, 0, }, { x1, y1, }, },
			{ { 0, 1, }, { x0, y0, }, },
			{ { 1, 1, }, { x1, y0, }, },
		};
		this->updateVertexBuffer(coords, 4, 0);
	}
	
	void Texture::set(size_t width, size_t height, ePixel format, void const * data)
	{
		if(format == ePixel::GA)
		{
			auto image = Image::Create(ePixel::RGBA, width, height);
			image->copy(format, data);
			return this->set(image.get());
		}
		else if(format == ePixel::RGB)
		{
			auto image = Image::CreateWidthByte(ePixel::RGBA, width, height, 0xff);
			image->copyColor(format, data);
			return this->set(image.get());
		}
		else
		{
			m_format = format;
			m_size = { (float)width, (float)height };
			m_using_size = { 1, 1 };
			m_texture.mt_buffer.create(width, height, format==ePixel::Grey, data, 0);
		}
	}
	
	void Helper::BeginRender()
	{
		Metal::Render::Get()->start();
	}
	
	void Helper::EndRender()
	{
		Metal::Render::Get()->end();
	}
	void Helper::SetClearColor(Zen::Color4f color)
	{
		Metal::Render::Get()->setClearColor(color);
	}
}}

#endif
