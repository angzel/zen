
#include "zen_metal_id.h"
#include "zen_metal_type.h"

namespace Zen { namespace Metal {

	Device * Device::S()
	{
		static auto single = new Device;
		return single;;
	}
	
	DeviceID * Device::getID() const
	{
		return m_id;
	}

	Device::Device()
	{
		m_id = new DeviceID;
	}
	Device::~Device()
	{
		delete m_id;
	}

	DeviceID::DeviceID()
	{
		device = MTLCreateSystemDefaultDevice();
		library = [device newDefaultLibrary];
		command_queue = [device newCommandQueue];
	}
}}
