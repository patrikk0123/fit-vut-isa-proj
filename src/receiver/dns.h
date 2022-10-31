/**
 * @file
 * @brief DNS module API.
 *
 * Provides generic DNS interfaces and functions.
 */

#ifndef __DNS_H__
#define __DNS_H__

#include <stdint.h>

#define DNS_PORT 53

#define QTYPE_A   1
#define QCLASS_IN 1

/**
 * @struct dns_header_t
 * @brief DNS header - TCP variant with length included
 */
typedef struct {
  uint16_t len;
  uint16_t id;

  uint16_t rd     : 1;
  uint16_t tc     : 1;
  uint16_t aa     : 1;
  uint16_t opcode : 4;
  uint16_t qr     : 1;
  uint16_t rcode  : 4;
  uint16_t zero   : 3;
  uint16_t ra     : 1;

  uint16_t qdcount;
  uint16_t ancount;
  uint16_t nscount;
  uint16_t arcount;
} __attribute__((packed)) dns_header_t;

/**
 * @struct dns_udp_header_t
 * @brief DNS header - UDP variant without length
 */
typedef struct {
  uint16_t id;

  uint16_t rd     : 1;
  uint16_t tc     : 1;
  uint16_t aa     : 1;
  uint16_t opcode : 4;
  uint16_t qr     : 1;
  uint16_t rcode  : 4;
  uint16_t zero   : 3;
  uint16_t ra     : 1;

  uint16_t qdcount;
  uint16_t ancount;
  uint16_t nscount;
  uint16_t arcount;
} __attribute__((packed)) dns_udp_header_t;

/**
 * @struct dns_question_t
 * @brief DNS question
 */
typedef struct {
  uint16_t qtype;
  uint16_t qclass;
} __attribute__((packed)) dns_question_t;

/**
 * @struct dns_resource_record_t
 * @brief DNS resource record
 */
typedef struct {
  uint16_t type;
  uint16_t class;
  uint32_t ttl;
  uint16_t rdlength;
  uint32_t rdata;
} __attribute__((packed)) dns_resource_record_t;

/**
 * Copies IP of default DNS server into buffer (if any).
 * Buffer is expected to be large enough to contain IP address.
 * @param buffer - buffer where the IP address will be copied
 */
void get_default_dns_ip(uint8_t* buffer);

#endif   // __DNS_H__
