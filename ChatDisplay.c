#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib,"ws2_32.lib")

#define BUF_SIZE 512
#define BCAST_PORT 9999
#define STACK_SIZE 100   // เก็บข้อความล่าสุด 100 ข้อความ

typedef struct {
    char *items[STACK_SIZE];
    int top;
} Stack;

// Push: ข้อความใหม่อยู่ top
void push(Stack *s, const char *msg) {
    if(s->top >= STACK_SIZE) {
        // Stack เต็ม: free ข้อความเก่าสุดด้านล่าง
        free(s->items[STACK_SIZE-1]);
        s->top--;
    }
    // เลื่อนข้อความลง
    for(int i=s->top; i>0; i--) {
        s->items[i] = s->items[i-1];
    }
    s->items[0] = _strdup(msg);
    s->top++;
}

// แสดงข้อความ: top อยู่ด้านบน
void displayStack(Stack *s) {
    system("cls");
    printf("=== แสดงข้อความแชท (อ่านอย่างเดียว) ===\n\n");
    for(int i=0;i<s->top;i++) {
        printf("%s", s->items[i]);
    }
}

void freeStack(Stack *s) {
    for(int i=0;i<s->top;i++)
        free(s->items[i]);
}

int main() {
    system("chcp 65001 >nul");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "th_TH.UTF-8");

    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

    // Bind กับทุก IP ของเครื่อง
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(BCAST_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));

    SetConsoleTitleA("📺 Chat Display");
    system("color 0A");

    Stack chatStack = { .top = 0 };
    char buf[BUF_SIZE];

    while(1) {
        int n = recv(sock, buf, BUF_SIZE-1, 0);
        if(n > 0) {
            buf[n] = '\0';
            push(&chatStack, buf);
            displayStack(&chatStack);
        }
    }

    freeStack(&chatStack);
    closesocket(sock);
    WSACleanup();
    return 0;
}
