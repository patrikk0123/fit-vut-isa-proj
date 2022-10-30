/**
 * @file
 * @brief DNS question module.
 */

#include "dns_question.h"

#include <arpa/inet.h>
#include <stdint.h>
#include <string.h>

#include "dns.h"

int write_query_header(uint8_t* dns_buffer)
{
  dns_header_t* dns_header = (dns_header_t*)dns_buffer;

  memset(dns_header, 0, sizeof(dns_header_t));

  dns_header->rd = 1;
  dns_header->qdcount = htons(1);

  return sizeof(dns_header_t);
}

int write_query_hostname(uint8_t* dns_buffer, uint8_t* encoded_data, int data_len, uint8_t* basename)
{
  int hostname_len = write_query_hostname_data(dns_buffer, encoded_data, data_len);
  hostname_len += write_query_hostname_basename(dns_buffer, hostname_len, basename);

  return hostname_len;
}

int write_query_hostname_data(uint8_t* dns_buffer, uint8_t* encoded_data, int data_len)
{
  uint8_t* dns_name = dns_buffer + sizeof(dns_header_t);

  int label_cnt = data_len / 63 + (data_len % 63 != 0 ? 1 : 0);
  for (int i = 0; i < label_cnt; i++) {
    int label_len = i == label_cnt - 1 ? data_len % 63 : 63;
    *dns_name = (char)label_len;
    memcpy(dns_name + 1, encoded_data + i * 63, label_len);
    dns_name += label_len + 1;
  }

  return label_cnt + data_len;
}

int write_query_hostname_basename(uint8_t* dns_buffer, int hostname_len, uint8_t* basename)
{
  uint8_t* dns_basename = dns_buffer + sizeof(dns_header_t) + hostname_len;
  int basename_len = strlen(basename);

  memcpy(dns_basename + 1, basename, basename_len + 1);

  uint8_t* last_dot = dns_basename;
  int label_len = 0;
  for (int i = 1; i <= basename_len + 1; i++) {
    if (dns_basename[i] == '.' || dns_basename[i] == '\0') {
      *last_dot = (char)label_len;
      last_dot = dns_basename + i;
      label_len = 0;
    } else {
      label_len++;
    }
  }

  return basename_len + 2;
}

int write_query_question(uint8_t* dns_buffer, int hostname_len)
{
  dns_header_t* dns_header = (dns_header_t*)dns_buffer;
  dns_question_t* dns_question = (dns_question_t*)(dns_buffer + sizeof(dns_header_t) + hostname_len);

  dns_question->qtype = htons(QTYPE_A);
  dns_question->qclass = htons(QCLASS_IN);

  return sizeof(dns_question_t);
}

void write_query_len(uint8_t* dns_buffer, int hostname_len)
{
  dns_header_t* dns_header = (dns_header_t*)dns_buffer;
  dns_header->len = htons(sizeof(dns_header_t) + hostname_len + sizeof(dns_question_t) - 2);
}

int write_query(uint8_t* dns_buffer, uint8_t* encoded_data, int data_len, uint8_t* basename)
{
  int len_send = 0;
  len_send += write_query_header(dns_buffer);

  int host_len = write_query_hostname(dns_buffer, encoded_data, data_len, basename);
  len_send += host_len;

  len_send += write_query_question(dns_buffer, host_len);
  write_query_len(dns_buffer, host_len);

  return len_send;
}

void read_response_ip(uint8_t* ip_addr, uint8_t* dns_buffer)
{
  uint8_t* hostname = dns_buffer + sizeof(dns_udp_header_t);
  while (*hostname) {
    hostname++;
  }
  hostname++;

  dns_resource_record_t* response = (dns_resource_record_t*)hostname;
  inet_ntop(AF_INET, &(response->rdata), ip_addr, INET_ADDRSTRLEN);
}
