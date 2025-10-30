#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib,"ws2_32.lib")

#define PORT 8888
#define MAX_CLIENTS 10
#define BUF_SIZE 512

typedef struct {
    SOCKET sock;
    char name[50];
} Client;

Client clients[MAX_CLIENTS];
int clientCount = 0;

DWORD WINAPI ClientHandler(LPVOID param) {
    int idx = *(int*)param;
    char buffer[BUF_SIZE];
    int n;
    while ((n = recv(clients[idx].sock, buffer, BUF_SIZE-1, 0)) > 0) {
        buffer[n] = '\0';
        char msg[BUF_SIZE+60];
        snprintf(msg, sizeof(msg), "%s: %s\n", clients[idx].name, buffer);

        // ส่งข้อความไปทุก client
        for (int i = 0; i < clientCount; i++)
            send(clients[i].sock, msg, strlen(msg), 0);
    }
    closesocket(clients[idx].sock);
    printf("%s ออกจาก chat\n", clients[idx].name);
    return 0;
}

int main() {
    system("chcp 65001 >nul");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "th_TH.UTF-8");
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server, (struct sockaddr*)&addr, sizeof(addr));
    listen(server, MAX_CLIENTS);
    printf("Server รันบน port %d\n", PORT);

    while (1) {
        SOCKET clientSock = accept(server, NULL, NULL);
        if (clientSock != INVALID_SOCKET && clientCount < MAX_CLIENTS) {
            printf("มีผู้ใช้เชื่อมต่อ...\n");
            // รับชื่อ
            char name[50];
            recv(clientSock, name, sizeof(name)-1, 0);
            name[strcspn(name,"\n")]=0;
            clients[clientCount].sock = clientSock;
            strcpy(clients[clientCount].name, name);

            int idx = clientCount;
            clientCount++;
            CreateThread(NULL, 0, ClientHandler, &idx, 0, NULL);
        }
    }

    closesocket(server);
    WSACleanup();
    return 0;
}
