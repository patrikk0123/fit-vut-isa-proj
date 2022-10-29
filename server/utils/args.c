/**
 * @file
 * @brief Argument parsing module.
 */

#include "args.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../client/utils/generic.h"

void get_args(int argc, char* argv[], args_t* args)
{
  memset(args, 0, sizeof(args_t));

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
      print_help();
      exit(0);
    }

    if (!args->base_host) {
      args->base_host = get_arg_str(argc, argv, i);
    } else if (!args->dst_dirpath) {
      args->dst_dirpath = get_arg_str(argc, argv, i);
    } else {
      error_exit(ARG_ERR, "redundant argument");
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

  if (!args->dst_dirpath) {
    print_help();
    error_exit(ARG_ERR, "missing argument");
  }
}

void print_help()
{
  printf("./dns_receiver {BASE_HOST} {DST_DIRPATH}\n");
  printf("BASE_HOST   - base hostname domain\n");
  printf("DST_DIRPATH - directory into which received files are stored\n");
}
