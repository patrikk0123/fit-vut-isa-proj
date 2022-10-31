/**
 * @file
 * @brief Generic functionality module.
 */

#include "generic.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_error(char* message)
{
  fprintf(stderr, "ERROR: %s\n", message);
}

void error_exit(int error_code, char* message)
{
  print_error(message);
  exit(error_code);
}

void str_strip_right(char* string)
{
  for (int i = strlen(string) - 1; i >= 0; i--) {
    if (!isspace(string[i])) {
      string[i + 1] = '\0';
      break;
    }
  }
}
