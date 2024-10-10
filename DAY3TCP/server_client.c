#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 5566
#define BUFFER_SIZE 1024


void *run_server(void *arg) {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[BUFFER_SIZE];
    int n;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("[-]Socket error");
        exit(1);
    }
    printf("TCP server socket created.\n");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    
    n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (n < 0) {
        perror("Bind error");
        exit(1);
    }
    printf("Bind to the port number: %d\n", PORT);

    
    listen(server_sock, 5);
    printf("Listening...\n");

    while (1) {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
        printf("Client connected.\n");

        
        bzero(buffer, BUFFER_SIZE);
        recv(client_sock, buffer, sizeof(buffer), 0);
        printf("Client: %s\n", buffer);

        // Send response to client
        bzero(buffer, BUFFER_SIZE);
        strcpy(buffer, "HI, THIS IS SERVER. HAVE A NICE DAY!!!");
        printf("Server: %s\n", buffer);
        send(client_sock, buffer, strlen(buffer), 0);

        close(client_sock);
        printf("Client disconnected.\n\n");
    }

    close(server_sock);
    return NULL;
}

// Function for the client
void *run_client(void *arg) {
    int sock;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];

    // Create client socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket error");
        exit(1);
    }
    printf("TCP client socket created.\n");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Connection error");
        exit(1);
    }
    printf("Connected to the server.\n");

    // Send message to server
    bzero(buffer, BUFFER_SIZE);
    strcpy(buffer, "HELLO, THIS IS CLIENT.");
    printf("Client: %s\n", buffer);
    send(sock, buffer, strlen(buffer), 0);

    // Receive response from server
    bzero(buffer, BUFFER_SIZE);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("Server: %s\n", buffer);

    close(sock);
    printf("Disconnected from the server.\n");

    return NULL;
}

int main() {
    pthread_t server_thread, client_thread;

    // Create and run server thread
    pthread_create(&server_thread, NULL, run_server, NULL);

    // Give the server a moment to start
    sleep(1);

    // Create and run client thread
    pthread_create(&client_thread, NULL, run_client, NULL);

    // Wait for the threads to finish
    pthread_join(server_thread, NULL);
    pthread_join(client_thread, NULL);

    return 0;
}

