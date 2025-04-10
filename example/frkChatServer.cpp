#include <iostream>
#include <csignal>
#include "frkServer.h"

using namespace std;
using namespace WebSocket;

frkServer* chatServer = nullptr;

void signalHandler(int);
void printMessage(const string&);

int main() {
    signal(SIGINT, signalHandler);

    int port = 5000;
    int connectionSize = 16;

    cout << "Server port (Varsayılan: 5000): ";
    cin >> port;

    cout << "Maksimum bağlantı sayısı (Varsayılan: 16): ";
    cin >> connectionSize;

    try {
        chatServer = new frkServer(port, connectionSize);
        chatServer->setOnMessage(printMessage);
        chatServer->runServer();
    } catch (const exception& e) {
        cerr << "Hata: " << e.what() << endl;
    }

    delete chatServer;
    return 0;
}

void signalHandler(int) {
    cout << "\nServer kapatılıyor...\n";
    delete chatServer;
    exit(0);
}

void printMessage(const string& message) {
    cout << "İstemci: " << message << endl;
}