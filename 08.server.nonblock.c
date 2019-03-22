#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

int main(int argc, char **argv) {
	int sockfd, clientfd;
	socklen_t clen;
	struct sockaddr_in saddr, caddr;
	unsigned short port = 8785;

	if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Error creating socket\n");
		exit(1);
	}
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
	int fl = fcntl(sockfd, F_GETFL, 0);
	fl |= O_NONBLOCK;
	fcntl(sockfd, F_SETFL, fl);
	
	memset(&saddr, 0, sizeof(saddr)); 
	saddr.sin_family = AF_INET; 
	saddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	saddr.sin_port = htons(port);
	
	if (bind(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
		printf("Error binding\n");
		perror("bind");
		exit(1);
	}

	if (listen(sockfd, 5) < 0) { 
		printf("Error listening\n");
		exit(1);
	}

	clen=sizeof(caddr);
	while (1) {
		clientfd = accept(sockfd, (struct sockaddr *) &caddr, &clen);
		if (clientfd > 0) {
			int fl = fcntl(clientfd, F_GETFL, 0);
			fl |= O_NONBLOCK;
			fcntl(clientfd, F_SETFL, fl);
			printf("Successfully accepted a client\n");
			while (1) {
				char s[200];
				memset(s, 0, 200);
				if (read(clientfd, s, 200) > 0) {
					printf("Client says: %s\n", s);
				}
				struct pollfd input[1] = {{.fd = 0, .events = POLLIN}};
				if (poll(input, 1, 100) > 0) {
		      		fgets(s, 200, stdin);
			      	s[strlen(s) - 1] = 0;
			      	write(clientfd, s, strlen(s));
			    }
			}
		}
	}
	return 1;
}