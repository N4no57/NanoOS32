[BITS 32]
extern exception_handler
extern irq_handler

; ISR without error code
%macro isr_no_err_stub 1
global isr_stub_%1
isr_stub_%1:
    call exception_handler
    iret
%endmacro

; ISR with error code
%macro isr_err_stub 1
global isr_stub_%1
isr_stub_%1:
    call exception_handler
    add esp, 4
    iret
%endmacro

; IRQ no error code
%macro irq_no_err_stub 1
global irq_stub_%1
irq_stub_%1
    push %1+32
    call irq_handler
    add esp, 4
    iret
%endmacro


; Exceptions 0–31
isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

; IRQs 0–15 (vectors 32–47)
irq_no_err_stub 0
irq_no_err_stub 1
irq_no_err_stub 2
irq_no_err_stub 3
irq_no_err_stub 4
irq_no_err_stub 5
irq_no_err_stub 6
irq_no_err_stub 7
irq_no_err_stub 8
irq_no_err_stub 9
irq_no_err_stub 10
irq_no_err_stub 11
irq_no_err_stub 12
irq_no_err_stub 13
irq_no_err_stub 14
irq_no_err_stub 15

global isr_stub_table
global irq_stub_table
section .data

isr_stub_table:
%assign i 0 
%rep    32 
    dd isr_stub_%+i
%assign i i+1 
%endrep

irq_stub_table:
%assign i 0
%rep 16
    dd irq_stub_%+i
%assign i i+1
%endrep