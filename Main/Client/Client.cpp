#include <iostream>
#include <string>
#include <thread>
#include <algorithm>
#include <mutex>
#include <cmath>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)
#include <WinSock2.h>

std::mutex Switcher;
char* Insert(int &SizeOfArray, char Array[],char Element,int Position){
	int Index;

	for (Index = SizeOfArray; Index >= Position; Index--)
		Array[Index + 1] = Array[Index];

	Array[Position] = Element;
	SizeOfArray++;
	return Array;
}
char* GetSizeOfInt(int num) {
	int Size = 0;
	int NumBuf=num;
	while (NumBuf) {
		NumBuf /= 10;
		Size++;
	}
	char* Array = new char[Size-1];
	Array[0] = num / (int)pow(10, Size - 1);
	std::cout << (int)Array[0];
	for (int i = 1; i < Size; i++) {
		Array[i] = num / (int)pow(10, Size - 1 - i) - (int)Array[i-1] * 10;
		std::cout << (int)Array[i];
		num -= (int)Array[i-1] * pow(10, Size - i);
	}
	return Array;
}
class Chat {
private:
	
public:
	char Buffer[128];
	void GetWord(char Word[128]) {
		Switcher.lock();
		std::cin >> Word;
		int Size = strlen(Word);
		/*if (Size > 64) {
			std::cout << "Long Word!" << std::endl;
			return;
		}*/

		for (int Index = 0; Index < Size;Index++) {
				if (('9' < Word[Index]) || (Word[Index] < '0')) {
					std::cout << "BadWord" <<std::endl;
					return;
				}
		}

		std::sort(Word,Word+Size);
		for (int Index = 0; Index < Size; Index++) {
			if (Word[Index]%2 == 0) {
				Word[Index] = 'K';
				Insert(Size, Word, 'B', ++Index);
			}
		}

		Switcher.unlock();
		return;
	}
	void SendWord(char Word[128], SOCKET Connection) {
		Switcher.lock();
		int Size = strlen(Word),sum=0;
		for (int Index = 0; Index < Size; Index++) {
			if (('9' >= Word[Index]) && (Word[Index] >= '0')) {
				sum += Word[Index] - '0';
				//std::cout << "Element= " << Word[Index] - '0' << std::endl;
			}
		}
		std::cout << "Sum of Elements= " << sum << std::endl;

		int ArraySize = 0;
		int NumBuf = sum;
		while (NumBuf) {
			NumBuf /= 10;
			ArraySize++;
		}

		char* Message = new char[ArraySize - 1];
		std::cout << ArraySize << std::endl;
		Message=GetSizeOfInt(sum);
		send(Connection,Message,sizeof(Message), NULL);

		Word = NULL;
		Switcher.unlock();
		
		return;
	}
};

int main()
{	
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Eror lib " << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int SizeOfAddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, SizeOfAddr) != 0) {
		std::cout << "Error Connect to Server" << std::endl;
		return 1;
	}
	std::cout << "Connect to Server" << std::endl;
	
	Chat Client;
	for (;;) {
	std::thread FirstT(&Chat::GetWord, Client,Client.Buffer);
	if (FirstT.joinable())
		FirstT.join();
	//std::cout << Client.Buffer[0] << std::endl;
	std::thread SecondT(&Chat::SendWord, Client, Client.Buffer,Connection);
	if (SecondT.joinable())
		SecondT.join();
	}

	system("pause");


	return 0;
}

