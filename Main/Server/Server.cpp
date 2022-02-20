#include <iostream>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")
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

	SOCKET NewConnect=0;
	std::cout << "Wait Connect..." << std::endl;
	NewConnect = accept(SocketListen, (SOCKADDR*)&addr, &SizeOfAddr);

	if (NewConnect == 0) { 
		std::cout << "Eror Connect." << std::endl; 
	}
	else{ 
		std::cout << "Connect." << std::endl; 
	}

	int ConnectCheck = 0;
	char Buffer[128];
	for (;;) {
		ConnectCheck=recv(NewConnect, Buffer, sizeof(Buffer), NULL);

		if (ConnectCheck < 0) {
			std::cout << "\nClient was disconnect. Wait to reconnect..." << std::endl;
			NewConnect = accept(SocketListen, (SOCKADDR*)&addr, &SizeOfAddr);
			std::cout << "Client reconnected." << std::endl;
			continue;
		}

		int Size = 0, NumberFromClient = 0;
		while ((int)Buffer[Size] > 0) {
			Size++;
		}
		std::cout << std::endl;

		for (int n = 0; n < Size; n++) {
			NumberFromClient += (Buffer[n]) * pow(10, Size - 1 - n);
		}
		if (Size == 0)
			continue;

		if ((Size >= 2) && (NumberFromClient % 32 == 0)) { 
			std::cout << "-->Good Number";  }
		else {
			std::cout << "-->Bad Number";
		}

		for (int Index = 0; Index < Size; Index++) {
			Buffer[Index] = NULL;
		}
	}
}
