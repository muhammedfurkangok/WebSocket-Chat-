#ifndef FRKSERVER_H
#define FRKSERVER_H



#include "frkNodeV4.h"
#include <thread>
#include <mutex>
#include <vector>
#include <functional>
#include <algorithm>


namespace WebSocket {

	class frkServer : protected frkNodeV4 {
	protected:
		void setDefault();

		//message event function
		std::function<void(const std::string& t_message) > m_onMessage = nullptr;
		void showMessage(const std::string& t_message);

		//thread
		std::vector<std::thread> v_thread;
		void joinThread();
		std::mutex m_mu;


		//client
		int m_nodeSize = 0;
		std::vector<frkNodeV4*> v_client;
		void closeClient();

		//server
		int m_connectionSize = 128;
		void createServer();
		void bindServer();
		void listenServer();
		void handleServer();


		//message
		void sendMessageClients(const std::string& t_messsage);
		void sendMessageAll(const std::string& t_message, const int& t_socket);

	public:
		//constructors
		frkServer();
		frkServer(const int& t_port);
		frkServer(const int& t_port,const int& t_connectionSize);

		void setOnMessage(const std::function<void(const std::string&) >& t_function);

		void runServer();
		void closeServer();

		//destructor
		virtual ~frkServer();



	};


}


#endif // !FRKSERVER_H
