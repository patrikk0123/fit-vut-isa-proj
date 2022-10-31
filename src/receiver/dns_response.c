/**
 * @file
 * @brief DNS response module.
 */

#include "dns_response.h"

#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "dns.h"
#include "utils/base32.h"
#include "utils/generic.h"

void copy_hostname(uint8_t* hostname_to, uint8_t* dns_buffer)
{
  uint8_t* hostname_from = dns_buffer + sizeof(dns_udp_header_t);

  int offset = 0;
  while (hostname_from[offset] != '\0') {
    uint16_t len = hostname_from[offset];
    memcpy(hostname_to + offset, hostname_from + offset + 1, len);
    offset += len;
    hostname_to[offset++] = '.';
  }

  if (offset) {
    hostname_to[offset - 1] = '\0';
  }
}

int strip_basename(uint8_t* hostname, char* basename)
{
  int hostname_len = strlen(hostname);
  int basename_len = strlen(basename);

  if (hostname_len <= basename_len) {
    return -1;
  }

  char* hostname_suffix = hostname + hostname_len - basename_len;

  if (strcmp(hostname_suffix, basename)) {
    return -1;
  }

  *(hostname_suffix - 1) = '\0';
  return 0;
}

void dedot_hostname(uint8_t* hostname)
{
  int offset = 0;
  for (int i = 0; hostname[i] != '\0'; i++) {
    if (hostname[i] != '.') {
      hostname[offset++] = hostname[i];
    }
  }

  hostname[offset] = '\0';
}

int decode_hostname(uint8_t* decoded, uint8_t* dns_buffer, char* basename)
{
  char hostname[MAX_HOSTNAME_LEN];
  copy_hostname(hostname, dns_buffer);
  if (strip_basename(hostname, basename) == -1) {
    return -1;
  }
  dedot_hostname(hostname);

  return base32_decode(hostname, decoded, MAX_HOSTNAME_LEN);
}

int write_response_header(uint8_t* rr_dns_buffer, uint8_t* query_dns_buffer)
{
  dns_header_t* rr_dns_header = (dns_header_t*)rr_dns_buffer;
  dns_udp_header_t* query_dns_header = (dns_udp_header_t*)query_dns_buffer;

  memset(rr_dns_header, 0, sizeof(dns_header_t));

  rr_dns_header->id = query_dns_header->id;
  rr_dns_header->qr = 1;
  rr_dns_header->opcode = query_dns_header->opcode;
  rr_dns_header->rd = query_dns_header->rd;
  rr_dns_header->ancount = htons(1);

  return sizeof(dns_header_t);
}

int write_response_hostname(uint8_t* rr_dns_buffer, uint8_t* query_dns_buffer)
{
  uint8_t* hostname_to = rr_dns_buffer + sizeof(dns_header_t);
  uint8_t* hostname_from = query_dns_buffer + sizeof(dns_udp_header_t);

  int i;
  for (i = 0; hostname_from[i] != '\0'; i++) {
    hostname_to[i] = hostname_from[i];
  }
  hostname_to[i++] = '\0';

  return i;
}

int write_response_answer(uint8_t* dns_buffer, int hostname_len, char* ip_addr)
{
  dns_resource_record_t* dns_rr = (dns_resource_record_t*)(dns_buffer + sizeof(dns_header_t) + hostname_len);

  dns_rr->type = htons(QTYPE_A);
  dns_rr->class = htons(QCLASS_IN);
  dns_rr->ttl = htonl(DNS_RR_TTL);
  dns_rr->rdlength = htons(4);
  dns_rr->rdata = htonl(inet_network(ip_addr));

  return sizeof(dns_resource_record_t);
}

void write_response_len(uint8_t* dns_buffer, int hostname_len)
{
  dns_header_t* dns_header = (dns_header_t*)dns_buffer;
  dns_header->len = htons(sizeof(dns_header_t) + hostname_len + sizeof(dns_resource_record_t) - 2);
}

int write_response(uint8_t* rr_dns_buffer, uint8_t* query_dns_buffer, char* ip_addr)
{
  int len_send = 0;

  len_send += write_response_header(rr_dns_buffer, query_dns_buffer);

  int hostname_len = write_response_hostname(rr_dns_buffer, query_dns_buffer);
  len_send += hostname_len;

  len_send += write_response_answer(rr_dns_buffer, hostname_len, ip_addr);
  write_response_len(rr_dns_buffer, hostname_len);

  return len_send;
}
