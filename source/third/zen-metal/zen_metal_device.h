#pragma once

#include <string>

namespace Zen { namespace Metal {
	struct DeviceID;

	class Device
	{
	protected:
		Device();
	public:
		static Device * Get();
		
		std::shared_ptr<DeviceID> getID() const;
	protected:
		std::shared_ptr<DeviceID> m_id = nullptr;
	};
}}
