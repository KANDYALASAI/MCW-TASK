// tcp_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 5566
#define BUFFER_SIZE 1024

void *handle_client(void *arg) {
    int client_sock = *((int *)arg);
    char buffer[BUFFER_SIZE];
    int n;

   
    while (1) {
        bzero(buffer, BUFFER_SIZE);
        n = recv(client_sock, buffer, BUFFER_SIZE, 0);
        if (n <= 0) {
            printf("Client disconnected.\n");
            break;
        }
        printf("Client: %s\n", buffer);

        
        strcpy(buffer, "HI, THIS IS SERVER. HAVE A NICE DAY!!!");
        printf("Server: %s\n", buffer);
        send(client_sock, buffer, strlen(buffer), 0);
    }

    close(client_sock);
    return NULL;
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    pthread_t thread_id;

    
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bind to the port number: %d\n", PORT);

   
    listen(server_sock, 5);
    printf("Listening...\n");

    while (1) {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
        if (client_sock < 0) {
            perror("[-]Accept error");
            continue;
        }
        printf("[+]Client connected.\n");

        
        if (pthread_create(&thread_id, NULL, handle_client, &client_sock) != 0) {
            perror("[-]Thread creation error");
            close(client_sock);
        }

        
        pthread_detach(thread_id);
    }

    close(server_sock);
    return 0;
}

