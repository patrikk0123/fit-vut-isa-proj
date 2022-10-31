/**
 * @file
 * @brief DNS module.
 */

#include "dns.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "utils/generic.h"

void get_default_dns_ip(uint8_t* buffer)
{
  FILE* resolv_fd = fopen("/etc/resolv.conf", "r");
  if (!resolv_fd) {
    error_exit(FILE_ERR, "could not open resolv.conf file");
  }

  while (!feof(resolv_fd)) {
    char* look_for = "nameserver";   // line of default DNS starts with this string
    char line_buffer[SMALL_BUFF_SIZE];

    fgets(buffer, SMALL_BUFF_SIZE, resolv_fd);
    if (!strncmp(buffer, look_for, strlen(look_for))) {
      char* found_ip = buffer + strlen(look_for) + 1;
      str_strip_right(found_ip);
      strcpy(buffer, found_ip);
      break;
    }
  }

  fclose(resolv_fd);
}
