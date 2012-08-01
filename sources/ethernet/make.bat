set CC65LIB=c:\cc65\lib
cl65 -t none -C sbc3.cfg -o enet -m enet.map -l %CC65LIB%\sbc3.o main.c uip.c uip_arch.c appcall.c memb.c telnetd.c telnetd-shell.c smtp.c httpd.c fs.c cgi.c uip_arp.c enceth.s %CC65LIB%\sbc3.lib
del *.o /Q >nul
pause

