#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define HAVE_STRUCT_TIMESPEC
#include <stdio.h>
#include <pthread.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32")

//int sharedData = 0;
//pthread_mutex_t mutex;
//void push() {
//	for (int i = 0; i < 100000; i++) {
//		pthread_mutex_lock(&mutex);
//		sharedData++;
//		pthread_mutex_unlock(&mutex);
//	}
//}
void send_msg(SOCKET csock) {
	while (1) {
		char sbuff[100] = { 0 };

		// strcpy ���ڿ� ���� �Լ�

		printf("���� �޽��� �Է� : ");
		scanf_s("%s", sbuff, 100);
		send(csock, sbuff, 100, 0);
	}
}

void recv_msg(SOCKET csock) {
	while (1) {
		char rbuff[100] = { 0 };
		recv(csock, rbuff, 100, 0);

		printf("%s", rbuff);
	}
}
int main() {

	/*pthread_t threads[2];
	pthread_mutex_init(&mutex, NULL);

	if (pthread_create(&threads[0], NULL, push, NULL) != 0) {
		fprintf(stderr, "Error");
	}

	if (pthread_create(&threads[1], NULL, push, NULL) != 0) {
		fprintf(stderr, "Error");
	}

	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);

	printf("%d\n", sharedData);*/


	WSADATA wsadata;

	// �ʱ�ȭ ���� 0, ���� -1
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
		printf("winsock �ʱ�ȭ ����\n");
		return -1;
	}

	printf("winsock �ʱ�ȭ ����!!\n");

	// 3. ���� ������ �ݱ�.
	// socket(af, type, protocol) - ������ �������ִ� �Լ�.
	// af : �ּ�ü�� (ipv4 - AF_INET, ipv6 - AF_INET6)
	// type : ������ ������ ���� Ÿ�� (TCP / UDP) - SOCK_STREAM, SOCK_DGRAM 
	// protocol : �������� ����(���� 0���� ����)  
	// closesocket(sock): �ش� ������ �ڿ� �ݳ�

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == SOCKET_ERROR) {
		printf("���� ������ ���� �߻�!!\n");
		return -1;
	}

	printf("���� ���� �Ϸ�!!\n");


	// 4. �����ϰ��� �ϴ� ������ ���� �ּ� ����
	SOCKADDR_IN addr; // ���� �ϰ����ϴ� ��� ���� �ּ�
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // 127.0.0.1

	// 5. ������ ���� ��û - connect()
	// connect(sock, addr, addrlen)
	// sock - Ŭ���̾�Ʈ ����
	// addr - ������ ������ �ּҰ� ����ü�� ������
	// addrlen - addr�� ũ��

	if (connect(sock, (SOCKADDR_IN*)&addr, sizeof(addr)) != 0) {
		printf("������ ���� �õ��� ���� �߻�!!\n");
	}

	printf("������ ���� �Ǿ����ϴ�!\n");

	// 6. ������ �ۼ���
	pthread_t t1;
	pthread_t t2;
	pthread_create(&t1, NULL, send_msg, sock);
	pthread_create(&t2, NULL, recv_msg, sock);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);


	closesocket(sock);
	printf("���� �ݱ� �Ϸ�!!\n");

	WSACleanup();
	printf("winsock �ڿ� �ݳ�\n");


	return 0;
}