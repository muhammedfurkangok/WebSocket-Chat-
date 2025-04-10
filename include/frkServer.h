#ifndef FRKSERVER_H
#define FRKSERVER_H

#ifdef _WIN32
#include <winsock2.h> // Windows i�in socket API
#include <windows.h>
#else
#include <netinet/in.h> // POSIX i�in
#include <unistd.h>
#endif

#include "frkNodeV4.h"
#include <thread>
#include <mutex>
#include <vector>
#include <functional>
#include <algorithm>

namespace WebSocket {

	class frkServer : protected frkNodeV4 {
	protected:
		// Varsay�lan ayarlar� yap�land�r�r
		void setDefault();

		// Mesaj olay fonksiyonu
		std::function<void(const std::string& t_message)> m_onMessage = nullptr;
		void showMessage(const std::string& t_message);

		// Thread y�netimi
		std::vector<std::thread> v_thread;
		void joinThread();
		std::mutex m_mu;

		// Client y�netimi
		int m_nodeSize = 0;
		std::vector<frkNodeV4*> v_client;
		void closeClient();

		// Server y�netimi
		int m_connectionSize = 128; // Maksimum ba�lant� say�s�
		void createServer(); // Server olu�turur
		void bindServer(); // Server'� ba�lar
		void listenServer(); // Server'� dinler
		void handleServer(); // Ba�lant�lar� ele al�r

		// Mesaj g�nderme
		void sendMessageClients(const std::string& t_message); // T�m istemcilere mesaj g�nderir
		void sendMessageAll(const std::string& t_message, const int& t_socket); // Belirli bir istemci hari� herkese mesaj g�nderir

	public:
		// Constructor'lar
		frkServer();
		frkServer(const int& t_port);
		frkServer(const int& t_port, const int& t_connectionSize);

		// Mesaj olay i�leyicisini ayarlar
		void setOnMessage(const std::function<void(const std::string&)>& t_function);

		// Server'� �al��t�r�r
		void runServer();

		// Server'� kapat�r
		void closeServer();

		// Destructor
		virtual ~frkServer();
	};

} // namespace WebSocket

#endif // FRKSERVER_H