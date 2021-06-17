#pragma once

#ifdef ZEN_OS_WIN
#include <Windows.h>
#else
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <netdb.h> // gethostbyname
#include <sys/ioctl.h>
#include <string>
#endif

#include "zen_endian.h"
#include <vector>
#include <memory.h>

#if ZEN_OS_APPLE
#define ZEN_HAVE_SOCKADDR_SA_LEN 1
#endif

#define ZEN_INVALID_SOCKET INVALID_SOCKET
#define ZEN_SOCKET_ERROR SOCKET_ERROR

#ifdef ZEN_OS_WIN
#define MSG_DONTWAIT 0
#endif

namespace Zen { namespace Socket {
	
	typedef int Handle;
	
	static const Handle SockNone = -1;
	
	enum class Family
	{
		unspec = AF_UNSPEC,
		inet = AF_INET,
		inet6 = AF_INET6,
	};
	
	enum class SockType
	{
		unspec = 0,
		stream = SOCK_STREAM,
		datagram = SOCK_DGRAM,
		raw = SOCK_RAW,
	};
	
	enum class IPProto
	{
		ip   = IPPROTO_IP  ,
		icmp = IPPROTO_ICMP,
		igmp = IPPROTO_IGMP,
		tcp  = IPPROTO_TCP ,
		pup  = IPPROTO_PUP ,
		udp  = IPPROTO_UDP ,
		idp  = IPPROTO_IDP ,
		raw  = IPPROTO_RAW ,
		max  = IPPROTO_MAX ,
	};
	
	enum class Shut
	{
#ifdef SHUT_RD
		RD   = SHUT_RD,
		WR   = SHUT_WR,
		RDWR = SHUT_RDWR,
#else
		RD   = 0,
		WR   = 1,
		RDWR = 2,
#endif
	};
	
	enum class MsgType
	{
		Default = 0,
		OutBand = MSG_OOB,
		Peek = MSG_PEEK,
		NoRoute = MSG_DONTROUTE,
		NoWait = MSG_DONTWAIT,
	};
	
	template<typename Option, typename Arg>
	inline int _IOCtrl(Handle sock, Option cmd, Arg arg)
	{
#ifdef ZEN_OS_WIN
		return ::ioctlsocket(sock, cmd, arg);
#else
		return ::ioctl(sock, cmd, arg);
#endif
	}
	
	inline int _SetOption(Handle socket, int level, int optname, const void *optval, size_t optlen)
	{
#ifdef ZEN_OS_WIN
		return ::setsockopt(socket, level, optname, (const char *)optval, (int)optlen);
#else
		return ::setsockopt(socket, level, optname, optval, (socklen_t)optlen);
#endif
	}
	
	inline int _CloseSocket(Handle s)
	{
#ifdef ZEN_OS_WIN
		return ::closesocket(s);
#else
		return ::close(s);
#endif
	}
	
	inline int _GetErrorNo()
	{
#ifdef ZEN_OS_WIN
		return WSAGetLastError();
#else
		return errno;
#endif
	}
	
	enum class Signals : int {
		interrupt = SIGINT,
		hangup = SIGHUP,
		pipe = SIGPIPE,
		io = SIGIO,
	};
//		void ignoreSignals()
 //		{
 //#ifndef ZEN_OS_WIN
// 			signal(SIGINT, SIG_IGN);
 //			signal(SIGHUP, SIG_IGN);
 //			signal(SIGPIPE, SIG_IGN);
 //#endif
 //		}
 //
 //		void setSignalHandler(int sig, void (*handle)(int))
 //		{
 //#ifndef ZEN_OS_WIN
 //			struct sigaction action;
 //			action.sa_handler = handle;
 //			sigemptyset(&action.sa_mask);
 //			action.sa_flags = 0;
 //			sigaction(sig, &action, NULL);
 //			signal(sig, handle);
 //#endif
 //		}
	
}}

namespace Zen { namespace Socket {
	
	struct Config {
		Family family = Family::inet;
		SockType type = SockType::stream;
		IPProto ipp = IPProto::ip;
	};
	
	/// Address begin
	union Address {
		sockaddr a;
		sockaddr_in v4;
		sockaddr_in6 v6;
	};
	
	inline size_t AddressLength(Address const & a)
	{
#if ZEN_HAVE_SOCKADDR_SA_LEN
		return a.a.sa_len;
#else
		if(a.a.sa_family == AF_INET) return sizeof(a.v4);
		if(a.a.sa_family == AF_INET6) return sizeof(a.v6);
		return 0;
#endif
	}
	inline void AddressSetPort(Address & address, int port)
	{
		address.v4.sin_port = htons((in_port_t)port);
	}
	
	inline int AddressGetPort(Address const & address)
	{
		return (int)ntohs(address.v4.sin_port); // v6 same
	}
	
	inline Address AddressMake(uint8_t ip0, uint8_t ip1, uint8_t ip2, uint8_t ip3, int port)
	{
		Address res;
		::memset(&res, 0, sizeof(res));
		res.v4.sin_family = (sa_family_t)Family::inet;
#if ZEN_HAVE_SOCKADDR_SA_LEN
		res.v4.sin_len = sizeof(sockaddr_in);
#endif
		res.v4.sin_port = htons((in_port_t)port);
		res.v4.sin_addr.s_addr = Zen::Byte4(ip0, ip1, ip2, ip3).value;
		return res;
	}
	
	inline Address AddressMake(std::string const & host_name, int port, Family family = Family::inet)
	{
		Address ip;
		ip.a.sa_family = (int)Family::unspec;
#if ZEN_HAVE_SOCKADDR_SA_LEN
		ip.a.sa_len = 0;
#endif
		addrinfo * info;
		if(getaddrinfo(host_name.data(), nullptr, nullptr, &info) == 0)
		{
			std::vector<Address> v;
			for(;info; info = info->ai_next)
			{
				if(info->ai_family != (int)family) continue;

				::memcpy(&ip.a, info->ai_addr, info->ai_addrlen);
				ip.v4.sin_port = htons((in_port_t)port); // v6 same
				break;
			}
		}
		return ip;
	}
	inline std::string AddressGetIP(Address const & address)
	{
		static const int max_len = 256;
		char buf[256];
		if(address.a.sa_family == AF_INET)
		{
			auto r = inet_ntop(AF_INET, &address.v4.sin_addr, buf, max_len);
			if(r) return r;
		}
		else if(address.a.sa_family == AF_INET6)
		{
			auto r = inet_ntop(AF_INET6, &address.v6.sin6_addr, buf, max_len);
			if(r) return r;
		}
		return "";
	}
	inline std::string AddressToString(Address const & address)
	{
		static const int max_len = 256;
		char buf[256];
		std::string res;
		auto port = std::to_string(AddressGetPort(address));
		if(address.a.sa_family == AF_INET)
		{
			auto r = inet_ntop(AF_INET, &address.v4.sin_addr, buf, max_len);
			if(r) return std::string(r) + ":" + port;
		}
		else if(address.a.sa_family == AF_INET6)
		{
			auto r = inet_ntop(AF_INET6, &address.v6.sin6_addr, buf, max_len);
			if(r) return "[" + std::string(r) + "]:" + port;
		}
		return "";
	}
	/// Address end
}}
