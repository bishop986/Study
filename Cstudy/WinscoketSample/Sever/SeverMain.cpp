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
	
	iResult = WSAStartup(MAKEWORD(2,2), &wsadata); /*winsock版本协商并初始化相关DLL使用*/
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

	iResult = getaddrinfo( NULL, DEFAULT_PORT, &hints, &result);/*按照hints的设置与指定端口解析主机地址并保存到result*/
	if (iResult != 0) {
		printf("getaddrinfo error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	SOCKET ListenSocket = INVALID_SOCKET;			/*创建监听套接字*/

	ListenSocket = socket( result->ai_family, result->ai_socktype, result->ai_protocol); /*套接字初始化设定*/
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);/*绑定到指定地址上进行监听*/
	if (iResult == SOCKET_ERROR)
	{
		printf("bind error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	if( listen( ListenSocket, SOMAXCONN) == SOCKET_ERROR) { /*开始监听，此函数已经分配至异步线程开始监听端口*/
		printf("listen error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	SOCKET ClientSocket;						/*创建临时套接字接受客户端数据*/

	ClientSocket = accept( ListenSocket, NULL, NULL);		/*接收监听到的套接字*/
	if (ClientSocket == INVALID_SOCKET) {
		printf(" accept error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	closesocket(ListenSocket);						/*已经建立连接，监听使命完成关闭*/

	char recvbuf[DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN];
	int iSendResult;
	int buflen = DEFAULT_BUFLEN;

	do {

		iResult = recv(ClientSocket, recvbuf, buflen, 0);		/*通过客户端的套接字接收客户端信息*/
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
		iSendResult = send(ClientSocket, sendbuf, strlen(sendbuf), 0); /*通过已经建立的套接字来发送信息*/
		if ( iSendResult == SOCKET_ERROR) {
			printf("send error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
		if( sendbuf[0] == 'q')
			break;
	} while(1);

	iResult = shutdown( ClientSocket, SD_SEND);				/*会话结束关闭链接*/
	if(iResult == SOCKET_ERROR) {
		printf("shutdown error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ClientSocket);							/*关闭套接字*/
	WSACleanup();								/*清除数据释放内存*/

	return 0;
}