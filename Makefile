CC=gcc
CFLAGS=-std=c11

dns_sender:  src/sender/*.c src/sender/*.h src/sender/*/*.c src/sender/*/*.h
	$(CC) $(CFLAGS) src/sender/*.c src/sender/*/*.c -o dns_sender

dns_receiver: src/receiver/*.c src/receiver/*.h src/receiver/*/*.c src/receiver/*/*.h
	$(CC) $(CFLAGS) src/receiver/*.c src/receiver/*/*.c -o dns_receiver
