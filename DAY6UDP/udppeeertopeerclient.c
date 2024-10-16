#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5566
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in broadcast_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(broadcast_addr);

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Enable broadcast on the socket
    int broadcast = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
        perror("Broadcast option failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Set up the broadcast address
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(PORT); // Network byte order
    broadcast_addr.sin_addr.s_addr = inet_addr("255.255.255.255");

    // Send a broadcast message
    const char *message = "Hello from client";
    sendto(sockfd, message, strlen(message), 0, 
           (struct sockaddr *)&broadcast_addr, addr_len);
    printf("Broadcast message sent\n");

    // Wait for a response from the server
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, 
                     (struct sockaddr *)&broadcast_addr, &addr_len);
    buffer[n] = '\0';
    printf("Received response: %s\n", buffer);

    close(sockfd);
    return 0;
}

