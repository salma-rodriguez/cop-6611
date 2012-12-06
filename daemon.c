#include <pthread.h>

const char *dev = "/dev/sde";

int func(void *arg) {
	while(1) {
		if(fopen(dev, O_RDONLY, O400) >= 0)
			continue;
		printf("device %s not available\n", dev);
	}
}

int main(void) {
	pthread_t thread;
	(void) pthread_create(&thread, NULL, func, NULL);
	(void) pthread_join(thread, NULL);
	printf("exiting all threads...\n");
	return 0;
}
