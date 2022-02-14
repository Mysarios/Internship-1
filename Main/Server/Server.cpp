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
	for (;;) {
		recv(NewConnect, Buffer, sizeof(Buffer), NULL);
		//std::cout << Buffer << std::endl;

		//if (Buffer[0] == 0) {
			//std::cout << "Bad Num";
			//Buffer[0] = NULL;
			//continue;
		//}
			//std::cout << "Msg from client= ";

			int i = 0, NumberFromClient = 0;
			while ((int)Buffer[i] > 0) {
				//std::cout << (int)Buffer[i];
				i++;
			}
			std::cout << std::endl;

			for (int n = 0; n < i; n++) {
				NumberFromClient += (Buffer[n]) * pow(10, i - 1 - n);
			}
			if ((i >= 2) && (NumberFromClient % 32 == 0)) {
				std::cout << "Good Num!";
			}
			else {
				std::cout << "Bad Num";
			}
			std::cout << std::endl;

			for (int n = 0; n < i; n++) {
				Buffer[n] = NULL;
			}
		//}
	}
	system("pause");
}
