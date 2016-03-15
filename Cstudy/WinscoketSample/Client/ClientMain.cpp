#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib,"Ws2_32.lib")
#pragma warning(disable: 4996)

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

int main(int argc, char** argv)
{
	WSAData wsadata;
	int iResult;
	
	if (argc == 1) {
		printf("Please run with destination");
		return 0;
	}

	iResult = WSAStartup(MAKEWORD(2,2), &wsadata);
	if ( iResult != 0) {
		printf("StartupError: %d\n", iResult);
		return 1;
	}

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory( &hints, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo( argv[1], DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;

	ptr = result;
	
	ConnectSocket = socket(ptr ->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if(ConnectSocket == INVALID_SOCKET) {
		printf("Error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = connect(ConnectSocket, ptr->ai_addr, ptr->ai_addrlen);

	if(iResult==SOCKET_ERROR) {
		printf("Error:%d\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	if(ConnectSocket == INVALID_SOCKET) {
		printf("Error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	int recvbuflen = DEFAULT_BUFLEN;

	char msg[DEFAULT_BUFLEN];

	char remsg[DEFAULT_BUFLEN];
	printf("send:");
	gets_s(msg);

	iResult = send(ConnectSocket, msg, strlen(msg), 0);
	if(iResult == SOCKET_ERROR) {
		printf("send error:%d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	if (msg[0] == 'q') {
		printf("Exit!\n");
		return 0;
	}

	iResult = recv( ConnectSocket, remsg, recvbuflen, 0);
	if(iResult == SOCKET_ERROR) {
		printf("recverror!: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	printf("%s\n", remsg);
	printf("================================================\n");
	do {
		printf("send:");
		gets_s(msg);
		iResult = send(ConnectSocket, msg, strlen(msg), 0);
		if(iResult == SOCKET_ERROR) {
			printf("send error:%d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
		return 1;
		}

		if (msg[0] == 'q')
			break;

		iResult = recv( ConnectSocket, remsg, recvbuflen, 0);
		if(iResult == SOCKET_ERROR) {
			printf("recverror!: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}

		printf("Message: %s\n", remsg);
		printf("================================================\n");
		if(remsg[0] == 'q') {
			printf("Sever is closing.\n");
			break;
		}
	} while(1);
	iResult = shutdown(ConnectSocket, SD_SEND);
	if(iResult == SOCKET_ERROR) {
		printf("shutdown error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}