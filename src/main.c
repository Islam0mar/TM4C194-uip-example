/**
 *   \file main.c
 *   \brief A Documented file.
 *
 *  Copyright (c) 2020 Islam Omar (io1131@fayoum.edu.eg)
 *
 */

#include <stdbool.h>
#include <stdint.h>

#include "config.h"
/* config has be first */

#include "bsp.h"
#include "inc/hw_types.h"
#include "logger.h"
#include "states.h"
#include "third_party/uip/uip/uip.h"
#include "third_party/uip/uip/uip_arp.h"

uint32_t g_ui32IPAddress;
/* when dhcp fail or not enabled */
#define DEFAULT_IPADDR0 192
#define DEFAULT_IPADDR1 168
#define DEFAULT_IPADDR2 1
#define DEFAULT_IPADDR3 13 /* not taken normally */
#define DEFAULT_NETMASK0 255
#define DEFAULT_NETMASK1 255
#define DEFAULT_NETMASK2 255
#define DEFAULT_NETMASK3 0
#define DHCPC_CLIENT_PORT 68

/* UIP Timers (in MS) */
#define UIP_PERIODIC_TIMER_MS 500
#define UIP_ARP_TIMER_MS 10000

/* Macro for accessing the Ethernet header information in the buffer. */
u8_t uip_buf[UIP_BUFSIZE + 2];

#define BUF ((struct uip_eth_hdr *)uip_buf)

int main(void) {
  uip_ipaddr_t sIPAddr;
  uint32_t ui32Temp;
  static struct uip_eth_addr sTempAddr;
  uint32_t ui32PeriodicTimer = 0, ui32ARPTimer = 0;
  uint8_t *mac_addr = BspGetMacAddress();

  /* TODO: HW failure */
  BspInit();

  // Initialize the uIP TCP/IP stack.
  uip_init();

  /* get mac address */
  sTempAddr.addr[0] = mac_addr[0];
  sTempAddr.addr[1] = mac_addr[1];
  sTempAddr.addr[2] = mac_addr[2];
  sTempAddr.addr[3] = mac_addr[3];
  sTempAddr.addr[4] = mac_addr[4];
  sTempAddr.addr[5] = mac_addr[5];

  // Set the local MAC address (for uIP).
  uip_setethaddr(sTempAddr);
  uip_ipaddr(sIPAddr, DEFAULT_IPADDR0, DEFAULT_IPADDR1, DEFAULT_IPADDR2,
             DEFAULT_IPADDR3);
  uip_sethostaddr(sIPAddr);
  LOG_INFO("Setting ip address done.");
  uip_ipaddr(sIPAddr, DEFAULT_NETMASK0, DEFAULT_NETMASK1, DEFAULT_NETMASK2,
             DEFAULT_NETMASK3);
  uip_setnetmask(sIPAddr);

  /* Main Application Loop. */
  AppState_t *s = (AppState_t *)&(uip_conn->appstate);
  s->state = kWaitingForConnection;
  uip_listen(HTONS(80));
  ui32ARPTimer = ui32PeriodicTimer = BspGetMSecTime();
  while (true) {
    // Check for an RX Packet and read it.
    if (HWREGBITW(&g_ui32Flags, kEmacReceivedFrame)) {
      //
      // Clear the RX Packet event flag.
      //
      HWREGBITW(&g_ui32Flags, kEmacReceivedFrame) = 0;

      // Get the packet and set uip_len for uIP stack usage.
      //
      uip_len = (unsigned short)BspPacketReceive(uip_buf, sizeof(uip_buf));

      //
      // Process incoming IP packets here.
      //
      if (BUF->type == htons(UIP_ETHTYPE_IP)) {
        uip_arp_ipin();
        uip_input();

        //
        // If the above function invocation resulted in data that
        // should be sent out on the network, the global variable
        // uip_len is set to a value > 0.
        //
        if (uip_len > 0) {
          uip_arp_out();
          BspPacketTransmit(uip_buf, uip_len);
          uip_len = 0;
        }
      }

      //
      // Process incoming ARP packets here.
      //
      else if (BUF->type == htons(UIP_ETHTYPE_ARP)) {
        uip_arp_arpin();

        // If the above function invocation resulted in data that
        // should be sent out on the network, the global variable
        // uip_len is set to a value > 0.
        if (uip_len > 0) {
          BspPacketTransmit(uip_buf, uip_len);
          uip_len = 0;
        }
      }
    }

    // Process TCP/IP Periodic Timer here.
    if (BspGetMSecTime() - ui32PeriodicTimer > UIP_PERIODIC_TIMER_MS) {
      ui32PeriodicTimer = BspGetMSecTime();
      for (ui32Temp = 0; ui32Temp < UIP_CONNS; ui32Temp++) {
        uip_periodic(ui32Temp);

        //
        // If the above function invocation resulted in data that
        // should be sent out on the network, the global variable
        // uip_len is set to a value > 0.
        //
        if (uip_len > 0) {
          uip_arp_out();
          BspPacketTransmit(uip_buf, uip_len);
          uip_len = 0;
        }
      }

      for (ui32Temp = 0; ui32Temp < UIP_UDP_CONNS; ui32Temp++) {
        uip_udp_periodic(ui32Temp);

        //
        // If the above function invocation resulted in data that
        // should be sent out on the network, the global variable
        // uip_len is set to a value > 0.
        //
        if (uip_len > 0) {
          uip_arp_out();
          BspPacketTransmit(uip_buf, uip_len);
          uip_len = 0;
        }
      }
    }

    // Process ARP Timer here.
    if (ui32ARPTimer - BspGetMSecTime() > UIP_ARP_TIMER_MS) {
      ui32ARPTimer = BspGetMSecTime();
      uip_arp_timer();
    }
  }
}

void AppCall(void) {
  static uint8_t local_buff[sizeof(uip_buf) / sizeof(uip_buf[0])];
  static uint16_t local_buff_len;
  static uint8_t poll_count;
  static AppState_t *s;
  s = (AppState_t *)&(uip_conn->appstate);
  if (uip_newdata() && s->state == kIdle) {
    local_buff_len = uip_len;
    MemCpy(local_buff, uip_buf, uip_len);
    uip_send(local_buff, local_buff_len);
    HWREGBITW(&g_ui32Flags, kAck) = 0;
    s->state = kWaitForAck;
    /* s->dataptr += uip_conn->len;
    s->dataleft -= uip_conn->len;
    uip_send(s->dataptr, s->dataleft); */
  } else if (uip_acked() && s->state == kWaitForAck) {
    HWREGBITW(&g_ui32Flags, kAck) = 1;
    s->state = kIdle;
  } else if (uip_rexmit() && s->state == kWaitForAck) {
    uip_send(local_buff, local_buff_len);
    s->state = kWaitForAck;
  } else if (uip_connected() && s->state == kWaitingForConnection) {
    s->state = kIdle;
    uip_send("Hello, World", 12);
    poll_count = 0;
  } else if (uip_poll() && s->state == kIdle) {
    // If we are polled more than ten times, we abort the connection
    // . This is
    // because we don't want connections lingering indefinately in
    // the system.
    if (poll_count++ > 10) {
      uip_abort();
    }
    return;
  } else if (uip_timedout() && s->state != kNone) {
    HWREGBITW(&g_ui32Flags, kTimeOut) = 1;
    s->state = kWaitingForConnection;
  } else if (uip_aborted() && s->state != kNone) {
    HWREGBITW(&g_ui32Flags, kAbort) = 1;
    s->state = kWaitingForConnection;
  } else if (uip_closed() && s->state != kNone) {
    s->state = kNone;
  }
}

#ifdef UIP_ARCH_IPCHKSUM
//*****************************************************************************
//
// Return the IP checksum for the packet in uip_buf.  This is a dummy since
// the hardware calculates this for us.
//
//*****************************************************************************
u16_t uip_ipchksum(void) {
  //
  // Dummy function - the hardware calculates and inserts all required
  // checksums for us.
  //
  return (0xffff);
}

//*****************************************************************************
//
// This is a dummy since the hardware calculates this for us.
//
//*****************************************************************************
u16_t uip_chksum(u16_t *data, u16_t len) { return (0xffff); }

//*****************************************************************************
//
// This is a dummy since the hardware calculates this for us.
//
//*****************************************************************************
u16_t uip_icmp6chksum(void) { return (0xffff); }

//*****************************************************************************
//
// This is a dummy since the hardware calculates this for us.
//
//*****************************************************************************
u16_t uip_tcpchksum(void) { return (0xffff); }
#endif
