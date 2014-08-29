#ifdef __gnu_linux__

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
    char buffer[MAX_BUFFER_SIZE];
    int length;
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NUM);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (0 > (server_socket = socket(AF_INET, SOCK_STREAM, 0))) {
        printf("Bad socket\n");
        exit(1);
    }
    
    if (0 > bind(server_socket, (struct sockaddr*) &server_addr, sizeof(struct sockaddr))) {
        printf("Bad bind\n");
        exit(1);
    }
    
    if (0 > listen(server_socket, 5)) {
        printf("Bad listen\n");
        exit(1);
    }
    
    client_addr_size = sizeof(struct sockaddr_in);
    while(1) {
        if (0 > (client_socket = accept(server_socket, (struct sockaddr*) &client_addr, (socklen_t*) &client_addr_size))) {
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

#elif WIN32

#include <Windows.h>
#include <WinSock.h>
#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 255
#define PORT_NUM 7000

int main(int argc, char* argv[]) {
    WSADATA wsa_data;
    SOCKET server_socket;
    SOCKET client_socket;
    WORD socket_version;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int client_addr_size;
    char buffer[MAX_BUFFER_SIZE];
    int length;
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NUM);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    socket_version = MAKEWORD(2, 2);
    if (0 != WSAStartup(socket_version, &wsa_data)) {
        printf("Init winsock.dll failed\n");
        exit(1);
    }
    
    if (INVALID_SOCKET == (server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        printf("Bad socket\n");
        exit(1);
    }
    
    if (SOCKET_ERROR == bind(server_socket, (struct sockaddr*) &server_addr, sizeof(struct sockaddr))) {
        printf("Bad bind\n");
        exit(1);
    }
    
    if (SOCKET_ERROR == listen(server_socket, 5)) {
        printf("Bad listen\n");
        exit(1);
    }
    
    client_addr_size = sizeof(struct sockaddr_in);
    while(1) {
        if (INVALID_SOCKET == (client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &client_addr_size))) {
            printf("Bad accept\n");
            exit(1);
        }
        
        length = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
        buffer[length] = '\0';
        if (length > 0)
            printf("%s\n", buffer);
    }
    closesocket(server_socket);
    closesocket(client_socket);
    WSACleanup();
    
    return 0;
}

#endif
