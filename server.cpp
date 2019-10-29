#include <cstdlib>
#include <iostream>
#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <synchapi.h>
using namespace std;
int main(int argc, char *argv[]) {
    cout << "SR START" << '\n';
    WSAData WSData;
    WSAStartup(0x101, &WSData);
    struct sockaddr_in local;
    SOCKET s;
    int s1;
    int rc;
    char buf [1024];
    local.sin_family = AF_INET;
    local.sin_port = htons(25001);
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) perror("Could not create socket.");
    rc = bind(s, (struct sockaddr *) &local, sizeof (local));
    if (rc < 0) {
        perror("Could not bind address");
        closesocket(s);
    }
    int server_running=1; 
    while (server_running) {
        rc = listen(s, 5);
        if (rc) {
            perror("Cannot listen port.");
            closesocket(s);
        }
        s1 = accept(s, NULL, NULL);
        if (s1 < 0) {
            perror("Cannot accept connection.");
            closesocket(s);
        }
        rc = recv(s1, buf, 1024, 0);
        if (rc <= 0) {
            perror("Cannot read data.");
            closesocket(s);
        }
        cout << buf << '\n';
        Sleep(7);
    }
    closesocket(s);
    WSACleanup();
    system("PAUSE");
    return EXIT_SUCCESS;
}