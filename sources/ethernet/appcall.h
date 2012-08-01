//
// this is used to direct any uip_appcall to the proper place
//

void appcall(void);

/* UIP_APPCALL: the name of the application function. This function
   must return void and take no arguments (i.e., C type "void
   appfunc(void)"). */

#ifndef UIP_APPCALL
#define UIP_APPCALL   appcall
#endif

/* ensure this number is large enough to hold the largest app installed.
 The sizes can be calculated using the app's structure located in the header file. */

#ifndef UIP_APPSTATE_SIZE
#define UIP_APPSTATE_SIZE (20)
#endif