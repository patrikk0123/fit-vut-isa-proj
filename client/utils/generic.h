/**
 * @file
 * @brief Generic functionality module API.
 */

#ifndef __GENERIC_H__
#define __GENERIC_H__

#define ARG_ERR      1
#define FILE_ERR     2
#define SENDER_ERR   3
#define RECEIVER_ERR 4
#define BASENAME_ERR 5
#define DSTFILE_ERR  6
#define INTERNAL_ERR 99

#define SMALL_BUFF_SIZE 100
#define BIG_BUFF_SIZE   1024

/**
 * Prints message to the STDERR.
 * @param message - message to print to the STDERR
 */
void print_error(char* message);

/**
 * Prints message to the STDERR and exits with error code.
 * @param error_code - error code to exit with
 * @param message    - message to print to the STDERR
 */
void error_exit(int error_code, char* message);

/**
 * Strips whitespace characters from the right side of a string.
 * @param string - string to edit
 */
void str_strip_right(char* string);

#endif   // __GENERIC_H__
