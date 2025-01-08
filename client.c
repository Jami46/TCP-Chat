#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    
    if(inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(1);
    }

    if(connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    while(1) {
        printf("Enter message: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(clientSocket, buffer, strlen(buffer), 0);
        
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if(bytesRead <= 0) break;
        buffer[bytesRead] = '\0';
        printf("Server: %s\n", buffer);
    }

    close(clientSocket);
    return 0;
}
