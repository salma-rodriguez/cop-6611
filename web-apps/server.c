#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX 100
#define BUFSIZE 1024
#define MIN(a,b) a < b ? a:b
#define DEBUG 1

#if DEBUG
#define DPRINTF(s, arg...) printf(s "\n", ##arg);
#else
#define DPRINTF(s, arg...) /* do nothing */
#endif

const char *dev = "/dev/sde";

void handler(int); /* function prototype */

char* concat(int numargs, const char* str, ...) {
        char *s;
        char *cat;
        va_list argp;
        s = malloc(1024 * sizeof(char));
        va_start(argp, str);
        while(numargs--) {
                cat = va_arg(argp, char *);
                s = strcat(s,cat);
        }
        va_end(argp);
        return s;
}

void error(const char *msg)
{
	perror(concat(2, msg, "\n"));
	exit(1);
}

void* func(void *arg) {
	char buf[1];
	int fd = *(int *)arg;
	while (read(fd, buf, 1));
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t thread;
	int fd, sockfd, newsockfd, portno, pid;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	if (argc < 2) {
		error("ERROR, no port provided");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
				sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	listen(sockfd, MAX);
	clilen = sizeof(cli_addr);

	while (1) {
		

			newsockfd = accept(sockfd,
				(struct sockaddr *) &cli_addr, &clilen);

			if (newsockfd < 0)
				error("ERROR on accept");
			pid = fork();
			if (pid < 0)
				error("ERROR on fork");
			if (pid == 0)  {
				handler(newsockfd);
				exit(0);
			}
			else close(newsockfd); 
		} /* else */
	} /* end of while */
	close(sockfd);
	return 0; /* we never get here */
}

/********************* HANDLER() *********************
  Separate instance for each connection
  Handles all communication when connected to sock
 *****************************************************/
void handler (int sock)
{
        int fd, n;
        char buf[BUFSIZE];
        bzero(buf, BUFSIZE);

	if ((fd = open(dev, O_RDONLY, 0400)) >= 0)
	{
		pthread_create(&thread, NULL, &func, &fd);
		do {
			sleep(5);
			if (!pthread_kill(thread, 0))
			printf("thread still running...\n");
		} while(!pthread_kill(thread, 0));

        if ((n = read(sock, buf, BUFSIZE - 1)) < 0)
                error("ERROR reading from socket");
        DPRINTF("message received: %s", buf);
	
	sprintf(buf, "device %s is available\n", dev);

	/* if((fd = open(dev, O_RDONLY, 0400)) < 0)
		sprintf(buf, "device %s is not available\n", dev);
	else
		sprintf(buf, "device %s is available\n", dev); */

        // send message back to client
        if ((n = write(sock, buf, strlen(buf))) < 0)
		error("ERROR writing to socket");
}
