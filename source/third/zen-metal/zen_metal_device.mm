
#include "zen_metal_device_id.h"

namespace Zen { namespace Metal {

	Device * Device::Get()
	{
		static auto single = new Device;
		return single;;
	}
	
	std::shared_ptr<DeviceID> Device::getID() const
	{
		return m_id;
	}

	Device::Device()
	{
		m_id = std::shared_ptr<DeviceID>(new DeviceID);
	}
	
	void DeviceID::setMTKView(MTKView * view)
	{
		this->device = view.device;
		this->library = [device newDefaultLibrary];
		this->command_queue = [device newCommandQueue];
		this->view = view;
//		this->view.colorPixelFormat;
//		this->view.depthStencilPixelFormat;
	}
}}
