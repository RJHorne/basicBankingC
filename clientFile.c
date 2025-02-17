/* -------------------- client.c -------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080

void* receive_handler(void* sockfd) {
    int client_sock = *(int*)sockfd;
    char buffer[1024];
    
    while(1) {
        ssize_t bytes_recv = recv(client_sock, buffer, sizeof(buffer)-1, 0);
        if(bytes_recv <= 0) break;
        
        buffer[bytes_recv] = '\0';
        printf("Server response: %s\n", buffer);
    }
    
    return NULL;
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if(inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported\n");
        return -1;
    }
    
    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }
    
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_handler, &sock);
    
    printf("Connected to bank server. Commands available:\n");
    printf("BALANCE <account>\n");
    printf("DEPOSIT <account> <amount>\n");
    printf("WITHDRAW <account> <amount>\n");
    printf("TRANSFER <from> <to> <amount>\n");
    
    while(1) {
        char command[1024];
        printf("> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;
        
        send(sock, command, strlen(command), 0);
        usleep(100000); // Allow time for response
    }
    
    close(sock);
    return 0;
}

