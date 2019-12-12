
#include "zen_vap2d_layer.h"

namespace Zen { namespace Vap2d {
	Layer::Layer(std::string const & name)
	: BNode(name)
	{
	}
	void Layer::draw()
	{
		if(m_nodes.empty()) return;

		bool dirty = this->updateWorldMatrix();

		RenderStackGuard rg(&m_world_matrix, dirty, this->getAlpha(), m_blend);

		BNode::draw();
	}

}}
