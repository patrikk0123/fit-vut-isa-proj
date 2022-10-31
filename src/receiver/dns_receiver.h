/**
 * @file
 * @brief DNS receiver module API.
 *
 * Provides functions to create DNS server, parse DNS tunneling queries,
 * create requested files and respond to the client.
 */

#ifndef __DNS_RECEIVER_H__
#define __DNS_RECEIVER_H__

#include <stdint.h>

#define BYTES_READ    120
#define BYTES_ENCODED BYTES_READ * 2

#define LISTEN_QUEUE_LEN 5

#define IP_ADDR_OK           "1.2.3.4"
#define IP_ADDR_BASENAME_ERR "1.1.1.1"
#define IP_ADDR_FILE_ERR     "2.2.2.2"

/**
 * Initializes server socket and listening queue.
 * If error occurs, exits with error code.
 * @return Server socket
 */
int init_dns_server();

/**
 * Runs server loop and serves to the clients.
 * @param server_socket - server socket
 * @param dst_dirpath   - path where received files are stored
 * @param base_host     - base url of DNS query hostname
 */
void start_dns_server(int server_socket, char* dst_dirpath, char* base_host);

/**
 * Accepts DNS queries containg file and creates said file.
 * @param client_socket - socket using which communicate with client
 * @param dst_dirpath   - path where received files are stored
 * @param base_host     - base url of DNS query hostname
 */
void serve_client(int client_socket, char* dst_dirpath, char* base_host);

/**
 * Sends DNS reply with IP address to the query.
 * @param client_socket - socket using which communicate with client
 * @param dns_query     - DNS query to send response to
 * @param ip_addr       - IP address sent as reply
 */
void send_response(int client_socket, uint8_t* dns_query, char* ip_addr);

/**
 * Constructs destination filepath.
 * @param dst_filename - buffer to which constructed filepath is stored
 * @param dst_dirpath  - dirpath to the filename
 * @param filename     - filename
 */
void get_dst_filepath(char* dst_filepath, char* dst_dirpath, char* filename);

#endif   // __DNS_RECEIVER_H__
