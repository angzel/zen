
#include "zen_vap2d_layer.h"

namespace Zen { namespace Vap2d {
	Layer::Layer(std::string const & name)
	: BranchNode(name)
	{
	}
	void Layer::draw()
	{
		if(m_nodes.empty()) return;

		bool dirty = this->updateMatrix();

		DrawStack::GuardAll rg(&m_world_matrix, dirty, this->getAlpha(), m_blend);

		BranchNode::draw();
	}

}}
