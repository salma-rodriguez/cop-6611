#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#define BUFSIZE 1024
#define DEBUG 1

#if DEBUG
#define DPRINTF(s, arg...) printf(s "\n", ##arg)
#else
#define DPRINTF(s, arg...) /* do nothing */
#endif

int portno;
char *hostname;

void error(char *msg) {
	perror(msg);
	exit(0);
}

int send_req(char *hostname, int portno) {
	int sockfd, n;
	char *message, buf[BUFSIZE];
	struct sockaddr_in serveraddr;
	struct hostent *server;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("ERROR opening socket");
		exit(0);
	}

	server = gethostbyname(hostname);
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host as %s\n", hostname);
		exit(0);
	}

	message = "wazzup\n";

	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		(char *)&serveraddr.sin_addr.s_addr, server->h_length);
	serveraddr.sin_port = htons(portno);

	if (connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
		error("ERROR connecting");

	if((n = write(sockfd, message, strlen(message))) < 0)
		error ("ERROR writing to socket");

	bzero(buf, BUFSIZE);

	if ((n = read(sockfd, buf, BUFSIZE)) < 0)
		error("ERROR receiving acknowledgement from server");

	DPRINTF("Echo from server: %s", buf);
	close(sockfd);
	return 0;
}

int main(int argc, char **argv) {
	int fd;
	char sp[1];
	if (argc != 3) 
		fprintf(stderr, "usage: %s <hostname> <port>\n", argv[0]);
	sp[0] = 0x30;
	hostname = argv[1];
	portno = atoi(argv[2]);

	while(1)
		send_req(hostname, portno);
}
