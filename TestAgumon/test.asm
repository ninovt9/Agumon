.386
.model flat, stdcall
option casemap : none


include windows.inc
include kernel32.inc
includelib kernel32.lib
include	msvcrt.inc
includelib msvcrt.lib


.data
i dd 2


.code
start:
mov eax, 2
mov i, eax
end start
