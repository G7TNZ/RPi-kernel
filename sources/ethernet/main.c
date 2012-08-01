/*
 * Copyright (c) 2001-2003, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * Modified for the SBC-3 by Daryl Rictor - June 2010
 *
 */

#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sbc3.h>
#include "uip.h"
#include "uip_arp.h"
#include "httpd.h"
#include "smtp.h"
#include "telnetd.h"
#include "enceth.h"

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

static int email;


/*-----------------------------------------------------------------------------------*/
int main(void) {

  int c;
  u16_t ipaddr[2];
  u8_t i;
  clock_t  arptimer, ptimer;
  clock_t  arpto = 600;
  clock_t  period = 30;
  
  /* Initialize the device driver. */
  init_ether();

  /* Initialize the uIP TCP/IP stack. */
  uip_init();

  /* Initialize the ARP driver. */
  uip_arp_init();

  /* Initialize the MAC Address */
  uip_ethaddr.addr[0] = 0;
  uip_ethaddr.addr[1] = 6;
  uip_ethaddr.addr[2] = 5;
  uip_ethaddr.addr[3] = 8;
  uip_ethaddr.addr[4] = 1;
  uip_ethaddr.addr[5] = 6;
  uip_setethaddr(uip_ethaddr);

  /* Initialize the IP address. */
  uip_ipaddr(ipaddr, 192,168,0,65); /* your ip */
  uip_sethostaddr(ipaddr);
  uip_ipaddr(ipaddr, 192,168,0,254); /* gateway ip */
  uip_setdraddr(ipaddr);
  uip_ipaddr(ipaddr, 255,255,255,0); /* subnet mask */
  uip_setnetmask(ipaddr);

  /* Initialize the SMTP app */
  uip_ipaddr(ipaddr, 0,0,0,0);  /* Enter your email server ip here */
  smtp_configure("sbc3", ipaddr); 
  email = 0;

  /* Initialize the HTTP server. */
  httpd_init();

  /* Initialize the Telnet server. */
  telnetd_init();

  /* Initialize the timers. */
  arptimer = clock();
  ptimer = clock();

  while(1) {

    if (kbhit()) {
       c = cgetc();
       if (c == 'q' || c == 'Q')
	 break;

       switch (c) {
         case 'm':
           cprintf("Send Email\r\n");
           smtp_send("recipient email address","your email address","title","message body",16);  /* replace 16 with the length of your message body */
           break;

         default:
           break;
       } 
    }

    uip_len = rcv_ether();

    if(uip_len > 0) {

      if(BUF->type == htons(UIP_ETHTYPE_IP)) {
 	    uip_arp_ipin();
	    uip_input();
	    /* If the above function invocation resulted in data that
	       should be sent out on the network, the global variable
	       uip_len is set to a value > 0. */
	    if(uip_len > 0) {
	      uip_arp_out();
	      send_ether();
	    }
	  } else if(BUF->type == htons(UIP_ETHTYPE_ARP)) {

	    uip_arp_arpin();
	    /* If the above function invocation resulted in data that
	       should be sent out on the network, the global variable
	       uip_len is set to a value > 0. */
	    if(uip_len > 0) {
	      send_ether();
	    }
      } else {
        /* don't know what to do with data, so discard it */
        //uip_log("unknown packet type");
        uip_len = 0;
      }
    }

    /* check ptimer for next cycle */
    if ((clock() - ptimer) > period) {
      ptimer += period;
      for(i = 0; i < UIP_CONNS; i++) {
  	  uip_periodic(i);
	  /* If the above function invocation resulted in data that
	     should be sent out on the network, the global variable
	     uip_len is set to a value > 0. */
	  if(uip_len > 0) {
	    uip_arp_out();
	    send_ether();
	  }
      }
#if UIP_UDP
        for(i = 0; i < UIP_UDP_CONNS; i++) {
  	    uip_udp_periodic(i);
	    /* If the above function invocation resulted in data that
	       should be sent out on the network, the global variable
	       uip_len is set to a value > 0. */
	    if(uip_len > 0) {
	      uip_arp_out();
	      send_ether();
	    }
        }
#endif /* UIP_UDP */
    }

    /* Call the ARP timer function every 10 seconds. */
    if(clock() - arptimer > arpto) {
  	arptimer += arpto;
      uip_arp_timer();
    }
  }
  return 0;
}
