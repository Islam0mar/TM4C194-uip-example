/*
 * dhcp.h
 *
 *  Created on: Jan 2, 2021
 *      Author: islam
 */

#ifndef SRC_DHCP_H_
#define SRC_DHCP_H_

#include <stdint.h>

#include "third_party/uip/uip/uip.h"

/*
 * http://www.tcpipguide.com/free/t_DHCPMessageFormat.htm
 * https://github.com/samueldotj/dhcp-client/blob/master/dhcp-client.c
 */
#define DHCP_CHADDR_LEN 16
#define DHCP_SNAME_LEN 64
#define DHCP_FILE_LEN 128
#define DHCP_MAGIC_COOKIE 0x63825363
#define DHCP_BOOTREQUEST 1
#define DHCP_BOOTREPLY 2

#define DHCP_HARDWARE_TYPE_10_EHTHERNET 1

#define MESSAGE_TYPE_PAD 0
#define MESSAGE_TYPE_REQ_SUBNET_MASK 1
#define MESSAGE_TYPE_ROUTER 3
#define MESSAGE_TYPE_DNS 6
#define MESSAGE_TYPE_DOMAIN_NAME 15
#define MESSAGE_TYPE_REQ_IP 50
#define MESSAGE_TYPE_DHCP 53
#define MESSAGE_TYPE_PARAMETER_REQ_LIST 55
#define MESSAGE_TYPE_END 255

#define DHCP_OPTION_DISCOVER 1
#define DHCP_OPTION_OFFER 2
#define DHCP_OPTION_REQUEST 3
#define DHCP_OPTION_PACK 4

typedef struct dhcp {
  uint8_t opcode;
  uint8_t htype;
  uint8_t hlen;
  uint8_t hops;
  uint32_t xid;
  uint16_t secs;
  uint16_t flags;
  uint8_t ciaddr[4];
  uint8_t yiaddr[4];
  uint8_t siaddr[4];
  uint8_t giaddr[4];
  uint8_t chaddr[DHCP_CHADDR_LEN];
  char bp_sname[DHCP_SNAME_LEN];
  char bp_file[DHCP_FILE_LEN];
  uint32_t magic_cookie;
  uint8_t bp_options[0];
} dhcp_t;

/* TODO: complete DHCP DORA */
/* static void create_msg(register dhcp_t *m) { */
/*   m->op = DHCP_BOOTREQUEST; */
/*   m->htype = DHCP_HARDWARE_TYPE_10_EHTHERNET; */
/*   m->hlen = 6; */
/*   m->hops = 0; */
/*   memcpy(m->xid, xid, sizeof(m->xid)); */
/*   m->secs = 0; */
/*   m->flags = HTONS(BOOTP_BROADCAST); /\*  Broadcast bit. *\/ */
/*   memcpy(m->ciaddr, uip_hostaddr, sizeof(m->ciaddr)); */
/*   memset(m->yiaddr, 0, sizeof(m->yiaddr)); */
/*   memset(m->siaddr, 0, sizeof(m->siaddr)); */
/*   memset(m->giaddr, 0, sizeof(m->giaddr)); */
/*   memcpy(m->chaddr, s.mac_addr, s.mac_len); */
/*   memset(&m->chaddr[s.mac_len], 0, sizeof(m->chaddr) - s.mac_len); */
/*   memset(m->sname, 0, sizeof(m->sname)); */
/*   memset(m->file, 0, sizeof(m->file)); */
/*   memcpy(m->options, magic_cookie, sizeof(magic_cookie)); */
/* } */
/* TODO: afte DHCP implementation  */
/* LOG_INFO("Waiting for IP address..."); */
/* uip_ipaddr(sIPAddr, 0, 0, 0, 0); */
/* uip_sethostaddr(sIPAddr); */
/* uip_ipaddr(sIPAddr, 0, 0, 0, 0); */
/* uip_setnetmask(sIPAddr); */
/* /\* send dhcp request message *\/ */
/* uip_ipaddr(sIPAddr, 255, 255, 255, 255); */
/* struct uip_udp_conn *conn = uip_udp_new(&sIPAddr,
 * HTONS(DHCPC_SERVER_PORT)); */
/* if (conn != NULL) { */
/*   uip_udp_bind(conn, HTONS(DHCPC_CLIENT_PORT)); */
/* } */
/* u8_t *end; */
/* struct dhcp_msg *m = (struct dhcp_msg *)uip_appdata; */
/* create_msg(m); */
/* end = add_msg_type(&m->options[4], DHCPREQUEST); */
/* end = add_server_id(end); */
/* end = add_req_ipaddr(end); */
/* end = add_end(end); */
#endif /* SRC_DHCP_H_ */
