#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>  // Winsock library for Windows

#pragma comment(lib, "ws2_32.lib")  // Link with Winsock library

#define PORT 8081
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Initialize Winsock
    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Winsock initialized.\n");

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    printf("Socket created.\n");

    // Setup the sockaddr_in structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        printf("Connect failed with error code: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    printf("Connected to server.\n");

    // Get the number of terms in the Fibonacci sequence from the user
    int n;
    printf("Enter the number of Fibonacci terms: ");
    scanf("%d", &n);

    // Send n to the server
    char message[BUFFER_SIZE];
    snprintf(message, sizeof(message), "%d", n);
    if (send(sock, message, strlen(message), 0) == SOCKET_ERROR) {
        printf("Send failed with error code: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Receive the Fibonacci sequence from the server
    int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received == SOCKET_ERROR) {
        printf("Receive failed with error code: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    buffer[bytes_received] = '\0';  // Null-terminate the received data
    printf("Fibonacci sequence received from server: %s\n", buffer);

    // Cleanup Winsock
    closesocket(sock);
    WSACleanup();

    return 0;
}
