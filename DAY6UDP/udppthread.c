#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 5566
#define BUFFER_SIZE 1024

// Server function
void *run_server(void *arg) {
    int server_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[BUFFER_SIZE];

    // Create UDP socket
    server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sock < 0) {
        perror("[-]Socket error");
        exit(1);
    }
    printf("UDP server socket created.\n");

    // Bind to the specified port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind error");
        exit(1);
    }
    printf("Bind to the port number: %d\n", PORT);

    // Receive data from client
    while (1) {
        bzero(buffer, BUFFER_SIZE);
        addr_size = sizeof(client_addr);
        recvfrom(server_sock, buffer, BUFFER_SIZE, 0, 
                 (struct sockaddr *)&client_addr, &addr_size);
        printf("Client: %s\n", buffer);

        // Send response to client
        bzero(buffer, BUFFER_SIZE);
        strcpy(buffer, "Hello from UDP Server!");
        sendto(server_sock, buffer, strlen(buffer), 0, 
               (struct sockaddr *)&client_addr, addr_size);
        printf("Server: %s\n", buffer);
    }

    close(server_sock);
    return NULL;
}

// Client function
void *run_client(void *arg) {
    int client_sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_size;

    // Create UDP socket
    client_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_sock < 0) {
        perror("Socket error");
        exit(1);
    }
    printf("UDP client socket created.\n");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Send message to server
    bzero(buffer, BUFFER_SIZE);
    strcpy(buffer, "Hello from UDP Client!");
    sendto(client_sock, buffer, strlen(buffer), 0, 
           (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Client: %s\n", buffer);

    // Receive response from server
    bzero(buffer, BUFFER_SIZE);
    addr_size = sizeof(server_addr);
    recvfrom(client_sock, buffer, BUFFER_SIZE, 0, 
             (struct sockaddr *)&server_addr, &addr_size);
    printf("Server: %s\n", buffer);

    close(client_sock);
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

