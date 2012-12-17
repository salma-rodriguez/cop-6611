/*
 * Simple client to test if iscsi is working
 *
 */

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define MAX_SIZE 1024

char buf[MAX_SIZE + 1];
const char *dev = "/dev/sde";

void* func(void *arg) {
	int fd;
	bzero(buf, MAX_SIZE);
	if((fd = open(dev, O_RDONLY, 0400)) < 0)
		printf("device %s not available\n", dev);
	// if (read(fd, buf, MAX_SIZE) < 0)
		// printf("unable to read from %s\n", dev);
	printf("content: %s\n", buf);
	close(fd);
	return NULL;
}

int main(void) {
	pthread_t thread;
	while(1) {
		(void) pthread_create(&thread, NULL, &func, NULL);

		do {
			sleep(5);
			if(!pthread_kill(thread, 0))
				printf("thread still running...\n");
		} while(!pthread_kill(thread, 0));

		(void) pthread_join(thread, NULL);
		printf("exiting thread...\n");
	}
	return 0;
}
