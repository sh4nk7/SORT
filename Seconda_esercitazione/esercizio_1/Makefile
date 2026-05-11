CC = g++
CFLAGS = -O3 -Wall -pthread -std=c++11
LFLAGS = -Lrt -pthread -lrt_pthread

OUT = rt/librt_pthread.a hello

all: $(OUT)

hello: hello.o barrier.o
	$(CC) -o $@ $^ $(LFLAGS)

hello.o: hello.cpp barrier.h
	$(CC) $(CFLAGS) -c hello.cpp

barrier.o: barrier.cpp barrier.h
	$(CC) $(CFLAGS) -c barrier.cpp

rt/librt_pthread.a:
	cd rt; make

clean:
	rm -f *.o *~ hello
	cd rt; make clean
