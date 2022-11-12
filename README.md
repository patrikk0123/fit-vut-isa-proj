# DNS Tunneling Client & Server

DNS tunneling client-server stack. Sender (client) sends data through DNS queries to the receiver (server). Data are encoded using base32 into hostname.

Author: Patrik Korytár\
Login: xkoryt04\
Date: 12/11/2022

## Project files

Project consists of following files:

- ./Makefile
- ./manual.pdf
- ./README.md
- ./src/sender/client.c
- ./src/sender/dns_question.{c,h}
- ./src/sender/dns_sender.{c,h}
- ./src/sender/events/dns_sender_event.{c,h}
- ./src/receiver/server.c
- ./src/receiver/dns_response.{c,h}
- ./src/receiver/dns_receiver.{c,h}
- ./src/receiver/events/dns_receiver_event.{c,h}
- ./src/{sender,receiver}/dns.{c,h}
- ./src/{sender,receiver}/utils/args.{c,h}
- ./src/{sender,receiver}/utils/generic.{c,h}
- ./src/{sender,receiver}/utils/base32.{c,h}

## Prerequisites

Program is made for UNIX operating systems. C11 (or later) is used for the implementation. Implementation uses standard socket and networking libraries (sys/socket.h, arpa/inet.h, netinet/in.h).

## Installation

To compile sender and receiver, run this command in the console in the root directory:

```constole
make
```

## Sender parameters

| Parameter         | Argument          | Description                             | Default value                    |
| ----------------- | ----------------- | --------------------------------------- | -------------------------------- |
| -h \| --help      |                   | print help and exit                     |                                  |
| -u                | [UPSTREAM_DNS_IP] | DNS server to connect to                | default DNS server (resolv.conf) |
| first positional  | [BASE_HOST]       | base hostname domain                    | NOT OPTIONAL                     |
| second positional | [DST_FILENAME]    | name under which to store the sent file | NOT OPTIONAL                     |
| third positional  | [SRC_FILEPATH]    | path to the sent file                   | standard input                   |

## Receiver parameters

| Parameter         | Argument      | Description                                    | Default value |
| ----------------- | ------------- | ---------------------------------------------- | ------------- |
| -h \| --help      |               | print help and exit                            |               |
| first positional  | [BASE_HOST]   | base hostname domain                           | NOT OPTIONAL  |
| second positional | [DST_DIRPATH] | directory into which received files are stored | NOT OPTIONAL  |

## Example usage of sender

**Send 'test.txt' file under name 'mytext.txt' to the server on the loopback (base hostname random.com)**

```console
./dns_sender -u 127.0.0.1 random.com mytext.txt test.txt
```

**Send data from stdin under name 'mytext.txt' to the default dns server (base hostname random.com)**

```console
./dns_sender random.com mytext.txt
```

## Example usage of receiver

**Start receiver server (base hostname random.com) with directory for received files being named 'received'**

```console
./dns_receiver random.com received
```

**Start receiver server (base hostname random.com) with directory for received files being named 'received'**

```console
./dns_receiver random.com received/
```
