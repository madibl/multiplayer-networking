// SERVER SIDE 

#include <thread>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

int sharedCounter = 0;
mutex counterMtx;

void handleClient(int clientSocket) {
    // send data?
    char buffer[1024];

    while (true) {
        memset(buffer, 0, sizeof(buffer));

        int bytesRecieved = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

        if (bytesRecieved <= 0) {
            cout << "Client disconnected." << endl;
            break;
        }

        // cout << "Message from client: " << buffer << endl;

        string command(buffer);
        if (command.find("INC") != string::npos) {
            // lock it so no threads access at the same time
            lock_guard<mutex> lock(counterMtx);

            // intentionally unsafe incrementation of counter
            int temp = sharedCounter;
            this_thread::sleep_for(chrono::microseconds(1));
            temp++;
            sharedCounter = temp;

            string response = "Counter: " + to_string(sharedCounter) + "\n";
            send(clientSocket, response.c_str(), response.size(), 0);
        }
    }
    close (clientSocket);
}
 
int main() {
    // create server socket
    // AF_INET (IPV4 Protocol), SOCK_STREAM (TCP socket)
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("socket failed");
        return 1;
    }

    // set options for socket
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // define server address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080); // htons() converts port to network byte order
    serverAddress.sin_addr.s_addr = INADDR_ANY; // accept any connections on any IP


    // bind the socket to the address
    if (::bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("bind failed");
        return 1;
    }

    // listen for connections
    if (listen(serverSocket, 5) == -1) { // second number is the number that can be on the wailist
        perror("listen failed");
        return 1;
    }

    // open a thread 
    while(true) {
        // accept client connection
        int clientSocket = accept(serverSocket, nullptr, nullptr);

        if (clientSocket == -1) {
            perror("accept failed");
            continue;
        }
        
        cout << "New client connected." << endl;

        thread clientThread(handleClient, clientSocket);
        clientThread.detach();
    }

    // close socket
    close(serverSocket);

    return 0;
}