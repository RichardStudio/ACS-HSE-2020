format PE Console
entry start
include 'C:\asm\INCLUDE\win32ax.inc'
section '.text' code readable executable
  start:
    mov eax, 'A'
    mov ecx, 26
    print_char:
    push ecx
    cinvoke putchar,eax
    pop ecx
    inc eax
    loop print_char
    cinvoke putchar,10 ; new line display
    cinvoke system, 'pause'
    invoke ExitProcess, 0
section '.idata' import data readable
  library\
    kernel32, 'kernel32.dll', \
    msvcrt,     'msvcrt.dll' ; C-Run time from MS. This is always on every windows machine
  import kernel32, \
    ExitProcess, 'ExitProcess'
  import msvcrt, \
    putchar, 'putchar', \
    system, 'system'