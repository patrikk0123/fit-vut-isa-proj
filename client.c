#include <unistd.h>

#include "client/dns_sender.h"
#include "client/utils/args.h"

int main(int argc, char* argv[])
{
  args_t args;
  get_args(argc, argv, &args);
  validate_args(&args);

  int server_socket = connect_to_dns_server(args.dns_ip);
  send_file(args.dst_filename, args.src_filepath, server_socket, args.base_host);

  close(server_socket);

  return 0;
}
