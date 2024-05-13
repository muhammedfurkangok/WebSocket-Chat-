#ifndef NODEV4_H
#define NODEV4_H

#include <cstring>
#include <stdexcept> //exception

// cross platform
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
typedef int socklen_t;
static WSAData wsaData;
#else
//android
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif



namespace WebSocket {
	enum class STATE {
		OPEN,
		CLOSED,
	};



class frkNodeV4 {
protected :

	//Socket
	int m_socketFd = -1; // (file descriptor)
	struct sockaddr_in m_socketInfo; //  IPv4 adresleme
	int m_port = 5000; //  port numarasýn
	int m_adressLen = 0; //adres uzunlugu
	int m_messageSize = 4096; //maks boyut
	STATE m_state; //chat durumu

	//socket 

	void setSocketInfo();
	void createSocket();
	void sendMessage(const int& t_socketfd, const std::string& t_message);
	void closeSocket();


public:
	//constructors
	frkNodeV4();
	frkNodeV4(const int& t_port);

	//Port
	int getPort() const;
	void setPort(const int& t_port);

	sockaddr_in getSocketInfo() const;
	void setSocketInfo(const sockaddr_in& t_socketInfo);

	//Socket file descriptor
	int getSocketFd() const;
	void SetSocketFd(const int& t_socketFd);

	//Connection status
	STATE getState() const;
	void setState(const STATE& t_state);

	//Message Size
	int getMessageSize() const;
	void SetMessageSize(const int& t_messageSize);

	//destructor
	virtual  ~frkNodeV4();
};

}
#endif // !NODEV4_H