format PE console
entry start

include 'C:\asm\INCLUDE\win32a.inc'

section '.data' data readable writable

        strVecSize   db 'Array size: ', 0
        strIncorSize db 'Wrong size = %d', 10, 0
        strVecElemI  db 'Input [%d] element ', 0
        strScanInt   db '%d', 0
        strVecElemOut  db '[%d] = %d', 10, 0
        strOutA        db 'Array A:', 10, 0
        strOutB        db 'Array B:', 10, 0
        strEmptyVec    db 'Empty Array', 10, 0

        vec_size     dd 0
        vec2_size    dd 0
        test1        dd 1
        i            dd ?
        j            dd ?
        tmp          dd ?
        tmpStack     dd ?
        vec          rd 100
        vec2         rd 100

;--------------------------------------------------------------------------
section '.code' code readable executable
start:
        call VectorInput
        push strOutA
        call [printf]
        call VectorOut
        call CreateB
        push strOutB
        call [printf]
        call VectorBOut
finish:
        call [getch]

        push 0
        call [ExitProcess]

;--------------------------------------------------------------------------
VectorInput:
        push strVecSize
        call [printf]
        add esp, 4

        push vec_size
        push strScanInt
        call [scanf]
        add esp, 8

        mov eax, [vec_size]
        cmp eax, 0
        jg  getVector
; fail size
        push [vec_size]
        push strIncorSize
        call [printf]
        call[getch]
        push 0
        call [ExitProcess]
; else continue...
getVector:
        xor ecx, ecx            ; ecx = 0
        mov ebx, vec            ; ebx = &vec
getVecLoop:
        mov [tmp], ebx
        cmp ecx, [vec_size]
        jge endInputVector       ; to end of loop

        ; input element
        mov [i], ecx
        push ecx
        push strVecElemI
        call [printf]
        add esp, 8

        push ebx
        push strScanInt
        call [scanf]
        add esp, 8

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp getVecLoop
endInputVector:
        ret
;--------------------------------------------------------------------------
VectorOut:
        mov [tmpStack], esp
        xor ecx, ecx            ; ecx = 0
        mov ebx, vec            ; ebx = &vec
putVecLoop:
        mov [tmp], ebx
        cmp ecx, [vec_size]
        je endOutputVector      ; to end of loop
        mov [i], ecx

        ; output element
        push dword [ebx]
        push ecx
        push strVecElemOut
        call [printf]

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp putVecLoop
endOutputVector:
        mov esp, [tmpStack]
        ret
;--------------------------------------------------------------------------
CreateB:
        xor ecx, ecx
        mov ebx, vec2   ;ebx = &vec2
        mov eax, vec    ;eax = &vec
        mov [j], 0      ;size of array B
intoSecVecLoop:
        cmp ecx, [vec_size]  ; if index == array A size end loop
        je endCreateB

        mov [i], ecx
        mov edx, [eax]
        cmp edx, 0
        jle  skipElem     ; if el <= 0 skip this elem

        mov ecx, [i]
        mov [ebx], ecx
        inc ecx            ; increase index in Arr A
        mov [i], ecx
        inc [j]            ; memorizing the size of second array
        add ebx, 4         ; to next el
        add eax, 4         ; to next el
        jmp intoSecVecLoop
skipElem:
        inc ecx
        mov [i], ecx
        add eax, 4         ; to next el
        jmp intoSecVecLoop
endCreateB:
        mov ecx, [j]
        mov [vec2_size], ecx  ;final size of array B
        ret
;--------------------------------------------------------------------------
VectorBOut:
        mov [tmpStack], esp
        xor ecx, ecx            ; ecx = 0
        mov ebx, vec2            ; ebx = &vec
        cmp [vec2_size], 0      ; if the array is empty output message
        je emptyVec
putVecBLoop:
        mov [tmp], ebx
        cmp ecx, [vec2_size]
        je endOutputVectorB      ; to end of loop
        mov [i], ecx

        ; output element
        push dword [ebx]
        push ecx
        push strVecElemOut
        call [printf]

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp putVecBLoop
emptyVec:
        push strEmptyVec
        call [printf]
        jmp endOutputVectorB
endOutputVectorB:
        mov esp, [tmpStack]
        ret
;--------------------------------------------------------------------------
section '.idata' import data readable
    library kernel, 'kernel32.dll',\
            msvcrt, 'msvcrt.dll',\
            user32,'USER32.DLL'

include 'C:\asm\INCLUDE\API\user32.inc'
include 'C:\asm\INCLUDE\API\kernel32.inc'
    import kernel,\
           ExitProcess, 'ExitProcess',\
           HeapCreate,'HeapCreate',\
           HeapAlloc,'HeapAlloc'
  include 'C:\asm\INCLUDE\API\kernel32.inc'
    import msvcrt,\
           printf, 'printf',\
           scanf, 'scanf',\
           getch, '_getch'