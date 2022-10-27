CC=gcc
CFLAGS=-std=c11

dns-sender: client.c client/*.c client/*.h client/*/*.c client/*/*.h
	$(CC) $(CFLAGS) client.c client/*.c client/*/*.c -o dns-sender

dns-receiver: server.c server/*.c server/*.h server/*/*.c server/*/*.h
	$(CC) $(CFLAGS) server.c server/*.c server/*/*.c -o dns-receiver
