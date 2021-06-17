/**

 include this file to for your custom draws

 */

#pragma once

#include "zen_app_config.h"
#include "zen_app.h"

namespace Zen { namespace Vap2d {

	struct SpriteBuffer;

	struct ParticleBuffer;

	struct DoodleBuffer;

	struct Texture2DBuffer;

	enum class eBlend {
		Inherit,
		Normal,
		Add,
		Inverse,
		None,
	};
}}


#if ZEN_DRAW_GLES
#define INCLUDE_VAP2D_GLES 1
#include "zen_vap2d_gles2.h"
#undef INCLUDE_VAP2D_GLES

#elif ZEN_DRAW_METAL
#include "zen_vap2d_metal.h"

#else
#error "DRAW API invalid"
#endif
