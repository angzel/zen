#pragma once

#include <iostream>
#include <vector>
#include "zen_vap2d_config.h"
#include "zen_vap2d.h"
#include "zen_image_raw.h"
#include "zen_system.h"
#include "zen_ticker.h"

#include "zen_vap2d_config.h"

using namespace Zen::Vap2d;
using namespace Zen;

/**
 this is a custom node.
 */
class MyDrawNode : public LNode {
	Random r;
public:
	MyDrawNode()
	{
	}
};

