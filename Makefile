all:	
	gcc -Wall -o SELECT_SERVER select_server.c
	gcc -Wall -o UDP_CLIENT udp_client.c
	gcc -Wall -o TCP_CLIENT tcp_client.c
clean:
	rm TCP_CLIENT UDP_CLIENT SELECT_SERVER

