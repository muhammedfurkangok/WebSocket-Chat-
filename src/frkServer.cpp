#include "frkServer.h"

namespace WebSocket {

    frkServer::frkServer(int port, int connectionSize) {
        m_socketFd = socket(AF_INET, SOCK_STREAM, 0);
        if (m_socketFd < 0) {
            throw std::runtime_error("Socket oluşturulamadı.");
        }

        memset(&m_serverAddr, 0, sizeof(m_serverAddr));
        m_serverAddr.sin_family = AF_INET;
        m_serverAddr.sin_addr.s_addr = INADDR_ANY;
        m_serverAddr.sin_port = htons(port);

        if (bind(m_socketFd, (struct sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) < 0) {
            throw std::runtime_error("Socket bağlama başarısız.");
        }

        if (listen(m_socketFd, connectionSize) < 0) {
            throw std::runtime_error("Dinleme başarısız.");
        }

        m_running = true;
        std::cout << "Server başlatıldı. Port: " << port << "\n";
    }

    frkServer::~frkServer() {
        stopServer();
        closesocket(m_socketFd);
    }

    void frkServer::runServer() {
        while (m_running) {
            sockaddr_in clientAddr;
            socklen_t clientLen = sizeof(clientAddr);
            int clientSocket = accept(m_socketFd, (struct sockaddr*)&clientAddr, &clientLen);
            if (clientSocket < 0) {
                std::cerr << "Client kabul edilemedi.\n";
                continue;
            }

            std::cout << "Yeni bir istemci bağlandı.\n";
            m_clientThreads.emplace_back(&frkServer::handleClient, this, clientSocket);
        }
    }

    void frkServer::stopServer() {
        m_running = false;
        for (auto& thread : m_clientThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    void frkServer::handleClient(int clientSocket) {
        char buffer[1024];
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesRead <= 0) {
                std::cout << "İstemci bağlantısı kesildi.\n";
                closesocket(clientSocket);
                break;
            }
            std::cout << "İstemciden mesaj alındı: " << buffer << "\n";
            send(clientSocket, buffer, bytesRead, 0); // Echo mesajı
        }
    }

} // namespace WebSocket