/**
 * @file
 * @brief DNS response module API.
 *
 * Provides functions to read DNS queries and write DNS responses.
 */

#ifndef __DNS_RESPONSE_H__
#define __DNS_RESPONSE_H__

#include <stdint.h>

#define MAX_HOSTNAME_LEN 253

#define DNS_RR_TTL 999

/**
 * Copies DNS hostname from DNS message.
 * Hostname buffer is expected to be large enough.
 * @param hostname_to - buffer into which hostname is copied
 * @param dns_buffer  - DNS buffer
 */
void copy_hostname(uint8_t* hostname_to, uint8_t* dns_buffer);

/**
 * Strips basename from DNS hostname.
 * @param hostname - DNS hostname
 * @param basename - basename of DNS hostname
 * @return -1 if basename is not included in hostname, 0 otherwise
 */
int strip_basename(uint8_t* hostname, char* basename);

/**
 * Edits hostname so it does not contain any dots.
 * @param hostname - DNS hostname
 */
void dedot_hostname(uint8_t* hostname);

/**
 * Decodes data stored in DNS hostname using base32.
 * @param decoded    - buffer into which decoded data will be written
 * @param dns_buffer - DNS buffer
 * @param basename   - basename of DNS hostname
 * @return Number of bytes written, or -1 if basename error occured
 */
int decode_hostname(uint8_t* decoded, uint8_t* dns_buffer, char* basename);

/**
 * Writes DNS response header to the buffer.
 * DNS response buffer is expected to be large enough.
 * @param rr_dns_buffer    - DNS response buffer
 * @param query_dns_buffer - DNS query buffer
 * @return Number of bytes written
 */
int write_response_header(uint8_t* rr_dns_buffer, uint8_t* query_dns_buffer);

/**
 * Copies DNS hostname (qname) from query to response buffer.
 * DNS response buffer is expected to be large enough.
 * @param rr_dns_buffer    - DNS response buffer
 * @param query_dns_buffer - DNS query buffer
 * @return Number of bytes written
 */
int write_response_hostname(uint8_t* rr_dns_buffer, uint8_t* query_dns_buffer);

/**
 * Writes DNS resource record to the buffer.
 * DNS buffer is expected to be large enough.
 * @param dns_buffer   - DNS buffer
 * @param hostname_len - length of qname
 * @param ip_addr      - string containing IP address
 * @return Number of bytes written
 */
int write_response_answer(uint8_t* dns_buffer, int hostname_len, char* ip_addr);

/**
 * Writes length of the DNS response to the buffer.
 * DNS buffer is expected to be large enough.
 * @param dns_buffer   - DNS buffer
 * @param hostname_len - lenght of qname
 */
void write_response_len(uint8_t* dns_buffer, int hostname_len);

/**
 * Writes DNS response to the buffer.
 * IP address in response is used to indicate error.
 * DNS response buffer is expected to be large enough.
 * @param rr_dns_buffer    - DNS response buffer
 * @param query_dns_buffer - DNS query buffer
 * @param ip_addr          - string containing IP address
 * @return Number of bytes written
 */
int write_response(uint8_t* rr_dns_buffer, uint8_t* query_dns_buffer, char* ip_addr);

#endif   // __DNS_RESPONSE_H__
