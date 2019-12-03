#pragma once
#include "zen_ip.h"
#include <memory>

namespace Zen {
	class SocketConnector;

	class SocketListener;

	class Socket
	{
		friend class SocketListener;

		friend class SocketConnector;

	protected:
		SocketHandle mSocketID = InvalidSocket;

		SocketConfig mConfig;

        int mErrorNo = 0;

        bool mIsConnected = false;

		Socket(Socket const &);

		void operator = (Socket const &);

		Socket(SocketHandle sock, SocketConfig const & config);

	public:
		Socket();

		~Socket();
		
		void setConfig(SocketConfig const & config);

		bool open();

		bool close();

		bool shutdown(SocketWay how);

		bool setNonBlock(bool flag) ;

		bool setReuseable(bool flag);
        
        bool setTimeout(float second, SocketWay how);
		
		bool send_out_band(char c) ;

		bool recv_out_band(char & c);

//		return 0: no data; >0: data size
		size_t recv(char * buf, size_t size, SocketMsgType way = SocketMsgType::Default);

		size_t send(char const *buf, size_t size, SocketMsgType way = SocketMsgType::Default);

		size_t couldReadBytes() ;

		bool isConnected();
		
		SocketHandle native() const;

		bool isValid() const;

		SocketHandle handle() const;

		SocketConfig const & getConfig() const;
	};

	class SocketConnector : public Socket
	{
	protected:
		SocketConnector(SocketConnector const &);

		void operator = (SocketConnector const &);

		SocketConnector();

	public:
		static std::shared_ptr<SocketConnector> Create();

		bool connect(SocketAddress const & address);
        
        bool connect(IPInfo const & address, uint16_t port);
	};

	class SocketListener : public Socket
	{
	protected:
		SocketListener(SocketListener const &);

		void operator =(SocketListener const &);

		SocketListener();
	public:
		static std::shared_ptr<SocketListener> Create();

        using Socket::open;
        
        bool bind(IPInfo const & address, uint16_t port);
        
		bool listen(int backlog);

		/**
         return nullptr if failed.
         */
		std::shared_ptr<Socket> accept();

	};
}
