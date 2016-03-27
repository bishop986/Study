#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <stdio.h>

#pragma comment(lib,"Ws2_32.lib")
#pragma warning(disable: 4996)

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

int main()
{
	WSAData wsadata;
	int iResult;
	
	iResult = WSAStartup(MAKEWORD(2,2), &wsadata); /*winsock�汾Э�̲���ʼ�����DLLʹ��*/
	if ( iResult != 0) {
		printf("StartupError: %d\n", iResult);
		return 1;
	}

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory( &hints, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo( NULL, DEFAULT_PORT, &hints, &result);/*����hints��������ָ���˿ڽ���������ַ�����浽result*/
	if (iResult != 0) {
		printf("getaddrinfo error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	SOCKET ListenSocket = INVALID_SOCKET;			/*���������׽���*/

	ListenSocket = socket( result->ai_family, result->ai_socktype, result->ai_protocol); /*�׽��ֳ�ʼ���趨*/
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);/*�󶨵�ָ����ַ�Ͻ��м���*/
	if (iResult == SOCKET_ERROR)
	{
		printf("bind error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	if( listen( ListenSocket, SOMAXCONN) == SOCKET_ERROR) { /*��ʼ�������˺����Ѿ��������첽�߳̿�ʼ�����˿�*/
		printf("listen error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	SOCKET ClientSocket;						/*������ʱ�׽��ֽ��ܿͻ�������*/

	ClientSocket = accept( ListenSocket, NULL, NULL);		/*���ռ��������׽���*/
	if (ClientSocket == INVALID_SOCKET) {
		printf(" accept error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	closesocket(ListenSocket);						/*�Ѿ��������ӣ�����ʹ����ɹر�*/

	char recvbuf[DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN];
	int iSendResult;
	int buflen = DEFAULT_BUFLEN;

	do {

		iResult = recv(ClientSocket, recvbuf, buflen, 0);		/*ͨ���ͻ��˵��׽��ֽ��տͻ�����Ϣ*/
		recvbuf[iResult] = '\0';
		printf("recv: %d\n", iResult);
		printf("str: %d\n", strlen(recvbuf));
		if( recvbuf[0] == 'q'){
			printf("Client is down\n");
		}
		if(iResult > 0) {
			printf("message: %s\n", recvbuf);
			printf("==================================\n");
		} else if( recvbuf[0] == 'q') {
			printf("Client Closing......");
		} else {
			printf("recv error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
		printf("Re: ");
		gets_s(sendbuf);
		sendbuf[strlen(sendbuf)] = '\0';
		iSendResult = send(ClientSocket, sendbuf, strlen(sendbuf), 0); /*ͨ���Ѿ��������׽�����������Ϣ*/
		if ( iSendResult == SOCKET_ERROR) {
			printf("send error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
		if( sendbuf[0] == 'q')
			break;
	} while(1);

	iResult = shutdown( ClientSocket, SD_SEND);				/*�Ự�����ر�����*/
	if(iResult == SOCKET_ERROR) {
		printf("shutdown error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ClientSocket);							/*�ر��׽���*/
	WSACleanup();								/*��������ͷ��ڴ�*/

	return 0;
}