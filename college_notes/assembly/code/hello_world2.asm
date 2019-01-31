;  Source name     : EATSEG.ASM
;  Executable name : EATSEG.EXE
;  Code model:     : Real mode segmented model
;  Version         : 1.0
;  Created date    : 9/10/1999
;  Last update     : 9/10/1999
;  Author          : Jeff Duntemann
;  Description     : A simple example of a DOS .EXE file programmed for
;                    real mode segmented model, using NASM-IDE 1.1,
;                    NASM 0.98, and ALINK  This program demonstrates
;                    how segments are defined and initialized using NASM.


[BITS 16]                    ; Set 16 bit code generation

           SEGMENT code      ; Segment containing code

..start:	             ; The two dots tell the linker to Start Here.
                             ; Note that this is a special symbol and MUST
                             ;  be in lower case!  "..start:" <> "..START:"

; SEGMENT SETUP
;
;   In real mode segmented model, a program uses three segments, and it must
;   set up the addresses in the three corresponding segment registers.  This
;   is what the ASSUME directive does in MASM; we ASSUME nothing in NASM!
;   Each of the three segments has a name (here, code, data, and stack) and
;   these names are identifiers indicating segment addresses.  It is the
;   appropriate segment address that is moved into each segment register.
;   Note that you can't move an address directly into a segment register;
;   you must first move the address into a general purpose register.  Also
;   note that we don't do anything with CS; the ..start: label tells the
;   linker where the code segment begins.

    mov    ax,data           ; Move segment address of data segment into AX
    mov    ds,ax             ; Copy address from AX into DS
    mov    ax,stack          ; Move segment address of stack segment into AX
    mov    ss,ax             ; Copy address from AX into SS

    mov    sp,stacktop       ; Point SP to the top of the stack

    mov    dx,eatmsg         ; Mem data ref without [] loads the ADDRESS!
    mov    ah,9              ; Function 9 displays text to standard output.
    int    21H               ; INT 21H makes the call into DOS.

    mov    ax, 04C00H        ; This DOS function exits the program
    int    21H               ;   and returns control to DOS.

           SEGMENT data      ; Segment containing initialised data

eatmsg     db "Eat at Joe's!", 13, 10, "$"  ;Here's our message

           SEGMENT stack stack  ;This means a segment of *type* "stack"
                                ;  that is also *named* "stack"!  Some
                                ;  linkers demand that a stack segment
                                ;  have the explicit type "stack"

           resb 64           ; Reserve 64 bytes for the program stack
stacktop:                    ; It's significant that this label points to
                             ;   the *last* of the reserved 64 bytes, and
                             ;   not the first!


