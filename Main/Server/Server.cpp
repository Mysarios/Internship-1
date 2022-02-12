#pragma comment(lib,"ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#pragma warning(disable:4996)

int main(){
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Eror lib " << std::endl;
		exit(1);
	}
	
	SOCKADDR_IN addr;
	int SizeOfAddr = sizeof(addr);
	addr.sin_addr.s_addr= inet_addr("127.0.0.1");
	addr.sin_port=htons(1111);
	addr.sin_family = AF_INET;

	SOCKET SocketListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(SocketListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(SocketListen, SOMAXCONN);

	SOCKET NewConnect;
	NewConnect = accept(SocketListen, (SOCKADDR*)&addr, &SizeOfAddr);

	if (NewConnect == 0) {
		std::cout << "Eror Connect" << std::endl;
	}
	else {
		std::cout << "Connected!!" << std::endl;
	}
	char Buffer[128];
	//for (;;) {
		recv(NewConnect, Buffer, sizeof(Buffer), NULL);
		int Size = strlen(Buffer);
		Size = sizeof(Buffer);

		//std::cout << "Size= " << (int)Buffer[0] << std::endl;
		//std::cout << "Msg from client= "<< std::endl;
		for (int Index = 0; Index <= Size; Index++) {
			std::cout << Buffer[Index];
		}
		std::cout << std::endl;
		int i = 0;
		while ((int)Buffer[i]!=-52) {
			std::cout << (int)Buffer[i];
			i++;
		}
		std::cout << std::endl;
	//}
	system("pause");
}
