#include <unistd.h>

#include "dns_receiver.h"
#include "utils/args.h"

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
