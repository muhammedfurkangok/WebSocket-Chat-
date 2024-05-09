#include "frkClient.h"

namespace WebSocket {

	void frkClient::SetDefault()
	{
		inet_pton(AF_INET, this->m_serverName.c_str(), &this->m_socketInfo.sin_addr);
	}

	void frkClient::showMessage(const std::string& t_message)
	{
		if (this->m_onMessage != nullptr)
		{
			this->m_onMessage(t_message);
		}
	}

	void frkClient::_connectServer()
	{
		this->createSocket();

		if (connect(this->m_socketFd, reinterpret_cast<sockaddr*> (&this->m_socketInfo), this->m_adressLen) == -1)
		{
			throw std::runtime_error("Server'a baglanilmadi");
		}
	}

	void frkClient::recieveMessage()
	{
		this->m_thread = std::move(std::thread([=] {
			int len;
			char message_buffer[this->getMessageSize];

			while ((len = recv(this->m_socketFd, message_buffer, this->getMessageSize(), 0)) > 0) {
				message_buffer[len] = '\0';
				showMessage(message_buffer);
				std::memset(message_buffer, '\0', sizeof(message_buffer));
			}
			this->m_state = STATE::CLOSED;
			}));
	}

	void frkClient::writeMessage()
	{
		std::string message = "";
		while (this->m_state == STATE.OPEN)
		{
			std::getline(std::cin, message);
			this->sendMessage(this->m_socketFd, message);
		}
	}

	frkClient::frkClient() :
		frkNodeV4()
	{
		SetDefault();
	}

	frkClient::frkClient(const int& t_port) :
		frkClient(t_port)
	{
		SetDefault();
	}

	frkClient::frkClient(const std::string& t_serverName, const int& t_port) :
		frkNodeV4(t_port), m_serverName(t_serverName)
	{

		SetDefault();

	}
	void frkClient::setOnMessage(const std::function<void(const std::string&)>& t_function)
	{

		m_onMessage = t_function;

	}
	void frkClient::connectServer()
	{
		this->_connectServer();
		showMessage("Baglanti basarili");
		this->recieveMessage();
		this->writeMessage();
	}
	void frkClient::closeConnection()
	{
		this->closeSocket();
		if (this->m_thread.joinable())
		{
			this->m_thread.join();
		}
	}

	frkClient::~frkClient()
	{
		this->closeConnection();
	}

}