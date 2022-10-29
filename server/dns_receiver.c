/**
 * @file
 * @brief DNS receiver module.
 */

#include "dns_receiver.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "dns.h"
#include "dns_response.h"
#include "utils/generic.h"

int init_dns_server()
{
  int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (server_socket == -1) {
    error_exit(RECEIVER_ERR, "failed initialization - could not create socket");
  }
  int opt_val = 1;
  setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | 15, &opt_val, sizeof(opt_val));

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(DNS_PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
    error_exit(RECEIVER_ERR, "failed initialization - could not bind the socket");
  }

  if (listen(server_socket, LISTEN_QUEUE_LEN) == -1) {
    error_exit(RECEIVER_ERR, "failed initialization - could not create listening queue");
  };

  return server_socket;
}

void start_dns_server(int server_socket, char* dst_dirpath, char* base_host)
{
  // server loop
  while (true) {
    struct sockaddr_in client_addr;
    socklen_t socket_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &socket_len);
    if (client_socket == -1) {
      print_error("could not accept client connection");
      continue;
    }

    serve_client(client_socket, dst_dirpath, base_host);
    close(client_socket);
  }
}

void serve_client(int client_socket, char* dst_dirpath, char* base_host)
{
  bool first_cycle = true;
  char dns_query[BIG_BUFF_SIZE];
  char dns_response[BIG_BUFF_SIZE];
  FILE* file_fd = NULL;

  // process all queries
  while (true) {
    int recv_code = recv(client_socket, dns_query, 2, MSG_WAITALL);
    if (recv_code == 0 || recv_code == -1) {
      printf("connection with client ended\n");
      break;
    }

    dns_header_t* query_header = (dns_header_t*)dns_query;
    int query_len = ntohs(query_header->len);

    recv_code = recv(client_socket, dns_query, query_len, MSG_WAITALL);
    if (recv_code == 0 || recv_code == -1) {
      printf("connection with client ended\n");
      break;
    }

    char decoded_data[MAX_HOSTNAME_LEN];
    int decoded_data_len = decode_hostname(decoded_data, dns_query, base_host);
    if (first_cycle) {
      char dst_filepath[SMALL_BUFF_SIZE];
      get_dst_filepath(dst_filepath, dst_dirpath, decoded_data);
      printf("accepted file: %s\n", dst_filepath);

      file_fd = fopen(dst_filepath, "w+b");
      if (!file_fd) {
        error_exit(1, "smula neni :DDD");
      }

      first_cycle = false;
    } else {
      fwrite(decoded_data, 1, decoded_data_len, file_fd);
    }

    int len_send = write_response(dns_response, dns_query);
    send(client_socket, dns_response, len_send, 0);
  }

  if (file_fd) {
    fclose(file_fd);
  }
}

void get_dst_filepath(char* dst_filepath, char* dst_dirpath, char* filename)
{
  strcpy(dst_filepath, dst_dirpath);
  if (dst_dirpath[strlen(dst_dirpath) - 1] != '/') {
    strcat(dst_filepath, "/");
  }
  strcat(dst_filepath, filename);
}
