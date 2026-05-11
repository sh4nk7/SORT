CC = g++
CFLAGS = -O3 -Wall -pthread -std=c++11
LFLAGS = -Lrt -pthread -lrt_pthread

OUT = rt/librt_pthread.a hello-rt

all: $(OUT)

hello-rt: hello-rt.o barrier.o
	$(CC) -o $@ $^ $(LFLAGS)

hello-rt.o: hello-rt.cpp barrier.h
	$(CC) $(CFLAGS) -c hello-rt.cpp

barrier.o: barrier.cpp barrier.h
	$(CC) $(CFLAGS) -c barrier.cpp

rt/librt_pthread.a:
	cd rt; make

clean:
	rm -f *.o *~ hello-rt
	cd rt; make clean
