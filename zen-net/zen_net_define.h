
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
//#include <sys/proc.h>
#endif

#define ZEN_INVALID_SOCKET INVALID_SOCKET
#define ZEN_SOCKET_ERROR SOCKET_ERROR

#ifdef ZEN_OS_WIN
#define MSG_DONTWAIT 0
#endif

namespace Zen {
	
	typedef int SocketHandle;
    
	static const SocketHandle InvalidSocket = INADDR_NONE;
	
	enum class SocketFamily
	{
		unspec = AF_UNSPEC,
		inet = AF_INET,
		inet6 = AF_INET6,
	};

	enum class SocketType
	{
		raw = SOCK_RAW,
		stream = SOCK_STREAM,
		datagram = SOCK_DGRAM,
	};

	enum class SocketProtocol
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

	enum class SocketWay
	{
		Read  = 0, //SHUT_RD,
		Write = 1, //SHUT_WR,
		Both  = 2, //SHUT_R&W,
	};
    
	enum class SocketMsgType
	{
		Default = 0,
        OutBand = MSG_OOB,
        Peek = MSG_PEEK,
        NoRoute = MSG_DONTROUTE,
        NoWait = MSG_DONTWAIT,
	};

    /*
     @ SocketLibrary
     - create an instance before using any socket funtions.
     and destroy it after all socket tasks finished.
     */
	class SocketLibrary
	{
	private:
		SocketLibrary(SocketLibrary const &) = delete;
		void operator = (SocketLibrary const &) = delete;
	public:
		SocketLibrary(int16_t version = 0x0202)
		{
#ifdef ZEN_OS_WIN
			WSADATA wsadata;
			WSAStartup(version, &wsadata);
#endif
		}
#ifndef ZEN_OS_WIN
        void ignoreSignals()
        {
			signal(SIGINT, SIG_IGN);
			signal(SIGHUP, SIG_IGN);
			signal(SIGPIPE, SIG_IGN);
        }
        void setSignalHandler(int sig, void (*handle)(int))
        {
            struct sigaction action;
            action.sa_handler = handle;
            sigemptyset(&action.sa_mask);
            action.sa_flags = 0;
            sigaction(sig, &action, NULL);
            signal(sig, handle);
        }
#endif

		~SocketLibrary()
		{
#ifdef ZEN_OS_WIN
			WSACleanup();
#endif
		}
	};
	template<typename Option, typename Arg>
	inline int _SocketCtrl(SocketHandle sock, Option cmd, Arg arg)
	{
#ifdef ZEN_OS_WIN
		return ::ioctlsocket(sock, cmd, arg);
#else
		return ::ioctl(sock, cmd, arg);
#endif
	}
	
	inline int _SetSocketOption(SocketHandle socket, int level, int optname, const void *optval, size_t optlen)
	{
#ifdef ZEN_OS_WIN
		return ::setsockopt(socket, level, optname, (const char *)optval, (int)optlen);
#else
		return ::setsockopt(socket, level, optname, optval, (socklen_t)optlen);
#endif
	}
	inline int _CloseSocket(SocketHandle s)
	{
#ifdef ZEN_OS_WIN
		return ::closesocket(s);
#else
		return ::close(s);
#endif
	}
	inline int _GetSocketError()
	{
#ifdef ZEN_OS_WIN
		return WSAGetLastError();
#else
		return errno;
#endif
	}
}
