// SERVER SIDE 

#include <thread>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

int main() {
    // 1) create server socket
    // AF_INET (IPV4 Protocol), SOCK_STREAM (TCP socket)
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // 2) define server address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080); // htons() converts port to network byte order
    serverAddress.sin_addr.s_addr = INADDR_ANY; // accept any connections on any IP


    // 3) bind the socket to the address
    if (::bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("bind failed");
        return 1;
    }

    // 4) listen for connections
    if (listen(serverSocket, 5) == -1) { // second number is the number that can be on the wailist
        perror("listen failed");
        return 1;
    }
    // 5) accept client connection
    int clientSocket = accept(serverSocket, nullptr, nullptr);

    // 6) receive data from client
    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0); // reads data from clientSocket into buffer
    cout << "Message from client: " << buffer << endl;

    // 7) close socket
    close(serverSocket);

    return 0;
}