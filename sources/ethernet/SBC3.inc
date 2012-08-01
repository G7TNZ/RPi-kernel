;
; SBC-3 include File by Daryl Rictor (c) 2010
;
; Top of available memory
TOPMEM := $00F7FF

.P816					; use 65816 assembly


; Zero-page locations

Ptr		:=	$E0		; 3 byte memory pointer
temp		:= 	$E3		; 1 byte temp holder
Addr		:=	$E4		; 3 byte address counter


; BIOS jump table

_savef          := $07fd61
_loadf          := $07fd59
_restdir        := $07fd51
_idestat        := $07fd49
_rxide          := $07fd41
_sendide        := $07fd39
_sendidec       := $07fd31
_gticks         := $07fd22
_getcalr        := $07fd07
_prdt           := $07fcff
_getdt          := $07fce7
_setser         := $07fcdf
_scanser        := $07fcd7
_serout         := $07fccf
_serin          := $07fcc7
_scankb         := $07fcbf
_inpt           := $07fcb7
_scan           := $07fcaf
_cin            := $07fca7
_scursor        := $07fc80
_hwout          := $07fc78
_hbout          := $07fc70
_crout          := $07fc68
_getc           := $07fc59
_setc           := $07fc48
_gety           := $07fc3c
_sety           := $07fc2e
_getx           := $07fc22
_setx           := $07fc14
_cls            := $07fc08
_cout           := $07fc00
_expjmp         := $07d196
_expcmd         := $07d15c
_osreturn       := $07d0af
;
; end
;
