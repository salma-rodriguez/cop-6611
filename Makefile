SRCS = dm-cache.c dm.h
APPS = web-apps
EXTRA_CFLAGS :=-g
obj-m += dm-cache.o

all:	
	make -C $(APPS)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules



TAGS: $(SRCS)
	find . -regex ".*\.[cChH]\(pp\)?" -print | etags -

clean:
	make -C $(APPS) clean
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
