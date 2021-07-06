
#include "zen_vap_render.h"

#if ZEN_DRAW_GLES

#include "zen_vap.h"

namespace Zen { namespace Vap {

	
	void Helper::BeginRender()
	{
		GLHelper::GetGLRender()->clear();
	}
	void Helper::EndRender()
	{
		
	}
	void Helper::SetClearColor(Zen::Color4f color)
	{
		GLHelper::GetGLRender()->setClearColor(color);
	}
	
	GL::Render * GLHelper::GetGLRender()
	{
		static auto r = new GL::Render;
		return r;
	}
	
	eBlend GLHelper::PerformBlend(eBlend blend)
	{
		if(blend == eBlend::Inherit)
		{
			blend = DrawStack::Get()->getTopBlend();
		}
		auto s = GetGLRender();
		
		switch (blend) {
			case eBlend::Normal:
				s->enableBlend();
				s->setBlendFunction
				(GL::eSrcBlend::SrcAlpha,
				 GL::eDstBlend::OneMinusSrcAlpha);
				break;
			case eBlend::Add:
				s->enableBlend();
				s->setBlendFunction
				(GL::eSrcBlend::SrcAlpha,
				 GL::eDstBlend::One);
				break;
			case eBlend::Inverse:
				s->enableBlend();
				s->setBlendFunction
				(GL::eSrcBlend::One,
				 GL::eDstBlend::SrcAlpha);
				break;
			case eBlend::None:
				s->disableBlend();
				break;
			default:
				break;
		}
		return blend;
	}

}}

#include "zen_numerical.h"

namespace Zen { namespace Vap {
	
}}

#endif
