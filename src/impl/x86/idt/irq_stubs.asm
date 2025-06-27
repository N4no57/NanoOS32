[bits 32]
extern irq_handler

; IRQ no error code
%macro irq_no_err_stub 1
global irq_stub_%1
irq_stub_%1
    push %1+32
    call irq_handler
    add esp, 4
    iret
%endmacro

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
    
global irq_stub_table
section .data
irq_stub_table:
%assign i 0
%rep 16
    dd irq_stub_%+i
%assign i i+1
%endrep