#ifndef FRKSERVER_H
#define FRKSERVER_H

#ifdef _WIN32
#include <winsock2.h> // Windows için socket API
#include <windows.h>
#else
#include <netinet/in.h> // POSIX için
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
		// Varsayýlan ayarlarý yapýlandýrýr
		void setDefault();

		// Mesaj olay fonksiyonu
		std::function<void(const std::string& t_message)> m_onMessage = nullptr;
		void showMessage(const std::string& t_message);

		// Thread yönetimi
		std::vector<std::thread> v_thread;
		void joinThread();
		std::mutex m_mu;

		// Client yönetimi
		int m_nodeSize = 0;
		std::vector<frkNodeV4*> v_client;
		void closeClient();

		// Server yönetimi
		int m_connectionSize = 128; // Maksimum baðlantý sayýsý
		void createServer(); // Server oluþturur
		void bindServer(); // Server'ý baðlar
		void listenServer(); // Server'ý dinler
		void handleServer(); // Baðlantýlarý ele alýr

		// Mesaj gönderme
		void sendMessageClients(const std::string& t_message); // Tüm istemcilere mesaj gönderir
		void sendMessageAll(const std::string& t_message, const int& t_socket); // Belirli bir istemci hariç herkese mesaj gönderir

	public:
		// Constructor'lar
		frkServer();
		frkServer(const int& t_port);
		frkServer(const int& t_port, const int& t_connectionSize);

		// Mesaj olay iþleyicisini ayarlar
		void setOnMessage(const std::function<void(const std::string&)>& t_function);

		// Server'ý çalýþtýrýr
		void runServer();

		// Server'ý kapatýr
		void closeServer();

		// Destructor
		virtual ~frkServer();
	};

} // namespace WebSocket

#endif // FRKSERVER_H