#include <cstdlib>
#include <iostream>
#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <vector>
#include <synchapi.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;
using namespace std;
// Используем библиотеку RapidJson
 char* WriteJsonInfoAboutWindow(const char* key_as_window_name,const char* value_as_window_class) {
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject(); 
    writer.Key(key_as_window_name); 
    writer.String(value_as_window_class); 
    writer.EndObject();
    return (char*)s.GetString();
}
vector<string> GetActiveWindowTitleAndClassName() {
    vector<string> list_about_window;
    char wnd_title[256];
    char wnd_class_name[256];
    HWND hwnd = GetForegroundWindow(); 
    GetWindowText(hwnd, wnd_title, sizeof (wnd_title));
    GetClassName(hwnd, wnd_class_name, sizeof (wnd_class_name));
    list_about_window.push_back(string(wnd_title));
    list_about_window.push_back(string(wnd_class_name));
    return list_about_window;
}
using namespace std;
// переодичность сбора - 1
// адресс сервера - 2
// порт у сервера - 3
int main(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "We must have 3 parameters."<<endl;
        cout << "Usage:<program> <rate to ask> <addres to server> <servers port>"<<endl;
		cout<<"Server is on port 25001"<<endl;
        return EXIT_FAILURE;
    }
    cout << "CL START" << '\n';
    WSAData WSData;
    WSAStartup(0x101, &WSData);
    struct sockaddr_in peer;
    SOCKET s;
    int rc;
    string window_name;
    string window_class;
    vector<string>common_window_info;
    char* json_str;
    peer.sin_family = AF_INET;
    peer.sin_port = htons(atoi(argv[3]));
    peer.sin_addr.s_addr = inet_addr(argv[2]); // 127.0.0.1 - адресс сервера
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Could not create socket.");
        closesocket(s);
    }
    rc = connect(s, (struct sockaddr *) &peer, sizeof (peer));
    if (rc) {
        perror("Could not connect");
        closesocket(s);
    }
	int client_running=1;
    while (client_running) {
        common_window_info = GetActiveWindowTitleAndClassName();
        window_name = common_window_info.at(0);
        window_class = common_window_info.at(1);
        json_str = WriteJsonInfoAboutWindow(window_name.c_str(), window_class.c_str());
        cout<<json_str<<endl;
        rc = send(s, json_str, sizeof(json_str), 0);
        if (rc <= 0) {
            perror("Could not send data");
            closesocket(s);
        }
        Sleep(atoi(argv[1]));
    }
    WSACleanup();
    system("PAUSE");
    return EXIT_SUCCESS;
}