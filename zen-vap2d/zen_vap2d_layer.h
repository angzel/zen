#pragma once

#include "zen_vap2d_node.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap2d {
	class Layer : public BNode, public View {
	public:
		Layer();

		virtual void draw() override;
	};
}}
