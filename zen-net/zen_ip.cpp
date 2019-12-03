
#include "zen_ip.h"
#include "zen_endian.h"

namespace Zen {
    IPv4::IPv4(std::string const & host_name)
    {
        this->setHostAddress(host_name);
    }
    
    bool IPv4::isValidAddress() const
    {
        return mIP != INADDR_NONE;
    }
    
    void IPv4::setAddress(uint8_t ip0, uint8_t ip1, uint8_t ip2, uint8_t ip3)
    {
        mIP = Zen::Byte4(ip0, ip1, ip2, ip3).value;
    }
    void IPv4::setHostAddress(std::string const & host_name)
    {
        auto host = gethostbyname(host_name.data());
        if(host && host->h_addrtype == (int)SocketFamily::inet
        && host->h_addr_list != nullptr)
        {
            mIP = *(in_addr_t*)(host->h_addr_list[0]);
        }
        else
        {
            mIP = INADDR_NONE;
        }
    }

    std::string IPv4::getAddressString() const
    {
        in_addr ad;
        ad.s_addr = mIP;
        auto str = ::inet_ntoa(ad);
        if(str == nullptr) return "";
        return str;
    }
    
    
    IPInfo::~IPInfo()
    {
        this->cleanAddress();
    }
        
    bool IPInfo::isValidAddress() const
    {
        return mAddress != nullptr;
    }
    
    void IPInfo::cleanAddress()
    {
        if(mAddress)
        {
            freeaddrinfo(mAddress);
            mAddress = nullptr;
        }
    }
        
    void IPInfo::setHostAddress(std::string const & host_name,
                                std::string const & service,
                                addrinfo * hints)
    {
        this->cleanAddress();
        
        auto res = getaddrinfo(host_name.data(), service.empty()?nullptr:service.data(),
                               hints, &mAddress);
        
        if(res != 0) mAddress = nullptr;
    }
    
    void IPInfo::setLocalAddress(std::string const & service, addrinfo * hints)
    {
        this->cleanAddress();
        char buf[128];
        if(gethostname(buf, sizeof(buf)) != 0) mAddress = nullptr;
        else setHostAddress(buf, service, hints);
    }
    
    SocketFamily IPInfo::getFamily() const
    {
        if(mAddress == nullptr) return SocketFamily::unspec;
        return (SocketFamily)mAddress->ai_family;
    }
        
    bool IPInfo::isIPv6() const
    {
        return mAddress && mAddress->ai_family == (int)SocketFamily::inet6;
    }
    bool IPInfo::isIPv4() const
    {
        return mAddress && mAddress->ai_family == (int)SocketFamily::inet;
    }
    
    std::string IPInfo::GetAddressString(addrinfo * addr)
    {
        if(addr == nullptr) return "";
        static const int max_len = 256;
        char buf[256];
        auto sa = addr->ai_addr;
        std::string res;
        switch(sa->sa_family) {
            case AF_INET:
                res = inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),
                          buf, max_len);
                break;
                
            case AF_INET6:
                res = inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
                          buf, max_len);
                break;
        }
        return res;
    }
    
	std::string GetStringByIp(uint32_t ip)
	{
		in_addr add;
		add.s_addr = ip;
		return inet_ntoa(add);
	}
	uint32_t GetIpByName(const char * name)
	{
		hostent* pHostent = gethostbyname(name);
		if(pHostent == nullptr) return INADDR_NONE;
		uint32_t host = *(uint32_t*)pHostent->h_addr_list[0];
		return host;
	}
}
