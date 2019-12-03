#include "zen_socket.h"
#include "zen_log.h"
#include "zen_exception.h"
#include <cstring>
#if defined(ZEN_OS_WIN)
#pragma comment(lib,"ws2_32.lib")
#endif

namespace Zen {
	int GetError()
	{
		int eno = _GetSocketError();
		if(eno == EINTR || eno == EWOULDBLOCK || eno == EAGAIN)
		{
			return 0;
		}
		return eno;
	}

	Socket::Socket()
	{
	}
	Socket::Socket(SocketHandle sock, SocketConfig const & config)
	{
		this->mSocketID = sock;
		this->mConfig = config;
		this->mIsConnected = (sock != InvalidSocket);
		this->mErrorNo = 0;
	}

    
	Socket::~Socket()
	{
		if(mSocketID != InvalidSocket) this->close();
	}

    bool Socket::setTimeout(float second, SocketWay how)
    {
        if(mSocketID == InvalidSocket) return false;
        
        struct timeval timeout = {(int)second, (int)(second * 1000) % 1000 };
        switch (how) {
            case SocketWay::Read:
                return setsockopt(mSocketID, SOL_SOCKET, SO_RCVTIMEO,
                                  (char *)&timeout,sizeof(struct timeval)) == 0;
                
            case SocketWay::Write:
                return setsockopt(mSocketID, SOL_SOCKET, SO_SNDTIMEO,
                                  (char *)&timeout, sizeof(struct timeval)) == 0;
            default:
                return setsockopt(mSocketID, SOL_SOCKET, SO_RCVTIMEO,
                                  (char *)&timeout,sizeof(struct timeval)) == 0
                && setsockopt(mSocketID, SOL_SOCKET, SO_SNDTIMEO,
                              (char *)&timeout, sizeof(struct timeval)) == 0;
        }
    }

    SocketHandle Socket::handle() const
    {
        return mSocketID;
    }

	SocketConfig const & Socket::getConfig() const
	{
		return this->mConfig;
	}

	void Socket::setConfig(SocketConfig const & config)
	{
		this->mConfig = config;
	}
    
    bool Socket::open()
	{
		if(mSocketID != InvalidSocket) this->close();

		this->mErrorNo = 0;
		this->mIsConnected = false;
		
		mSocketID = ::socket((int)mConfig.family, (int)mConfig.type, (int)mConfig.ipproto);
		if(mSocketID == InvalidSocket)
		{
			Zen::LogE("error when create socket");
			return false;
		}
		return true;
	}
	bool Socket::close()
	{
        mIsConnected = false;
		if(mSocketID == InvalidSocket) return true;
		if(_CloseSocket(mSocketID) != 0)
		{
			int eno = _GetSocketError();
			Zen::LogE("error for close socket, e:%d", eno);
			return false;
		}
		mSocketID = InvalidSocket;
		return true;
	}
	bool Socket::shutdown(SocketWay how)
	{
		if(::shutdown(mSocketID, (int)how) == -1)
		{
			int eno = _GetSocketError();
			Zen::LogE("error for shutdown socket, e:%d", eno);
			return false;
		}
		return true;
	}
	SocketHandle Socket::native() const
	{
		return mSocketID;
	}
	bool Socket::isValid() const
	{
		return mSocketID != InvalidSocket;
	}
	bool Socket::send_out_band(char c)
	{
		return ::send(mSocketID, &c, 1, MSG_OOB) == 1;
	}
	bool Socket::recv_out_band(char &c)
	{
		return ::recv(mSocketID, &c, 1, MSG_OOB) == 1;
	}

	size_t Socket::recv(char * buf, size_t size, SocketMsgType way)
	{
		if(!mIsConnected) return 0;
		if(size == 0) return 0;

        long r = (long)::recv(mSocketID, buf, size, (int)way);
		if(r > 0) return r;
		if(r == 0)
		{
			this->close();
			return 0;
		}
		auto error = GetError();
		if(!error) return 0;

		this->mErrorNo = error;
		this->close();
		return 0;
	}
	size_t Socket::send(char const *buf, size_t size, SocketMsgType way )
	{
		if(!mIsConnected) return 0;
		if(size == 0) return 0;
		long r = (long)::send(mSocketID, buf, size, (int)way);
		if(r > 0) return r;
		if(r == 0)
		{
			this->close();
			return 0;
		}
		auto error = GetError();
		if(!error) return 0; // GOON

		this->mErrorNo = error;
		this->close();
		return 0;
	}
	bool Socket::setNonBlock(bool flag)
	{
		u_long arg(flag?1:0);
		return _SocketCtrl(mSocketID, FIONBIO, &arg) == 0;
	}
	bool Socket::setReuseable(bool flag)
	{
		int use = flag?1:0;
		return _SetSocketOption(mSocketID, SOL_SOCKET, SO_REUSEADDR, &use, sizeof(int)) == 0;
	}
	size_t Socket::couldReadBytes()
	{
		unsigned long arg(0);
		if(_SocketCtrl(mSocketID, FIONREAD, &arg) == 0) return arg;
		return 0;
	}
	bool Socket::isConnected()
	{
		return mIsConnected;
	}
	SocketConnector::SocketConnector()
	{
	}

	std::shared_ptr<SocketConnector> SocketConnector::Create()
	{
		return std::shared_ptr<SocketConnector>(new SocketConnector());
	}
    bool SocketConnector::connect(SocketAddress const & ip)
    {
        mIsConnected = false;
        mErrorNo = 0;
        
        if(mSocketID == InvalidSocket)
        {
			if(!this->open()) return false;
        }
		if(::connect(mSocketID, (struct sockaddr *)&ip.a, ip.a.sa_len) != 0)
        {
			mErrorNo = _GetSocketError();
            return false;
        }
        mIsConnected = true;
        return true;
    }
	bool SocketConnector::connect(IPInfo const & address, uint16_t port)
	{
        mIsConnected = false;
        mErrorNo = 0;
        
        if(mSocketID == InvalidSocket)
        {
			if(!this->open()) return false;
        }
        addrinfo * info = address.getAddressInfo();
        for (; info != nullptr; info = info->ai_next)
        {
            if(info->ai_family == (int)mConfig.family) break;
        }
        if(info == nullptr)
        {
            return false;
        }
        
        int res = 0;
        if(mConfig.family == SocketFamily::inet6)
        {
            sockaddr_in6 addr;
            musts(info->ai_addrlen == sizeof(addr), "system error");
            ::memcpy(&addr, info->ai_addr, sizeof(addr));
            addr.sin6_port = htons(port);
            res = ::connect(mSocketID, (sockaddr*)&addr, sizeof(addr));
        }
        else if(mConfig.family == SocketFamily::inet)
        {
            sockaddr_in addr;
            musts(info->ai_addrlen == sizeof(addr), "system error");
            ::memcpy(&addr, info->ai_addr, sizeof(addr));
            addr.sin_port = htons(port);
            res = ::connect(mSocketID, (sockaddr*)&addr, sizeof(addr));
        }
        
        if(res != 0)
        {
			this->mErrorNo = _GetSocketError();
            return false;
        }
        mIsConnected = true;
        return true;
	}
	SocketListener::SocketListener()
	{
	}

	std::shared_ptr<SocketListener> SocketListener::Create()
	{
		return std::shared_ptr<SocketListener>(new SocketListener());
	}

	bool SocketListener::listen(int backlog)
	{
		this->mErrorNo = 0;
		if(::listen(mSocketID, backlog) == 0) return true;
		this->mErrorNo = _GetSocketError();
		return false;
	}
	
	std::shared_ptr<Socket> SocketListener::accept()
	{
		if(!isValid()) return nullptr;

        SocketHandle acc = ::accept(mSocketID, 0, 0);

		if(acc == InvalidSocket) return nullptr;
		return std::shared_ptr<Socket>(new Socket(acc, mConfig));
	}

    bool SocketListener::bind(IPInfo const & address, uint16_t port)
	{
        mIsConnected = false;
        
        if(mSocketID == InvalidSocket)
        {
            Zen::LogE("invalid socket for bind");
            return false;
        }
        addrinfo * info = address.getAddressInfo();
        for (; info != nullptr; info = info->ai_next)
        {
            if(info->ai_family == (int)mConfig.family) break;
        }
        if(info == nullptr)
        {
            Zen::LogE("cannot find a valid server address");
            return false;
        }
        
        int res = 0;
        if(mConfig.family == SocketFamily::inet6)
        {
            sockaddr_in6 addr;
            musts(info->ai_addrlen == sizeof(addr), "system error");
            ::memcpy(&addr, info->ai_addr, sizeof(addr));
            addr.sin6_port = htons(port);
            res = ::bind(mSocketID, (sockaddr*)&addr, sizeof(addr));
        }
        else if(mConfig.family == SocketFamily::inet)
        {
            sockaddr_in addr;
            musts(info->ai_addrlen == sizeof(addr), "system error");
            ::memcpy(&addr, info->ai_addr, sizeof(addr));
            addr.sin_port = htons(port);
            res = ::bind(mSocketID, (sockaddr*)&addr, sizeof(addr));
        }
        
        if(res != 0)
        {
            Zen::LogE("failed to bind to %s %d (e:%d)",
                            IPInfo::GetAddressString(info).data(), (int)port,
                            _GetSocketError());
            return false;
        }
        mIsConnected = true;
        return true;
	}
}
