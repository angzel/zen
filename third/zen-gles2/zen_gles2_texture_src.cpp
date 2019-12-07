#include "zen_gles2_texture_src.h"

namespace Zen { namespace GL {
	std::shared_ptr<TextureSrc const>
	TextureSrc::Create(uint32_t width, uint32_t height, ETextureFmt format, void const * data)
	{
		auto ts = new TextureSrc;
		ts->texture.create();
		ts->texture.bindData(width, height, format, data, 0);
		ts->size = { (float)width, (float)height };
		ts->real_size = ts->size;
		ts->gl_size = { 1.0f, 1.0f };
		return std::shared_ptr<TextureSrc const>(ts);
	}
}}
