#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")


int main(int argc, char* argv[])
{
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsok = WSAStartup(version, &data);
	if (wsok != 0)
	{
		std::cout << "ERROR STARTING WINSOCK - error: " << wsok << std::endl;
		return 1;
	}

	sockaddr_in serverHint;

	serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(54000);

	inet_pton(AF_INET, "127.0.0.1", &serverHint.sin_addr);

	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	int serverLength = sizeof(serverHint);
	char* buffer = new char[1024]();

	std::string msg = "";

	if (argc > 1)
	{
		msg = argv[1];
	}
	else
	{
		msg = "Iae BB, bora pro bem bolado hj?";
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

	if (bind(out, (sockaddr*)&serverHint, serverLength) == SOCKET_ERROR)
	{
		std::cout << "ERROR BINDING SOCKET - error: " << WSAGetLastError() << std::endl;
		return 2;
	}
	else
	{
		std::cout << "Second socket success" << std::endl;
	}

	int bytesIn = recvfrom(out, buffer, 1024, 0, (sockaddr*)&serverHint, &serverLength);

	if (bytesIn == SOCKET_ERROR)
	{
		std::cout << "ERROR RECEIVING MESSAGE - error: " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << "Cliente 2: " << buffer << std::endl;
	}
	
	closesocket(out);

	//Shutdown
	WSACleanup();
	return 0;

}
