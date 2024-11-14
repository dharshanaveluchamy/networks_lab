#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 12345
#define BUFFER_SIZE 1024

// Function to compute Fibonacci number
unsigned long long fibonacci(int n) {
    if (n <= 1) return n;
    unsigned long long a = 0, b = 1, c;
    for (int i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int main() {
    WSADATA wsa;
    SOCKET sockfd;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    int num;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error Code : %d", WSAGetLastError());
        return 1;
    }

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        perror("Socket creation failed");
        WSACleanup();
        return 1;
    }

    // Prepare server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        perror("Bind failed");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        // Receive data from client
        int len = recvfrom(sockfd, (char*)&num, sizeof(num), 0, (struct sockaddr*)&client_addr, &client_addr_len);
        if (len == SOCKET_ERROR) {
            perror("recvfrom failed");
            continue;
        }

        // Compute Fibonacci number
        unsigned long long result = fibonacci(num);
        printf("Received request for Fibonacci number %d\n", num);

        // Send result back to client
        sendto(sockfd, (char*)&result, sizeof(result), 0, (struct sockaddr*)&client_addr, client_addr_len);
    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}
