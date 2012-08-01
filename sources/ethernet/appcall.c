//
//  This routine received call from uIP via uip_appcall macro.
//  it will steer to the proper handler based on the port #
//

#include "appcall.h"
#include "uip.h"
#include "httpd.h"
#include "smtp.h"
#include "telnetd.h"

/*-----------------------------------------------------------------------------------*/
void
appcall(void)
{
  if (uip_conn->lport == HTONS(80)) {
    httpd_appcall();
  } else if (uip_conn->rport == HTONS(25)) {
    smtp_appcall();
  } else if (uip_conn->lport == HTONS(23)) {
    telnetd_app();
  } else {
    uip_abort();
  }
}
