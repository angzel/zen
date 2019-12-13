#pragma once

#include <iostream>
#include <vector>
#include "zen_image_raw.h"
#include "zen_system.h"
#include "zen_ticker.h"

using namespace Zen;

/**
 this is a custom node.
 */
class MyDrawNodeMTData;

class MyDrawNodeMT  {
	MyDrawNodeMTData * m_data;
public:
	MyDrawNodeMT();

	virtual void draw();
};

