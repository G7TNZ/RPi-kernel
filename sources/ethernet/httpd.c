/**
 * \file
 * HTTP server.
 * \author Adam Dunkels <adam@dunkels.com>
 *
 * Modified for the SBC-3 by Daryl Rictor - June 2010
 */

/*
 * Copyright (c) 2001, Adam Dunkels.
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
 * $Id: httpd.c,v 1.28.2.6 2003/10/07 13:22:27 adam Exp $
 *
 */

#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sbc3.h>

#include "uip.h"
#include "httpd.h"
#include "fs.h"
#include "cgi.h"

/* The HTTP server states: */
#define HTTP_NOGET        0
#define HTTP_FILE         1
#define HTTP_TEXT         2
#define HTTP_FUNC         3
#define HTTP_END          4

struct httpd_state *hs;

static char fbuff[UIP_TCP_MSS+1];

static void next_scriptline(void);
static void next_scriptstate(void);

/*-----------------------------------------------------------------------------------*/
/**
 * Initialize the web server.
 *
 * Starts to listen for incoming connection requests on TCP port 80.
 */
/*-----------------------------------------------------------------------------------*/
void
httpd_init(void)
{
  /* Listen to port 80. */
  uip_listen(HTONS(80));
}
/*-----------------------------------------------------------------------------------*/
void
httpd_appcall(void)
{
  int file;
  
    /* Pick out the application state from the uip_conn structure. */
    hs = (struct httpd_state *)(uip_conn->appstate);

    /* We use the uip_ test functions to deduce why we were
       called. If uip_connected() is non-zero, we were called
       because a remote host has connected to us. If
       uip_newdata() is non-zero, we were called because the
       remote host has sent us new data, and if uip_acked() is
       non-zero, the remote host has acknowledged the data we
       previously sent to it. */
    if(uip_connected()) {
      /* Since we have just been connected with the remote host, we
         reset the state for this connection. The ->count variable
         contains the amount of data that is yet to be sent to the
         remote host, and the ->state is set to HTTP_NOGET to signal
         that we haven't received any HTTP GET request for this
         connection yet. */
      hs->state = HTTP_NOGET;
      hs->count = 0;
      return;
    } else if(uip_aborted()) {
      uip_abort();
      if (hs->file) {
        close(hs->file);
        hs->file=0;
        hs->count=0;
      }
      if (hs->scfile) {
        close(hs->scfile);
        hs->scfile=0;
      }
      return;
    } else if(uip_poll()) {
      /* If we are polled ten times, we abort the connection. This is
         because we don't want connections lingering indefinately in
         the system. */
      if(hs->count++ >= 10) {
        uip_abort();
      }
      return;
    } else if(uip_newdata() && hs->state == HTTP_NOGET) {
      /* This is the first data we receive, and it should contain a
	 GET. */
      
      /* Check for GET. */
      if(uip_appdata[0] != ISO_G ||
	 uip_appdata[1] != ISO_E ||
	 uip_appdata[2] != ISO_T ||
	 uip_appdata[3] != ISO_space) {
	/* If it isn't a GET, we abort the connection. */
        uip_abort();
	  return;
      }
	       
      /* open file, if not found, open /404.htm - if no 404.htm, abort connection */
      file = fs_open((char *)&uip_appdata[4]);	
      if(!file) {
        file = fs_open("/404.htm");
        if(!file) {
          uip_abort();
          return;
        }
      } 

      if(uip_appdata[4] == ISO_slash &&
	 uip_appdata[5] == ISO_c &&
	 uip_appdata[6] == ISO_g &&
	 uip_appdata[7] == ISO_i &&
	 uip_appdata[8] == ISO_slash) {
	/* If the request is for a file that starts with "/cgi/", we
           prepare for invoking a script. */	
        hs->scfile = file;
        hs->scfp = 0;
        hs->file = 0;
        hs->count = 0;
        next_scriptstate();
      } else {
        hs->scfile = 0;
        /* The web server is now no longer in the HTTP_NOGET state, but
	   in the HTTP_FILE state since is has now got the GET from
	   the client and will start transmitting the file. */
        hs->state = HTTP_FILE;

        /* Point the file pointers in the connection state to point to
	   the first byte of the file. */
        hs->file = file;
        hs->fp = 0;
        hs->count = (int)fsize(file);	
      }     
    }

    if(hs->state != HTTP_FUNC) {
      /* Check if the client (remote end) has acknowledged any data that
	 we've previously sent. If so, we move the file pointer further
	 into the file and send back more data. If we are out of data to
	 send, we close the connection. */
      if(uip_acked()) {
        if(hs->count >= uip_conn->len) {
          hs->count -= uip_conn->len;
          hs->fp += uip_conn->len;
        } else {
          hs->count = 0;
        }
	
        if(hs->count == 0) {
	    if(hs->scfile != 0) {
            if (hs->state != HTTP_TEXT){
              close(hs->file);
            }
	      next_scriptline();
	      next_scriptstate();
	    } else {
 	      uip_close();
            if (hs->state != HTTP_TEXT){
              close(hs->file);
            }
            hs->file = 0;
	    }
	  }
      }         
    } else {
      /* Call the CGI function. */
      lseek (hs->scfile,(long)hs->scfp, SEEK_SET);  /* move pointer */
      read(hs->scfile, fbuff, UIP_TCP_MSS);
      if(cgitab[fbuff[2] - ISO_a](uip_acked())) {
	/* If the function returns non-zero, we jump to the next line
           in the script. */
        next_scriptline();
	  next_scriptstate();
      }
    }

    if(hs->state != HTTP_FUNC && !uip_poll()) {
      /* Send a piece of data, but not more than the MSS of the
	 connection. */

      if (hs->file && hs->count) {
        lseek (hs->file,(long)hs->fp, SEEK_SET);  /* move pointer */
        file = read(hs->file, &uip_appdata[0], UIP_TCP_MSS);
      }
      uip_send(uip_appdata, hs->count);

    }

    /* Finally, return to uIP. Our outgoing packet will soon be on its
       way... */
    return;
    
}

/*-----------------------------------------------------------------------------------*/
/* next_scriptline():
 *
 * Reads the script until it finds a newline. */
static void
next_scriptline(void)
{
  int i;

  lseek (hs->scfile,(long)hs->scfp, SEEK_SET);  /* move pointer */
  read(hs->scfile, fbuff, UIP_TCP_MSS);
  i=1;
  for (;i <=UIP_TCP_MSS && fbuff[i] != ISO_nl; i++); 
  hs->scfp += i+1;

}
/*-----------------------------------------------------------------------------------*/
/* next_sciptstate:
 *
 * Reads one line of script and decides what to do next.
 */
static void
next_scriptstate(void)
{
  u8_t i;
  int file;

 again:
  lseek (hs->scfile,(long)hs->scfp, SEEK_SET);  /* move pointer */
  read(hs->scfile, fbuff, UIP_TCP_MSS);

  switch(fbuff[0]) {
  case ISO_t:
    /* Send a text string. */
    hs->state = HTTP_TEXT;
    hs->file = hs->scfile;
    hs->fp = hs->scfp+2;

    /* Calculate length of string. */
//    lseek (hs->file,(long)hs->fp, SEEK_SET);  /* move pointer */
//    read(hs->file, fbuff, UIP_TCP_MSS);
    for(i = 2; fbuff[i] != ISO_nl && i < UIP_TCP_MSS; ++i );
    hs->count = (int)i-2;    
    break;
  case ISO_c:
    /* Call a function. */
    hs->state = HTTP_FUNC;
    hs->file = 0;
    hs->count = 0;
    cgitab[fbuff[2] - ISO_a](0);
    break;
  case ISO_i:   
    /* Include a file. */
    file = fs_open((char *)&fbuff[2]);
    if(!file) {
      uip_abort();
    } else {
      hs->state = HTTP_FILE;
      hs->file = file;
      hs->fp =0; 
      hs->count = fsize(file);
    }
    break;
  case ISO_hash:
    /* Comment line. */
    next_scriptline();
    goto again;
    break;
  case ISO_period:
    /* End of script. */
    hs->state = HTTP_END;
    uip_close();
    close(hs->scfile);
    hs->scfile = 0;
    break;
  default:
    uip_abort();
    break;
  }
}
/*-----------------------------------------------------------------------------------*/
