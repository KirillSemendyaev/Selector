#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	if (argc != 3) {
		perror("args");
		return -2;
	}
	int socket_fd, len, ret;
	struct sockaddr_in target;
	socklen_t target_size;

	char buf[16] = {0};

	socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_fd == -1) {
		perror("socket");
		return -1;
	}
	
	memset(&target, 0, sizeof(target));
	target.sin_family = AF_INET;
	target.sin_port = htons(atoi(argv[2]));
	target.sin_addr.s_addr = inet_addr(argv[1]);
	target_size = sizeof(target);
	len = sprintf(buf, "Hello!\n");
	printf("%s", buf);

	ret = connect(socket_fd, (struct sockaddr*)&target, target_size);
	if (ret == -1) {
		perror("connect");
		return -3;
	}
	ret = send(socket_fd, buf, 16, 0);
	if (ret < 1) {
		perror("send");
		return -4;
	}

	ret = recv(socket_fd, buf, 16, 0);
	if (ret < 1) {
		perror("recv");
		return -5;
	}
	printf("%s", buf);

	close(socket_fd);
	return 0;
}


