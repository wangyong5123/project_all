CFLAGS+=-g -Wall -O2
LDLIBS=-levent_core -Wl,--rpath -Wl,/usr/local/lib


#gcc libevent_client.c -o libevent_client1 -I/home/wy/Downloads/libevent-2.1.12-stable/build/include -L/home/wy/Downloads/libevent-2.1.12-stable/build/lib -levent_core -levent -levent_extra -levent_pthreads


all: client server bench server-line-echo server-half-close

clean:
	rm -f client server bench server-line-echo server-half-close

.PHONY: all clean

