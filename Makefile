CC=gcc
CFLAGS=-std=c11
TESTDIR=test

all: sender receiver

sender:  src/sender/*.c src/sender/*.h src/sender/*/*.c src/sender/*/*.h
	$(CC) $(CFLAGS) src/sender/*.c src/sender/*/*.c -o dns_sender

receiver: src/receiver/*.c src/receiver/*.h src/receiver/*/*.c src/receiver/*/*.h
	$(CC) $(CFLAGS) src/receiver/*.c src/receiver/*/*.c -o dns_receiver

tar:
	tar -czvf xkoryt04.tar.gz src/* Makefile

zip:
	zip -r xkoryt04.zip src/* Makefile

clean:
	rm -f dns_sender
	rm -f dns_receiver
	rm -f ${TESTDIR}/*
