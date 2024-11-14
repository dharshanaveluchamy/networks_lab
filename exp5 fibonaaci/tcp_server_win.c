#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>  // Winsock library for Windows

#pragma comment(lib, "ws2_32.lib")  // Link with Winsock library

#define PORT 8081
#define MAX 100

// Function to calculate Fibonacci sequence
void fibonacci(int n, int *fib) {
    fib[0] = 0;
    fib[1] = 1;
    for (int i = 2; i < n; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
}

int main() {
    WSADATA wsa;
    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    int fib[MAX];

    // Initialize Winsock
    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Winsock initialized.\n");

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Socket created.\n");

    // Prepare the sockaddr_in structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Bind to any local IP address
    address.sin_port = htons(PORT);        // Bind to specified port

    // Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Bind successful.\n");

    // Listen for incoming connections (3 is the maximum length for the queue of pending connections)
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        printf("Listen failed with error code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Waiting for incoming connections...\n");

    // Accept an incoming connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    if (new_socket == INVALID_SOCKET) {
        printf("Accept failed with error code: %d\n", WSAGetLastError());
        return 1;
    }
    printf("Connection accepted.\n");

    // Receive the value of n (number of Fibonacci terms) from client
    int valread = recv(new_socket, buffer, sizeof(buffer), 0);
    if (valread == SOCKET_ERROR) {
        printf("Receive failed with error code: %d\n", WSAGetLastError());
        closesocket(new_socket);
        return 1;
    }
    
    int n = atoi(buffer);  // Convert the received string to an integer
    printf("Received n = %d from client\n", n);

    // Generate Fibonacci sequence
    if (n > MAX) n = MAX;  // Limit n to prevent buffer overflow
    fibonacci(n, fib);

    // Send the Fibonacci sequence to the client
    char fib_str[1024] = "";
    for (int i = 0; i < n; i++) {
        char num_str[10];
        sprintf(num_str, "%d ", fib[i]);
        strcat(fib_str, num_str);  // Append the Fibonacci number to the result string
    }

    send(new_socket, fib_str, strlen(fib_str), 0);
    printf("Fibonacci sequence sent to client: %s\n", fib_str);

    // Cleanup Winsock
    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup();

    return 0;
}
