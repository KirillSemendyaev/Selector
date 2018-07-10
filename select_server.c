#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("Usage: server [address] [port]\n");
		return -1;
	}

	int udpsocket_fd, tcpsocket_fd, max_fd, con_fd, len, ret;
	char buf[16] = {0};

	udpsocket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	tcpsocket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (udpsocket_fd < 1) {
		perror("socketudp");
		return -2;
	}
	if (tcpsocket_fd < 1) {
		perror("sockettcp");
		return -2;
	}
	
	struct sockaddr_in target, server;
	socklen_t target_size = sizeof(target), server_size = sizeof(server);
	memset(&server, 0, server_size);
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	//server.sin_addr.s_addr = inet_addr(argv[1]);

	ret = bind(udpsocket_fd, (struct sockaddr*)&server, server_size);
	if (ret == -1) {
		perror("bindudp");
		return -3;
	}

	ret = bind(tcpsocket_fd, (struct sockaddr*)&server, server_size);
	if (ret == -1) {
		perror("bindtcp");
		return -4;
	}

	fd_set setmask;

	listen(tcpsocket_fd, 20);
	while (1)	{
		FD_ZERO(&setmask);
		FD_SET(tcpsocket_fd, &setmask);
		FD_SET(udpsocket_fd, &setmask);
		memset(&target, 0, target_size);
		select((tcpsocket_fd > udpsocket_fd ? tcpsocket_fd : udpsocket_fd) + 1, &setmask, NULL, NULL, NULL);
		printf("MAX fd = %d\n", tcpsocket_fd > udpsocket_fd ? tcpsocket_fd : udpsocket_fd);
		if (FD_ISSET(udpsocket_fd, &setmask)) {
			recvfrom(udpsocket_fd, buf, 16, 0, (struct sockaddr *) &target, &target_size);
			printf("UDP connection from %s:%d\n", inet_ntoa(target.sin_addr), ntohs(target.sin_port));
			buf[11] = buf[5];
			buf[12] = buf[6];
			buf[5] = ' ';
			buf[6] = 'w';
			buf[7] = 'o';
			buf[8] = 'r';
			buf[9] = 'l';
			buf[10] = 'd';
			sendto(udpsocket_fd, buf, 16, MSG_CONFIRM, (struct sockaddr *) &target, target_size);
			printf("Replied\n");
		} else {
			printf("NOT UDP ((((((((((((((((((((((     ");
		}
		if (FD_ISSET(tcpsocket_fd, &setmask)) {
			con_fd = accept(tcpsocket_fd, (struct sockaddr *) &target, &target_size);
			printf("TCP connection from %s:%d\n", inet_ntoa(target.sin_addr), ntohs(target.sin_port));
			recv(con_fd, buf, 16, 0);
			printf("%s", buf);
			buf[11] = buf[5];
			buf[12] = buf[6];
			buf[5] = ' ';
			buf[6] = 'w';
			buf[7] = 'o';
			buf[8] = 'r';
			buf[9] = 'l';
			buf[10] = 'd';
			send(con_fd, buf, 16, 0);
			printf("%s", buf);
			printf("Replied\n");
			close(con_fd);
		} else {
				printf("Undefined socket_fd\n");
		}

	}
	close(udpsocket_fd);
	close(tcpsocket_fd);

	return 0;
}



