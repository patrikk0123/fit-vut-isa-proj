/**
 * @file
 * @brief DNS sender module.
 */

#include "dns_sender.h"

#include <arpa/inet.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "dns.h"
#include "dns_question.h"
#include "utils/base32.h"
#include "utils/generic.h"

int connect_to_dns_server(char* server_ip)
{
  int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (server_socket == -1) {
    error_exit(SENDER_ERR, "failed initialization - could not create socket");
  }

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));

  if (server_ip) {
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
  } else {
    char default_ip[SMALL_BUFF_SIZE];
    get_default_dns_ip(default_ip);
    server_addr.sin_addr.s_addr = inet_addr(default_ip);
  }
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(DNS_PORT);

  if (connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
    error_exit(SENDER_ERR, "failed connection - could not connect to the server");
  }

  return server_socket;
}

void send_file(char* dst_filename, char* src_filepath, int server_socket, char* base_host)
{
  FILE* src_file = src_filepath ? fopen(src_filepath, "rb") : stdin;
  if (!src_file) {
    error_exit(FILE_ERR, "could not open source file");
  }

  bool first_cycle = true;
  uint8_t dns_query[BIG_BUFF_SIZE];

  int chunk_id = 0;
  while (!feof(src_file)) {
    int dns_query_len = 0;

    if (first_cycle) {
      uint8_t encoded_name[SMALL_BUFF_SIZE];
      size_t endoded_len = base32_encode(dst_filename, strlen(dst_filename), encoded_name, SMALL_BUFF_SIZE);

      dns_query_len = write_query(dns_query, encoded_name, endoded_len, base_host, chunk_id);

      first_cycle = false;
    } else {
      uint8_t read_data[BYTES_READ];
      size_t read_len = fread(read_data, 1, BYTES_READ, src_file);
      uint8_t encoded_data[BYTES_ENCODED];
      size_t encoded_len = base32_encode(read_data, read_len, encoded_data, BYTES_ENCODED);

      dns_query_len = write_query(dns_query, encoded_data, encoded_len, base_host, chunk_id);
    }

    chunk_id++;

    if (send(server_socket, dns_query, dns_query_len, 0) == -1) {
      error_exit(SENDER_ERR, "could not send data to the server");
    }
    read_response(server_socket);
  }

  printf("file successfuly sent\n");

  if (src_file != stdin) {
    fclose(src_file);
  }
}

void read_response(int server_socket)
{
  uint8_t dns_response[BIG_BUFF_SIZE];

  int recv_code = recv(server_socket, dns_response, 2, MSG_WAITALL);
  if (recv_code <= 0) {
    error_exit(SENDER_ERR, "connecton with server ended");
  }

  dns_header_t* response_header = (dns_header_t*)dns_response;
  int response_len = ntohs(response_header->len);

  recv_code = recv(server_socket, dns_response, response_len, MSG_WAITALL);
  if (recv_code <= 0) {
    error_exit(SENDER_ERR, "connecton with server ended");
  }

  if (response_header->rcode) {
    error_exit(SENDER_ERR, "server rejected sent file");
  }

  char ip_addr[INET_ADDRSTRLEN];
  read_response_ip(ip_addr, dns_response);

  if (!strcmp(ip_addr, IP_ADDR_BASENAME_ERR)) {
    error_exit(BASENAME_ERR, "invalid basename");
  }
  if (!strcmp(ip_addr, IP_ADDR_FILE_ERR)) {
    error_exit(DSTFILE_ERR, "file could not be created on the server");
  }
}
