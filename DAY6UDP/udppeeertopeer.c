#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5566
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Listen on all interfaces
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Server is listening on port %d...\n", PORT);

    // Wait for a broadcast message from a client
    while (1) {
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, 
                         (struct sockaddr *)&client_addr, &addr_len);
        buffer[n] = '\0';
        printf("Received: %s\n", buffer);

        // Send a response back to the client
        const char *response = "Hello from server";
        sendto(sockfd, response, strlen(response), 0, 
               (struct sockaddr *)&client_addr, addr_len);
        printf("Response sent to client\n");
    }

    close(sockfd);
    return 0;
}

