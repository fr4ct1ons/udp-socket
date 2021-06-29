﻿#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")


int main(int argc, char* argv[])
{
	//std::cout << "Tem que fazer o cliente" << std::endl;

	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsok = WSAStartup(version, &data);
	if (wsok != 0)
	{
		std::cout << "ERROR STARTING WINSOCK - error: " << wsok << std::endl;
		return 1;
	}

	sockaddr_in serverHint;

	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(54000);

	inet_pton(AF_INET, "127.0.0.1", &serverHint.sin_addr);

	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKET out2 = socket(AF_INET, SOCK_DGRAM, 0);

	std::string msg = "";

	if (argc > 1)
	{
		msg = argv[1];
	}
	else
	{
		msg = "loolloll";
	}
	
	int sendOk = sendto(out, msg.c_str(), msg.size() + 1, 0, (sockaddr*)&serverHint, sizeof(serverHint));

	if (sendOk == SOCKET_ERROR)
	{
		std::cout << "ERROR SENDING MESSAGE - error: " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << "Sent message " << msg << std::endl;
	}

	sockaddr_in clientHint;

	clientHint.sin_addr.S_un.S_addr = ADDR_ANY;
	clientHint.sin_family = AF_INET;
	clientHint.sin_port = htons(55000);

	int serverLength = sizeof(clientHint);
	char* buf = new char[1024]();

	if (bind(out2, (sockaddr*)&clientHint, serverLength) == SOCKET_ERROR)
	{
		std::cout << "ERROR BINDING SOCKET - error: " << WSAGetLastError() << std::endl;
		return 2;
	}
	else
	{
		std::cout << "Second socket success" << std::endl;
	}

	int bytesIn = recvfrom(out2, buf, 1024, 0, (sockaddr*)&clientHint, &serverLength);

	if (bytesIn == SOCKET_ERROR)
	{
		std::cout << "ERROR RECEIVING MESSAGE - error: " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << "received message back" << std::endl;
	}

	closesocket(out);
	closesocket(out2);

	//Shutdown
	WSACleanup();
	return 0;

}
