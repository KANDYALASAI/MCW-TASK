#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    char *ip = "127.0.0.1";  
    int port = 5566;

    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    socklen_t addr_size;
    int n;

  
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]UDP server socket created.\n");

    
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    
    n = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (n < 0) {
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bind to the port number: %d\n", port);

    printf("Listening for clients...\n");

    while (1) {
        
        addr_size = sizeof(client_addr);
        bzero(buffer, 1024);
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addr_size);
        printf("Client: %s\n", buffer);

        
        bzero(buffer, 1024);
        strcpy(buffer, "HI, THIS IS SERVER. HAVE A NICE DAY!!!");
        printf("Server: %s\n", buffer);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, addr_size);
    }

    close(sockfd);
    return 0;
}

