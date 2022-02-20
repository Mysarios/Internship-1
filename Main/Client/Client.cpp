#include <iostream>
#include <string>
#include <thread>
#include <algorithm>
#include <mutex>
#include <cmath>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

std::mutex Switcher;
char* InsertInArray(int &SizeOfArray, char Array[],char Element,int Position){
	int Index;

	for (Index = SizeOfArray; Index >= Position; Index--)
		Array[Index + 1] = Array[Index];

	Array[Position] = Element;
	SizeOfArray++;
	return Array;
}
char* GetSizeOfInt(int num) {
	if (num == 0) {
		char Array[1];
		Array[0] = 0;
		return Array;
	}

	int Size = 0;
	int NumBuf=num;
	while (NumBuf) {
		NumBuf /= 10;
		Size++;
	}
	char* Array = new char[Size-1];

	Array[0] = num / (int)pow(10, Size - 1);
	for (int i = 1; i < Size; i++) {
		Array[i] = num / (int)pow(10, Size - 1 - i) - (int)Array[i-1] * 10;
		num -= (int)Array[i-1] * pow(10, Size - i);
	}
	return Array;
}
class Chat {
public:
	char Buffer[128];
	void GetWord(char Word[128]) {
		for ( ; ; ){
			Switcher.lock();
			
			int Size;
			int Check = 0;
			do {
				std::cout << "-->Enter word => ";

				//Проверка старого ввода 
				if (char(std::cin.peek()) == '\n')
					std::cin.ignore();

				if (std::cin.fail())
				{
					std::cin.clear();
					std::cin.ignore(32767, '\n');
				}

				std::cin.getline(Word,66);
				Check = 0;
				Size = strlen(Word);
				if (Size > 64){
					std::cout << "Long word. Try again." << std::endl;
					Check = 1;
				}
				for (int Index = 0; Index < Size; Index++) {
					if (('9' < Word[Index]) || (Word[Index] < '0')) {
						Check = 2;
					}
				}
				if (Check==2) {
					std::cout << "Bad word. Try again." << std::endl;
				}

			} while (Check>0);

			std::sort(Word, Word + Size);
			for (int Index = 0; Index < Size; Index++) {
					if (Word[Index] % 2 == 0) {
						Word[Index] = 'K';
						InsertInArray(Size, Word, 'B', ++Index);
					}
				}				

			Switcher.unlock();
			Sleep(5);
		}
	}
	void SendWord(char Word[128], SOCKET Connection, SOCKADDR_IN addres) {
		int MsgSend = 0;
		for ( ; ; ) {
				Switcher.lock();

				int Size = strlen(Word), sum = 0;
				for (int Index = 0; Index < Size; Index++) {
					if (('9' >= Word[Index]) && (Word[Index] >= '0')) {
						sum += Word[Index] - '0';
					}
				}
				std::cout << "Get Word= " << Word << std::endl;
				std::cout << "Sum of Elements= " << sum << std::endl << std::endl;

				int ArraySize = 0;
				int NumBuf = sum;
				if (sum == 0) {
					ArraySize = 1;
				}

				while (NumBuf) {
					NumBuf /= 10;
					ArraySize++;
				}

				if (ArraySize) {
					char* Message = new char[ArraySize - 1];
					Message = GetSizeOfInt(sum);
					MsgSend = send(Connection, Message, sizeof(Message), NULL);
					if (MsgSend < 0) {
						std::cout << "Server was crash.    Try to reconnecting..." << std::endl;
						ConnectToServer(Connection, addres);
						MsgSend=send(Connection, Message, sizeof(Message), NULL);
					}	
					
				}
				for (int i = 0; i < ArraySize; i++) {
					Word[i] = NULL;
				}

				Switcher.unlock();
				Sleep(50);
		}
	}
	void ConnectToServer(SOCKET &Connection, SOCKADDR_IN &addres) {
		int SizeOfAddr = sizeof(addres);
		addres.sin_family = AF_INET;

		bool SuccessfulConnect = connect(Connection, (SOCKADDR*)&addres, SizeOfAddr);
		if (SuccessfulConnect) {
			std::cout << "Wait to connect... => ";
			do{
			Connection = socket(AF_INET, SOCK_STREAM, NULL);
				SuccessfulConnect = connect(Connection, (SOCKADDR*)&addres, SizeOfAddr);
			}while (SuccessfulConnect);
		}
			std::cout << "Connect to Server.\n" << std::endl;
			return ;
		

	}
};
int main(){	
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Eror lib " << std::endl;
		exit(1);
	}

	SOCKADDR_IN addres;
	addres.sin_addr.s_addr = inet_addr("127.0.0.1");
	addres.sin_port = htons(1111);
	addres.sin_family = AF_INET;
	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
	int SizeOfAddr = sizeof(addres);

	Chat Client;
	Client.ConnectToServer(Connection, addres);

	std::thread FirstThread(&Chat::GetWord, Client,Client.Buffer);
	Sleep(500);
	std::thread SecondThread(&Chat::SendWord, Client, Client.Buffer,Connection,addres);

	if (FirstThread.joinable())
		FirstThread.join();

	if (SecondThread.joinable())
		SecondThread.join();

	return 0;
}