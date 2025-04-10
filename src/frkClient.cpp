#include "frkClient.h"
#include <string>


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
			throw std::runtime_error("Server'a bağlanılamadı");
		}
	}
	void frkClient::recieveMessage()
	{
		// Winsock kullanımı için thread oluşturma
		this->m_thread = std::move(std::thread([this] {
			int len;
			const int bufferSize = this->getMessageSize(); // Mesaj boyutunu bir değişkene al
			char* message_buffer = new char[bufferSize];  // Dinamik olarak bellek ayır

			while (this->m_state == STATE::OPEN) {
				// Mesaj alımı
				len = recv(this->m_socketFd, message_buffer, bufferSize, 0);
				if (len > 0) {
					message_buffer[len] = '\0';  // Null-termination
					this->showMessage(message_buffer);
				}
				else if (len == 0) {
					// Bağlantı kapatıldı
					this->m_state = STATE::CLOSED;
					break;
				}
				else {
					// Hata oluştu
					std::cerr << "Error receiving message: " << WSAGetLastError() << std::endl;
					this->m_state = STATE::CLOSED;
					break;
				}
			}

			delete[] message_buffer;  // Dinamik belleği serbest bırak
			}));
	}

	void frkClient::writeMessage()
	{
		std::string message = "";
		while (this->m_state == STATE::OPEN)
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
		frkNodeV4(t_port)
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
		showMessage("Bağlantı başarılı");
		this->recieveMessage();
		this->writeMessage();
	}

	void frkClient::closeConnection()
	{
		closesocket(this->m_socketFd);
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