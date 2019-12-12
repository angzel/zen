#pragma once

#include "zen_vap2d_node.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap2d {
	class Layer : public BNode, public View {
	public:
		Layer(std::string const & name = "layer");

		virtual void draw() override;	public:

	};
}}
