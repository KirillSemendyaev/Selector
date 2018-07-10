#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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

	socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socket_fd == -1) {
		perror("socket");
		return -1;
	}
	
	memset(&target, 0, sizeof(target));
	target.sin_family = AF_INET;
	target.sin_port = htons(atoi(argv[2]));
	target.sin_addr.s_addr = inet_addr(argv[1]);
	target_size = sizeof(target);
	sprintf(buf, "Hello!");
	len = sizeof(buf);
	printf("%s\n", buf);

	ret = sendto(socket_fd, buf, len, MSG_CONFIRM, (struct sockaddr*)&target, target_size);
	if (ret == -1) {
		perror("send");
		return -3;
	}

	ret = recvfrom(socket_fd, buf, len, 0, (struct sockaddr*)&target, &target_size);
	printf("%s\n", buf);

	close(socket_fd);
	return 0;
}


