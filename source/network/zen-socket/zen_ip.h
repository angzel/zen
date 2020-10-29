#pragma once

#include "zen_net_define.h"
#include "zen_endian.h"
#include <vector>

namespace Zen {

	union SocketAddress {
		sockaddr a;
		sockaddr_in v4;
		sockaddr_in6 v6;
	};

	struct SocketConfig {
		SocketFamily family = SocketFamily::inet;
		SocketType type = SocketType::stream;
		SocketProtocol ipproto = SocketProtocol::ip;
	};

	inline SocketAddress SocketAddressMake(uint8_t ip0, uint8_t ip1, uint8_t ip2, uint8_t ip3, int port)
	{
		SocketAddress res;
		::memset(&res, 0, sizeof(res));
		res.v4.sin_family = (sa_family_t)SocketFamily::inet;
		res.v4.sin_len = sizeof(sockaddr_in);
		res.v4.sin_port = htons((in_port_t)port);
		res.v4.sin_addr.s_addr = Zen::Byte4(ip0, ip1, ip2, ip3).value;
		return res;
	}
	inline SocketAddress SocketAddressMake(std::string const & host_name, int port, SocketFamily const & family)
	{
		SocketAddress ip;
		ip.a.sa_family = (int)SocketFamily::unspec;
		ip.a.sa_len = 0;

		addrinfo * info;
		if(getaddrinfo(host_name.data(), nullptr, nullptr, &info) == 0)
		{
			std::vector<SocketAddress> v;
			for(;info; info = info->ai_next)
			{
				if(info->ai_family != (int)family) continue;

				::memcpy(&ip.a, info->ai_addr, info->ai_addrlen);
				ip.v4.sin_port = htons((in_port_t)port);
				break;
			}
		}
		return ip;
	}
	inline void SocketAddressSetPort(SocketAddress & address, int port)
	{
		address.v4.sin_port = htons((in_port_t)port);
	}
	inline std::string GetAddressString(SocketAddress const & address)
	{
		static const int max_len = 256;
		char buf[256];
		std::string res;
		switch(address.a.sa_family) {
			case AF_INET:
				res = inet_ntop(AF_INET, &address.v4.sin_addr, buf, max_len);
				break;

			case AF_INET6:
				res = inet_ntop(AF_INET6, &address.v6.sin6_addr, buf, max_len);
				break;
		}
		return res;
	}
}
