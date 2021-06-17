#pragma once
#include "zen_ip.h"
#include <memory>

namespace Zen { namespace Socket {
	
	class Link
	{
	public:
		struct Status {
			typedef uint8_t Type;
			
			static const Type OK = 0;
			static const Type SendClosed = 1;
			static const Type RecvClosed = 2;
			
			Type value = OK;
			
			bool isSendClosed() const { return (value & SendClosed); }
			bool isRecvClosed() const { return (value & RecvClosed); }
			bool isGood() const { return value == OK; }
			void addStatus(Type i) { value |= i; }
			void removeStatus(Type i) { value &= ~i; }
			void setStatus(Type i) { value = i; }
			operator bool() const { return isGood(); }
		};
	protected:
		Handle mHD = SockNone;

		Config mConfig;

		Status mStatus;
		
	public:

		Link(Link const &) = delete;

		void operator = (Link const &) = delete;

		Link(Handle hd, Config const & config);

		Link(Config const & config);
	public:
		virtual ~Link();

		bool open();

		bool close();

		bool shutdown(Shut how);

		bool setNonBlock(bool flag);

		bool setReuseable(bool flag);
        
        bool setTimeout(float second, bool for_send, bool for_recv);

//		return 0: no data; >0: data size
		size_t recv(void * buf, size_t size, MsgType way = MsgType::Default);

		size_t send(void const * buf, size_t size,  MsgType way = MsgType::Default);

		size_t canRecvSize() ;

		Status getStatus() const;

		bool isValidHandle() const;
		
		void clear();
		
		Handle handle() const;
		
		Config const & config() const;
		
		Address makeLocalAddress();
		
		Address makePeerAddress();
	};

	class TCPConnector
	{
	protected:
		Handle mTemp = SockNone;
		int mENo = 0;
	protected:
		TCPConnector(TCPConnector &) = delete;

		void operator = (TCPConnector &) = delete;

		TCPConnector();
		
	public:
		virtual ~TCPConnector();
		
		static std::shared_ptr<TCPConnector> Create();

		std::shared_ptr<Link> connect(Address const & server_address);
	};

	class TCPListener
	{
	protected:
		std::shared_ptr<Link> mLink;
		
	protected:
		
		TCPListener(TCPListener const &) = delete;

		void operator = (TCPListener const &) = delete;

		TCPListener();
		
	public:
		virtual ~TCPListener();
		
		static std::shared_ptr<TCPListener> Create();
		
		void reset();
		
        bool bind(Address const & listen_address);
        
		bool setNonBlock(bool flag);

		bool setReuseable(bool flag);
		
		bool listen(int backlog);
		
		std::shared_ptr<Link> getLink();
		/**
         return nullptr if failed.
         */
		std::shared_ptr<Link> accept();

	};
	
	/*
	 @ SocketStartup
	 - for windows/unix differ
	 */
	class SocketStartup
	{
	private:
		SocketStartup(SocketStartup const &) = delete;
		void operator = (SocketStartup const &) = delete;
	public:
		SocketStartup(int version = 0x0202)
		{
#ifdef ZEN_OS_WIN
			WSADATA wsadata;
			WSAStartup(version, &wsadata);
#else
			signal(SIGPIPE, SIG_IGN);
#endif
		}
		
		~SocketStartup()
		{
#ifdef ZEN_OS_WIN
			WSACleanup();
#endif
		}
	};
}}
