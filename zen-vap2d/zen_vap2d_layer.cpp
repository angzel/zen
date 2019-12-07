
#include "zen_vap2d_layer.h"

namespace Zen { namespace Vap2d {
	Layer::Layer()
	{
	}
	void Layer::draw()
	{
		if(m_nodes.empty()) return;

		bool dirty = this->updateWorldMatrix();

		RenderNode rn(&m_world_matrix, dirty, m_alpha, m_blend);

		BNode::draw();
	}
}}
