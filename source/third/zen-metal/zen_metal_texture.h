#pragma once

#include <string>
#include "zen_color.h"
#include "zen_metal_type.h"

namespace Zen { namespace Metal {
	struct TextureID;

	class Texture
	{
	public:
		Texture();
		virtual ~Texture();
		
		std::shared_ptr<TextureID> getID() const;

		void create
		(size_t width, size_t height,
		 bool is_a8, void const * data_null, int level);
	public:
		std::shared_ptr<TextureID> m_id = nullptr;
		ePixel m_format = ePixel::None;
	};

}}
