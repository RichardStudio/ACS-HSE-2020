                            ; FASM version of HelloWorld program using printf() function
format PE console
entry main
use32

; ========== CODE SECTION ==========
section '.text' code readable executable

printf: jmp [imp_printf]
exit:   jmp [imp_exit]

main:
    push message
    call printf
    add esp, 4
    call [getch]
    push 0
    call exit

; ========== DATA SECTION ==========
section '.data' data readable writeable
    message db "Hello, World!", 0

; ========== IMPORT SECTION ==========
section '.idata' data import readable

; The header included below contains "library" and "import" macros that
; generate import data which must be placed in the import section of PE file
include "C:\asm\INCLUDE\MACRO\import32.inc"

library msvcrt, "MSVCRT.DLL"

import msvcrt,\
    imp_printf ,'printf',\
    getch, '_getch',\
    imp_exit   ,'exit'