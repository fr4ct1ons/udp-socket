#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")


int main()
{
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	
	int wsok = WSAStartup(version, &data);

	if (wsok != 0)
	{
		std::cout << "ERROR STARTING WINSOCK - error: " << wsok << std::endl;
		return 1;
	}

	//Setup do cliente 1
	SOCKET skt = socket(AF_INET, SOCK_DGRAM, 0); // Recebe Mensagem do Cliente 1
	sockaddr_in client1Info;
	client1Info.sin_addr.S_un.S_addr = ADDR_ANY;
	client1Info.sin_family = AF_INET;
	client1Info.sin_port = htons(54000);

	if (bind(skt, (sockaddr*)&client1Info, sizeof(client1Info)) == SOCKET_ERROR)
	{
		std::cout << "ERROR BINDING SOCKET - error: " << WSAGetLastError() << std::endl;
		return 2;
	}


	SOCKET skt4 = socket(AF_INET, SOCK_DGRAM, 0); // Envia Mensagem para o Cliente 1
	client1Info.sin_port = htons(56000);
	
	if (bind(skt4, (sockaddr*)&client1Info, sizeof(client1Info)) == SOCKET_ERROR)
	{
		std::cout << "ERROR BINDING SOCKET - error: " << WSAGetLastError() << std::endl;
		return 2;
	}

	sockaddr_in client1;
	int client1Length = sizeof(client1);
	ZeroMemory(&client1, client1Length);

	char *buffer1 = new char[1024]();

	//Setup do cliente 2
	SOCKET skt2 = socket(AF_INET, SOCK_DGRAM, 0); // Envia Mensagem para o Cliente 2
	sockaddr_in client2Info;
	client2Info.sin_addr.S_un.S_addr = ADDR_ANY;
	client2Info.sin_family = AF_INET;
	client2Info.sin_port = htons(55000);

	if (bind(skt2, (sockaddr*)&client2Info, sizeof(client2Info)) == SOCKET_ERROR) // Conecta Sockete sk2 com o cliente 2
	{
		std::cout << "ERROR BINDING SOCKET - error: " << WSAGetLastError() << std::endl;
		return 2;
	}	


	SOCKET skt3 = socket(AF_INET, SOCK_DGRAM, 0); // Recebe Mensagem do Cliente 2
	client2Info.sin_port = htons(57000);
	
	if (bind(skt3, (sockaddr*)&client2Info, sizeof(client2Info)) == SOCKET_ERROR) // Conecta Sockete sk2 com o cliente 2
	{
		std::cout << "ERROR BINDING SOCKET - error: " << WSAGetLastError() << std::endl;
		return 2;
	}	

	sockaddr_in client2;
	int client2Length = sizeof(client2);
	ZeroMemory(&client2, client2Length);

	char *buffer2 = new char[1024]();	
	

	std::cout << "Server setup successful." << std::endl;
	
	while (true)
	{
		//Recebe do cliente 1 a mensagem
		if (recvfrom(skt, buffer1, 1024, 0, (sockaddr*)&client1, &client1Length) == SOCKET_ERROR)
		{
			std::cout << "ERROR RECEIVING MESSAGE FROM CLIENT - error: " << WSAGetLastError() << std::endl;
			continue;
		}
		char *client1IP = new char[256]();

		inet_ntop(AF_INET, &client1.sin_addr, client1IP, 256);

		//Exibe mensagem enviada pelo cliente 1
		std::cout << "Message received from " << client1IP << ": " << buffer1 << std::endl;
		

			
		inet_pton(AF_INET, client1IP, &client2Info.sin_addr);

		std::string buffer = buffer1;

		//Envia mensagem do cliente 1 para o cliente 2	
		if (sendto(skt2, buffer.c_str(), buffer.size() + 1, 0, (sockaddr*)&client2Info, sizeof(client2Info)) == SOCKET_ERROR)
		{
			std::cout << "ERROR SENDING MESSAGE - error: " << WSAGetLastError() << std::endl;
		}
		else
		{
			std::cout << "Sent message to Client 2" << std::endl;
		}

		//Recebe do cliente 2 a mensagem
		if (recvfrom(skt3, buffer2, 1024, 0, (sockaddr*)&client2, &client2Length) == SOCKET_ERROR)
		{
			std::cout << "ERROR RECEIVING MESSAGE FROM CLIENT - error: " << WSAGetLastError() << std::endl;
			continue;
		}
		
		char *client2IP = new char[256]();

		inet_ntop(AF_INET, &client2.sin_addr, client2IP, 256);

		//Exibe mensagem enviada pelo cliente 2
		std::cout << "Message received from " << client2IP << ": " << buffer2 << std::endl;

		//Envia mensagem para o cliente 1

		inet_pton(AF_INET, client2IP, &client1Info.sin_addr);		
		

		//String buffer
		buffer = buffer2;

		if (sendto(skt4, buffer.c_str(), buffer.size() + 1, 0, (sockaddr*)&client1Info, sizeof(client1Info)) == SOCKET_ERROR)
		{
			std::cout << "ERROR SENDING MESSAGE - error: " << WSAGetLastError() << std::endl;
		}
		else
		{
			std::cout << "Sent message to Client 1" << std::endl;
		}		
	}

	closesocket(skt);
	closesocket(skt2);
	closesocket(skt3);
	closesocket(skt4);

	

	//Shutdown
	WSACleanup();
	return 0;

}
