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

    class IPv4
    {
    public:
        typedef in_addr_t IPType;
        typedef uint16_t PortType;
    protected:
        IPType mIP = INADDR_NONE;
    public:
        IPv4() = default;
        IPv4(const IPv4 & ) = default;
        
        IPType getAddressValue() const { return mIP; }
        
        void setAddressValue(IPType value) { mIP = value; }
        
        IPv4(std::string const & host_name);
        
        bool isValidAddress() const;
        
        void setAddress(uint8_t ip0, uint8_t ip1, uint8_t ip2, uint8_t ip3);
        void setHostAddress(std::string const & host_name);

        std::string getAddressString() const;
    };
    
    class IPInfo
    {
    protected:
        addrinfo * mAddress = nullptr;

		IPInfo(IPInfo const & ) = delete;
    public:
        IPInfo() = default;

		void operator = (IPInfo const &) = delete;
        
        ~IPInfo();
        
        addrinfo * getAddressInfo() const { return mAddress; }
        
        bool isValidAddress() const;
        
        void setHostAddress(std::string const & host_name,
                            std::string const & service = "",
                            addrinfo * hints = nullptr);
        
        void setLocalAddress(std::string const & service = "",
                             addrinfo * hints = nullptr);
        
        
        SocketFamily getFamily() const;
        
        bool isIPv6() const;
        
        bool isIPv4() const;
        
        void cleanAddress();
        
    public:
        static std::string GetAddressString(addrinfo * addr);
    };
}
