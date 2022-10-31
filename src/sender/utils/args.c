/**
 * @file
 * @brief Argument parsing module.
 */

#include "args.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "generic.h"

void get_args(int argc, char* argv[], args_t* args)
{
  memset(args, 0, sizeof(args_t));

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
      print_help();
      exit(0);
    }

    if (!strcmp(argv[i], "-u")) {
      args->dns_ip = get_arg_str(argc, argv, i + 1);
      i++;
    } else {
      if (!args->base_host) {
        args->base_host = get_arg_str(argc, argv, i);
      } else if (!args->dst_filename) {
        args->dst_filename = get_arg_str(argc, argv, i);
      } else if (!args->src_filepath) {
        args->src_filepath = get_arg_str(argc, argv, i);
      } else {
        error_exit(ARG_ERR, "redundant argument");
      }
    }
  }
}

char* get_arg_str(int argc, char* argv[], int pos)
{
  if (pos < argc) {
    return argv[pos];
  }

  error_exit(ARG_ERR, "missing argument");
}

void validate_args(args_t* args)
{
  if (!args->base_host) {
    print_help();
    error_exit(ARG_ERR, "missing argument");
  }

  if (!args->dst_filename) {
    print_help();
    error_exit(ARG_ERR, "missing argument");
  }
}

void print_help()
{
  printf("./dns_sender [-u UPSTREAM_DNS_IP] {BASE_HOST} {DST_FILENAME} [SRC_FILEPATH]\n");
  printf("UPSTREAM_DNS_IP - DNS server to connect to (defaults to default DNS server)\n");
  printf("BASE_HOST       - base hostname domain\n");
  printf("DST_FILENAME    - name under which to store the sent file\n");
  printf("SRC_FILEPATH    - path to the sent file (defaults to STDIN)\n");
}
