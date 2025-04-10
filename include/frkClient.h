#ifndef FRK_CLIENT_H
#define FRK_CLIENT_H

#include "frkNodeV4.h"
#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <winsock2.h> // Windows için socket API
#include <windows.h>
#include <string>
#include <cstring>

namespace WebSocket {
	class frkClient : protected frkNodeV4 {
	protected:

		void SetDefault();

		// mesaj event fonksiyonu
		std::function<void(const std::string& t_message)> m_onMessage = nullptr;
		void showMessage(const std::string& t_message);

		// server
		std::string m_serverName = "localhost";

		// thread
		std::thread m_thread;
		std::mutex m_mu;

		void _connectServer();
		void recieveMessage();
		void writeMessage();

	public:
		// constructors
		frkClient();
		frkClient(const int& t_port);
		frkClient(const std::string& t_serverName, const int& t_port);

		void setOnMessage(const std::function<void(const std::string&)>& t_function);

		void connectServer();
		void closeConnection();

		// destructor
		virtual ~frkClient();

	};
}

#endif // !FRK_CLIENT_H