// CLIENT SIDE

#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

int main() {
    // 1) create socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("socket failed");
        return 1;
    }

    // 2) define server address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // 3) connect to server
    if (connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("connect failed");
        return 1;
    }
    // 4) send message
    const char* message = "Hello server!";
    send(clientSocket, message, strlen(message), 0);

    // 5) close client socket
    close(clientSocket);

    return 0;
}