#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <winsock2.h>
#include <windows.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 512
#define BCAST_PORT 9999
#define STACK_SIZE 100 // เก็บข้อความล่าสุด 100 ข้อความ

typedef struct {
    char *items[STACK_SIZE];
    int top;
} Stack;

// ---------------------- Stack Functions ----------------------
void push(Stack *s, const char *msg) {
    if (s->top == STACK_SIZE) {
        free(s->items[STACK_SIZE - 1]);
        memmove(&s->items[1], &s->items[0], (STACK_SIZE - 1) * sizeof(char *));
        s->items[0] = _strdup(msg);
        return;
    }
    memmove(&s->items[1], &s->items[0], s->top * sizeof(char *));
    s->items[0] = _strdup(msg);
    s->top++;
}

void displayStack(Stack *s) {
    system("cls");
    printf("=== แสดงข้อความแชท (อ่านอย่างเดียว) ===\n\n");
    for (int i = 0; i < s->top; i++) {
        printf("%s\n", s->items[i]);
    }
}

void freeStack(Stack *s) {
    for (int i = 0; i < s->top; i++)
        free(s->items[i]);
}

// ---------------------- Utility ----------------------
void formatMessage(char *out, const char *msg) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timeStr[16];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", t);
    snprintf(out, BUF_SIZE + 50, "%s - %s", msg, timeStr);
}

// ---------------------- Save Log (Stack Style + Read-only) ----------------------
void appendLogStack(const char *msg) {
    char filename[64];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(filename, sizeof(filename), "chat_%Y-%m-%d.txt", t);

    // ก่อนเขียน ต้องปลด Read-only (ถ้ามี)
    SetFileAttributesA(filename, FILE_ATTRIBUTE_NORMAL);

    // อ่านไฟล์เก่าแบบ binary
    FILE *fpOld = fopen(filename, "rb");
    char *oldContent = NULL;
    long fsize = 0;
    if (fpOld) {
        fseek(fpOld, 0, SEEK_END);
        fsize = ftell(fpOld);
        fseek(fpOld, 0, SEEK_SET);
        oldContent = malloc(fsize + 1);
        fread(oldContent, 1, fsize, fpOld);
        oldContent[fsize] = '\0'; // สำหรับ safety แต่เป็น binary
        fclose(fpOld);
    }

    // จัดการ BOM ใน oldContent (ถ้ามี ลบออก)
    char *content_start = oldContent;
    size_t content_size = fsize;
    if (fsize >= 3 && (unsigned char)oldContent[0] == 0xEF &&
        (unsigned char)oldContent[1] == 0xBB &&
        (unsigned char)oldContent[2] == 0xBF) {
        content_start += 3;
        content_size -= 3;
    }

    // เขียนใหม่แบบ binary (ข้อความใหม่อยู่บนสุด)
    FILE *fpNew = fopen(filename, "wb");
    if (fpNew) {
        // เขียน BOM สำหรับ UTF-8
        unsigned char bom[] = {0xEF, 0xBB, 0xBF};
        fwrite(bom, 1, 3, fpNew);

        // เขียนข้อความใหม่ + \r\n
        fwrite(msg, 1, strlen(msg), fpNew);
        fwrite("\r\n", 1, 2, fpNew);

        // เขียน oldContent (ไม่มี BOM)
        if (oldContent)
            fwrite(content_start, 1, content_size, fpNew);
        fclose(fpNew);
    }

    if (oldContent)
        free(oldContent);

    // ตั้งค่าเป็น Read-only ✅
    SetFileAttributesA(filename, FILE_ATTRIBUTE_READONLY);
}

// ---------------------- Main ----------------------
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
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));

    SetConsoleTitleW(L"📺 Chat Display"); // ✅ ใช้ W เพื่อรองรับ Unicode/อิโมจิ

    system("color 0A");

    Stack chatStack = {.top = 0};
    char buf[BUF_SIZE];
    char formatted[BUF_SIZE + 50];
    printf("เริ่มรับข้อความ...\n");

    while (1) {
        int n = recv(sock, buf, BUF_SIZE - 1, 0);
        if (n > 0) {
            buf[n] = '\0';
            formatMessage(formatted, buf);
            push(&chatStack, formatted);
            displayStack(&chatStack);
            appendLogStack(formatted); // ✅ บันทึกแบบ Stack และล็อกไฟล์ให้อ่านอย่างเดียว
        }
    }

    freeStack(&chatStack);
    closesocket(sock);
    WSACleanup();
    return 0;
}