#include "zen_socket.h"
#include "zen_log.h"
#include "zen_exception.h"
#include <cstring>

#if defined(ZEN_OS_WIN)
#pragma comment(lib,"ws2_32.lib")
#endif

namespace Zen { namespace Socket {
	
	static inline int _GetSocketErrorNo()
	{
		int eno = _GetErrorNo();
		return (eno == EINTR || eno == EWOULDBLOCK || eno == EAGAIN)?0:eno;
	}
	static inline void _SetConfigFamily(Config & config, Address const & address)
	{
		if(address.a.sa_family == (int)Family::inet)
			config.family = Family::inet;
		else if(address.a.sa_family == (int)Family::inet6)
			config.family = Family::inet6;
		else
			config.family = Family::unspec;
	}
	
	Link::Link(Handle hd, Config const & config)
	{
		this->mHD = hd;
		this->mConfig = config;
	}
	Link::Link(Config const & config)
	{
		this->mConfig = config;
	}
	
	void Link::clear()
	{
		mStatus.setStatus(Status::OK);
	}
	Link::~Link()
	{
		this->close();
	}
	
	bool Link::setTimeout(float second, bool for_send, bool for_recv)
	{
		if(!isValidHandle()) return false;
		
		struct timeval timeout = {(int)second, (int)(second * 1000) % 1000 };
		
		bool s = true, r = true;
		if(for_send)
			s = (_SetOption(mHD, SOL_SOCKET, SO_SNDTIMEO, (void const *)&timeout, sizeof(timeout)) == 0);
		if(for_recv)
			r = (_SetOption(mHD, SOL_SOCKET, SO_RCVTIMEO, (void const *)&timeout, sizeof(timeout)) == 0);
		return s && r;
	}
	
	Handle Link::handle() const
	{
		return mHD;
	}
	
	Config const & Link::config() const
	{
		return this->mConfig;
	}
	
	bool Link::open()
	{
		if(mHD != SockNone) return false;
		
		mHD = ::socket((int)mConfig.family, (int)mConfig.type, (int)mConfig.ipp);
		if(mHD == SockNone)
		{
			int error = _GetErrorNo();
			Zen::LogE("socket:[%d] open e:%d", mHD, error);
			return false;
		}
		this->clear();
		return true;
	}
	bool Link::close()
	{
		if(!isValidHandle()) return true;
		
		if(_CloseSocket(mHD) != 0)
		{
			int error = _GetErrorNo();
			Zen::LogE("socket:[%d] close e:%d", mHD, error);
			return false;
		}
		mHD = SockNone;
		return true;
	}
	bool Link::shutdown(Shut how)
	{
		if(::shutdown(mHD, (int)how) == -1)
		{
			int error = _GetErrorNo();
			Zen::LogE("socket:[%d] shutdown e:%d", mHD, error);
			return false;
		}
		return true;
	}
	size_t Link::recv(void * buf, size_t size, MsgType way)
	{
		if(size == 0 || !isValidHandle() || mStatus.isRecvClosed())
			return 0;
				
		long r = (long)::recv(mHD, buf, size, (int)way);
		if(r > 0)
			return r;
		
		int error = 0;
		if(r == 0 || (error = _GetSocketErrorNo()))
		{
			if(error) Zen::LogE("socket:[%d] recv e:%d", mHD, error);
			
			mStatus.addStatus(Status::RecvClosed);
		}
		return 0;
	}
	size_t Link::send(void const *buf, size_t size, MsgType way)
	{
		if(size == 0 || !isValidHandle() || mStatus.isSendClosed())
			return 0;
		
		long r = (long)::send(mHD, buf, size, (int)way);
		if(r > 0)
			return r;
		
		int error = 0;
		if(r == 0 || (error = _GetSocketErrorNo()))
		{
			if(error) Zen::LogE("send error socket[%d], e:%d", mHD, error);
			
			mStatus.addStatus(Status::SendClosed);
		}
		return 0;
	}
	bool Link::setNonBlock(bool flag)
	{
		if(!isValidHandle()) return false;
		unsigned long arg(flag?1:0);
		return _IOCtrl(mHD, FIONBIO, &arg) == 0;
	}
	bool Link::setReuseable(bool flag)
	{
		if(!isValidHandle()) return false;
		int use = flag?1:0;
		return _SetOption(mHD, SOL_SOCKET, SO_REUSEADDR, &use, sizeof(int)) == 0;
	}
	size_t Link::canRecvSize()
	{
		if(!isValidHandle()) return 0;
		unsigned long arg(0);
		if(_IOCtrl(mHD, FIONREAD, &arg) == 0) return arg;
		return 0;
	}
	
	Link::Status Link::getStatus() const
	{
		return mStatus;
	}
	
	bool Link::isValidHandle() const
	{
		return mHD != SockNone;
	}
	
	Address Link::makeLocalAddress()
	{
		Address a;
		a.a.sa_family = AF_UNSPEC;
		socklen_t len = 0;
		auto r = ::getsockname(mHD, nullptr, &len);
		if(r || len > sizeof(a)) return a;
		
		r = ::getsockname(mHD, &a.a, &len);
		if(r) a.a.sa_family = AF_UNSPEC;
		return a;
	}
	   
	Address Link::makePeerAddress()
	{
		Address a;
		a.a.sa_family = AF_UNSPEC;
		socklen_t len = 0;
		auto r = ::getpeername(mHD, nullptr, &len);
		if(r || len > sizeof(a)) return a;
		
		r = ::getpeername(mHD, &a.a, &len);
		if(r) a.a.sa_family = AF_UNSPEC;
		return a;
	}

	TCPConnector::TCPConnector()
	{
		
	}
	TCPConnector::~TCPConnector()
	{
		
	}
	std::shared_ptr<TCPConnector> TCPConnector::Create()
	{
		return std::shared_ptr<TCPConnector>(new TCPConnector());
	}

	std::shared_ptr<Link> TCPConnector::connect(Address const & server_address)
	{
		Config config;
		config.type = SockType::stream;
		config.ipp = IPProto::ip;
		_SetConfigFamily(config, server_address);
		if(config.family == Family::unspec)
		{
			Zen::LogE("invalid socket address for connect");
			return nullptr;
		}
		auto link = std::shared_ptr<Link>(new Link(config));
		if(!link->open())
		{
			return nullptr;
		}
		auto r = ::connect(link->handle(), &server_address.a, (socklen_t)AddressLength(server_address));
		if(r == 0)
		{
			return link;
		}
		
		mENo = _GetErrorNo();
		return nullptr;
	}
	
	TCPListener::TCPListener()
	{
	}
	TCPListener::~TCPListener()
	{
		
	}
	
	std::shared_ptr<TCPListener> TCPListener::Create()
	{
		return std::shared_ptr<TCPListener>(new TCPListener());
	}
	
	bool TCPListener::listen(int backlog)
	{
		if(!mLink || !mLink->isValidHandle()) return false;
		
		if(::listen(mLink->handle(), backlog) == 0) return true;
		int error = _GetErrorNo();
		Zen::LogE("socket:[%d] listen e:%d", mLink->handle(), error);
		return false;
	}
	std::shared_ptr<Link> TCPListener::getLink()
	{
		return mLink;
	}

	bool TCPListener::setNonBlock(bool flag)
	{
		return mLink && mLink->setNonBlock(flag);
	}

	bool TCPListener::setReuseable(bool flag)
	{
		return mLink && mLink->setReuseable(flag);
	}
	
	std::shared_ptr<Link> TCPListener::accept()
	{
		if(!mLink || !mLink->isValidHandle()) return nullptr;
		
		Handle acc = ::accept(mLink->handle(), 0, 0);
		
		if(acc == SockNone)
		{
			auto error = _GetSocketErrorNo(); // accept
			if(error) Zen::LogE("socket[%d]: accept e:%d", mLink->handle(), error);
			return nullptr;
		}
		return std::shared_ptr<Link>(new Link(acc, mLink->config()));
	}
	
	bool TCPListener::bind(Address const & ip)
	{
		Config config;
		config.type = SockType::stream;
		config.ipp = IPProto::tcp;
		_SetConfigFamily(config, ip);
		
		mLink = std::shared_ptr<Link>(new Link(config));
		
		if(!mLink->open())
		{
			return false;
		}
		mLink->setReuseable(true);
		
		auto r = ::bind(mLink->handle(), &ip.a, (socklen_t)AddressLength(ip));
		if(r == 0) return true;
		
		auto error = _GetErrorNo(); // bind
		Zen::LogE("socket:[%d] bind e:%d", mLink->handle(), error);
		return false;
	}
}}
