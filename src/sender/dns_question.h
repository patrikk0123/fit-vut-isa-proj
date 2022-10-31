/**
 * @file
 * @brief DNS question module API.
 *
 * Provides functions to write DNS queries.
 */

#ifndef __DNS_QUESTION_H__
#define __DNS_QUESTION_H__

#include <stdint.h>

/**
 * Writes DNS query header to the buffer.
 * DNS buffer is expected to be large enough.
 * @param dns_buffer - DNS buffer
 * @param id         - DNS query ID
 * @return Number of bytes written
 */
int write_query_header(uint8_t* dns_buffer, uint16_t id);

/**
 * Writes DNS hostname to the buffer (qname).
 * DNS buffer is expected to be large enough.
 * @param dns_buffer   - DNS buffer
 * @param encoded_data - buffer of encoded data sent through DNS tunneling
 * @param data_len     - lenght of encoded data
 * @param basename     - basename of DNS hostname
 * @return Number of bytes written
 */
int write_query_hostname(uint8_t* dns_buffer, uint8_t* encoded_data, int data_len, uint8_t* basename);

/**
 * Writes encoded data to the buffer (part of qname).
 * DNS buffer is expected to be large enough.
 * @param dns_buffer   - DNS buffer
 * @param encoded_data - buffer of encoded data sent through DNS tunneling
 * @param data_len     - lenght of encoded data
 * @return Number of bytes written
 */
int write_query_hostname_data(uint8_t* dns_buffer, uint8_t* encoded_data, int data_len);

/**
 * Writes DNS hostname basename to the buffer (part of qname).
 * DNS buffer is expected to be large enough.
 * @param dns_buffer   - DNS buffer
 * @param hostname_len - length of qname without basename
 * @param basename     - basename of DNS hostname
 * @return Number of bytes written
 */
int write_query_hostname_basename(uint8_t* dns_buffer, int hostname_len, uint8_t* basename);

/**
 * Gets pointer to the QNAME part of the DNS query buffer.
 * @param dns_buffer - DNS buffer
 */
uint8_t* get_query_hostname(uint8_t* dns_buffer);

/**
 * Writes DNS question to the buffer (just qtype and qclass).
 * DNS buffer is expected to be large enough.
 * @param dns_buffer   - DNS buffer
 * @param hostname_len - length of qname
 * @return Number of bytes written
 */
int write_query_question(uint8_t* dns_buffer, int hostname_len);

/**
 * Writes length of the DNS query to the buffer.
 * DNS buffer is expected to be large enough.
 * @param dns_buffer   - DNS buffer
 * @param hostname_len - lenght of qname
 */
void write_query_len(uint8_t* dns_buffer, int hostname_len);

/**
 * Writes DNS query to the buffer.
 * DNS buffer is expected to be large enough.
 * @param dns_buffer   - buffer to write DNS query to
 * @param encoded_data - buffer of encoded data sent through DNS tunneling
 * @param data_len     - lenght of encoded data
 * @param basenaeme    - basename of DNS hostname
 * @param id           - DNS query ID
 * @return Number of bytes written
 */
int write_query(uint8_t* dns_buffer, uint8_t* encoded_data, int data_len, uint8_t* basename, uint16_t id);

/**
 * Reads resolved IP from DNS response.
 * IP buffer is expected to be large enough.
 * @param ip_addr    - buffer to write IP address to
 * @param dns_buffer - DNS response buffer
 */
void read_response_ip(uint8_t* ip_addr, uint8_t* dns_buffer);

#endif   // __DNS_QUESTION_H__
