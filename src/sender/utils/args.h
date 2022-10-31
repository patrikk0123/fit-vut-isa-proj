/**
 * @file
 * @brief Argument parsing module API.
 */

#ifndef __ARGS_H__
#define __ARGS_H__

/**
 * @struct args_t
 * @brief Program arguments
 */
typedef struct {
  char* dns_ip;
  char* base_host;
  char* dst_filename;
  char* src_filepath;
} args_t;

/**
 * Parses program arguments data.
 * Exits with error code if reduntant arguments is found or argument is missing.
 * @param argc - program arguments count
 * @param argv - program arguments array
 * @param args - pointer to structure into which arguments are stored
 */
void get_args(int argc, char* argv[], args_t* args);

/**
 * Gets string from program arguments.
 * Exits with error code if missing.
 * @param pos  - argument position
 * @param argc - program arguments count
 * @param argv - program arguments array
 * @return Pointer to string argument
 */
char* get_arg_str(int argc, char* argv[], int pos);

/**
 * Validates arguments (whether are all mandatory included).
 * In case of an invalid argument, exits with error code.
 * @param args - pointer to arguments structure
 */
void validate_args(args_t* args);

/**
 * Prints program usage help to the console.
 */
void print_help();

#endif   // __ARGS_H__
