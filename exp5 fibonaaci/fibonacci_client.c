#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET sockfd;
    struct sockaddr_in server_addr;
    int server_addr_len = sizeof(server_addr);
    unsigned long long result;
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
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    printf("Enter the Fibonacci sequence position to request: ");
    scanf("%d", &num);

    // Send number to server
    sendto(sockfd, (char*)&num, sizeof(num), 0, (struct sockaddr*)&server_addr, server_addr_len);

    // Receive result from server
    recvfrom(sockfd, (char*)&result, sizeof(result), 0, (struct sockaddr*)&server_addr, &server_addr_len);
    printf("Fibonacci number at position %d is %llu\n", num, result);

    closesocket(sockfd);
    WSACleanup();
    return 0;
}
