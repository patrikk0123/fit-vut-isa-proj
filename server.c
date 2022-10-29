#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "server/dns.h"
#include "server/dns_receiver.h"
#include "server/dns_response.h"
#include "server/utils/args.h"
#include "server/utils/generic.h"

int main(int argc, char *argv[])
{
  args_t args;
  get_args(argc, argv, &args);
  validate_args(&args);

  int server_socket = init_dns_server();
  start_dns_server(server_socket, args.dst_dirpath, args.base_host);

  close(server_socket);

  return 0;
}
