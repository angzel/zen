
#include "zen_view_gl_layer.h"
#include "zen_view_gl_director.h"

namespace Zen { namespace Vap2d {
	void GLLayer::draw()
	{
		if(!this->isVisible()) return;

		if(mChildren.empty()) return;

		if(this->mIsChildrenDirty) this->_cleanChildrenDirty();

#include "zen_view_gl_update_final_mat_part.h"

		_GLNodeDrawingHelper helper(this);

		auto & children = mChildren;
		for(auto i : children)
		{
			i->draw();
		}
	}

	Mat4 * GLLayer::_getMat()
	{
		return &mFinalMat;
	}

	int GLLayer::_getMatVersion()
	{
		return mMatVersion;
	}

	Layer * Layer::Create()
	{
		return new GLLayer();
	}
}}
