/**

 include this file to for your custom draws

 */

#pragma once

#include "zen_app_config.h"
#include "zen_app_runtime.h"

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


#if ZEN_APP_DRAW_API_OPENGLES
#define INCLUDE_VAP2D_GLES 1
#include "zen_vap2d_gles2.h"
#undef INCLUDE_VAP2D_GLES
#elif ZEN_APP_DRAW_API_METAL
#include "zen_vap2d_metal.h"
#else
#error "DRAW API invalid"
#endif
