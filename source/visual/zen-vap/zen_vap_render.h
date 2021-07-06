/**
 
 include this file to for your custom draws
 
 */

#pragma once

#include "zen_app_config.h"
#include "zen_app.h"
#include "zen_color.h"

namespace Zen { namespace Vap {
	enum class eBlend {
		Inherit,
		Normal, Add,
		Inverse, None,
		Other,
	};
	
	struct Helper
	{
		static void SetClearColor(Zen::Color4f color);
		static void BeginRender();
		static void EndRender();
	};
	
	struct RBuffer
	{
		bool is_dirty = true;
	};
	
	struct TextureBuffer; // Inner Data for Vap::Texture
	
	struct RBufferTexture;
	
	struct RBufferPoints;
	
	struct RBufferColors;
	
	struct VertexDataTexture {
		Point2 coord;
		Point2 tt_coord;
	};
	
	struct VertexDataPoints {
		Point2 coord;
		Color4f color;
		float psize;
	};
	
	struct VertexDataColors {
		Point2 coord;
		Color4f color;
	};
	
	/// helper class for Helper
	struct GRenderGuard
	{
		GRenderGuard()
		{
			Helper::BeginRender();
		}
		
		~GRenderGuard()
		{
			Helper::EndRender();
		}
	};
}}


#include "zen_vap_render_gles2.h"
#include "zen_vap_render_metal.h"
