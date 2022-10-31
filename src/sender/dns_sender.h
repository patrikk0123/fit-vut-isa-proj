/**
 * @file
 * @brief DNS sender module API.
 *
 * Provides functions to connect and send data to the DNS server using DNS tunneling method.
 */

#ifndef __DNS_SENDER_H__
#define __DNS_SENDER_H__

#define BYTES_READ    120
#define BYTES_ENCODED BYTES_READ * 2

#define IP_ADDR_OK           "1.2.3.4"
#define IP_ADDR_BASENAME_ERR "1.1.1.1"
#define IP_ADDR_FILE_ERR     "2.2.2.2"

/**
 * Connects to the DNS server using TCP snd standard DNS port and returns socket file descriptor.
 * If server_ip is NULL, uses default DNS IP (as specified in resolv.conf).
 * If error occurs, exits with error code.
 * @param server_ip - ip address of server to connect to
 * @return Socket using which communicate with server
 */
int connect_to_dns_server(char* server_ip);

/**
 * Sends file to the DNS server through DNS tunneling method.
 * Also reads server responses.
 * If src_filepath is NULL, reads from stdin.
 * If error occurs, exits with error code.
 * @param dst_filename  - name of the sent file on the server
 * @param src_filepath  - path to the file to sent
 * @param server_socket - socket using which communicate with server
 * @param base_host     - base url of DNS query hostname
 */
void send_file(char* dst_filename, char* src_filepath, int server_socket, char* base_host);

/**
 * Reads DNS server response to the sent DNS query and processes it.
 * If error occurs, exits with error code.
 * @param server_socket - socket using which communicate with server
 */
void read_response(int server_socket);

#endif   // __DNS_SENDER_H__
