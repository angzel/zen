
#include "zen_vap_layer.h"

namespace Zen { namespace Vap {
	Layer::Layer(std::string const & name)
	: BNode(name), View2d(false)
	{
	}
	void Layer::draw()
	{
		if(m_nodes.empty()) return;

		this->updateMatrix();

		DrawStack::GuardAll rg(&getMatrix(), getMatrixVersion(), this->getAlpha(), m_blend);

		BNode::draw();
	}

}}
