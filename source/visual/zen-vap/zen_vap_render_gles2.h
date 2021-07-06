/**
 dont include this file.
 */
#pragma once

#include "zen_app_config.h"

#if ZEN_DRAW_GLES

#include "zen_gles2.h"

namespace Zen { namespace Vap {
	typedef Zen::GL::eVertexMode eVertexMode;

	struct GLHelper {
		static eBlend PerformBlend(eBlend blend);
		static GL::Render * GetGLRender();
	};
	
	struct RBuffer_i : public RBuffer
	{
		Zen::GL::ArrayBuffer v;
		size_t size = 0;
	};

	struct RBufferTexture : RBuffer_i
	{
	};

	struct RBufferColors : RBuffer_i
	{
	};

	struct RBufferPoints : RBuffer_i
	{
	};

	struct TextureBuffer
	{
		GL::Texture2DBuffer gl_buffer;
	};
}}

#endif
