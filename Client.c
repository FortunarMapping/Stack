#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib,"ws2_32.lib")
#define BUF_SIZE 512

DWORD WINAPI ReceiveThread(LPVOID param) {
    SOCKET sock = *(SOCKET*)param;
    char buffer[BUF_SIZE];
    int n;
    while ((n = recv(sock, buffer, BUF_SIZE-1, 0)) > 0) {
        buffer[n] = '\0';
        printf("%s", buffer);
    }
    return 0;
}

int main() {
    system("chcp 65001 >nul");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "th_TH.UTF-8");
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    char serverIP[20];
    char name[50];

    // ให้ผู้ใช้พิมพ์ชื่อ
    printf("พิมพ์ชื่อของคุณ: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0; // ตัด \n

    // ให้ผู้ใช้กรอก IP ของ Server
    printf("กรอก IP ของ Server: ");
    fgets(serverIP, sizeof(serverIP), stdin);
    serverIP[strcspn(serverIP, "\n")] = 0;

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr(serverIP);

    if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        printf("ไม่สามารถเชื่อมต่อ Server ที่ %s\n", serverIP);
        return 1;
    }

    // ส่งชื่อผู้ใช้ไป Server
    send(sock, name, strlen(name), 0);

    // เริ่ม thread สำหรับรับข้อความ
    CreateThread(NULL,0,ReceiveThread,&sock,0,NULL);

    char msg[BUF_SIZE];
    while (1) {
        fgets(msg, sizeof(msg), stdin);
        msg[strcspn(msg,"\n")]=0;
        if(strcmp(msg,"exit")==0) break;
        send(sock, msg, strlen(msg), 0);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
