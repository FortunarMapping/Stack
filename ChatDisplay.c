#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <winsock2.h>
#include <windows.h>
#include <time.h>

#pragma comment(lib,"ws2_32.lib")

#define BUF_SIZE 512
#define BCAST_PORT 9999
#define STACK_SIZE 100   // เก็บข้อความล่าสุด 100 ข้อความ

typedef struct {
    char *items[STACK_SIZE];
    int top;
} Stack;

void push(Stack *s, const char *msg) {
    if (s->top >= STACK_SIZE) {
        free(s->items[STACK_SIZE - 1]);
        s->top--;
    }
    for (int i = s->top; i > 0; i--)
        s->items[i] = s->items[i - 1];
    s->items[0] = _strdup(msg);
    s->top++;
}

void displayStack(Stack *s) {
    system("cls");
    printf("=== แสดงข้อความแชท (อ่านอย่างเดียว) ===\n\n");
    for (int i = 0; i < s->top; i++)
        printf("%s\n", s->items[i]);
}

void freeStack(Stack *s) {
    for (int i = 0; i < s->top; i++)
        free(s->items[i]);
}

// เติมเวลาลงท้ายข้อความ
void appendTime(char *out, const char *msg) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestr[16];
    strftime(timestr, sizeof(timestr), "%H:%M:%S", t);
    snprintf(out, BUF_SIZE + 64, "%s - %s", msg, timestr);
}

int main() {
    system("chcp 65001 >nul");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "th_TH.UTF-8");

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(BCAST_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));

    SetConsoleTitleA("📺 Chat Display");
    system("color 0A");

    Stack chatStack = { .top = 0 };
    char buf[BUF_SIZE];
    char withTime[BUF_SIZE + 64];

    while (1) {
        int n = recv(sock, buf, BUF_SIZE - 1, 0);
        if (n > 0) {
            buf[n] = '\0';
            appendTime(withTime, buf);     // เติมเวลาตรงนี้
            push(&chatStack, withTime);
            displayStack(&chatStack);
        }
    }

    freeStack(&chatStack);
    closesocket(sock);
    WSACleanup();
    return 0;
}
