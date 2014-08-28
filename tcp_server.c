#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 255
#define PORT_NUM 7000

int main(int argc, char* argv[]) {
    int server_socket;
    int client_socket;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int client_addr_size;
    int flag;
    char buffer[MAX_BUFFER_SIZE];
    int length;
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NUM);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    flag = server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (flag < 0) {
        printf("Bad socket\n");
        exit(1);
    }
    
    flag = bind(server_socket, (struct sockaddr*) &server_addr, sizeof(struct sockaddr));
    if (flag < 0) {
        printf("Bad bind\n");
        exit(1);
    }
    
    flag = listen(server_socket, 5);
    if (flag < 0) {
        printf("Bad listen\n");
        exit(1);
    }

    client_addr_size = sizeof(struct sockaddr_in);
    while(1) {
        flag = client_socket = accept(server_socket, (struct sockaddr*) &client_addr, (socklen_t*) &client_addr_size);
        if (flag < 0) {
            printf("Bad accept\n");
            exit(1);
        }
        
        length = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
        buffer[length] = '\0';
        if (length > 0)
            printf("%s\n", buffer);
    }
    close(server_socket);
    close(client_socket);
    
    return 0;
}
