#pragma once

#include "zen_vap_node.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap {
	class Layer : public BNode, public View2d {
	public:
		Layer(std::string const & name = "layer");

		virtual void draw() override;
	};
}}
