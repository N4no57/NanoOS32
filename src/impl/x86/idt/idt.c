#include <idt.h>
#include <pic.h>
#include <pit.h>
#include <stdbool.h>
#include <stdio.h>

static bool vectors[IDT_MAX_DESCRIPTORS];
extern void* isr_stub_table[];

__attribute__((noreturn))
void exception_handler(unsigned char isr) {
    printf("Exception interrupt\n");
    printf("interrupt code: %d\n", isr);
    __asm__ volatile("cli");
    __asm__ volatile("hlt");
}

__attribute__((aligned(0x10)));
static idt_entry_t idt[256];
static idtr_t idtr;

void interrupt_handler(unsigned char isr) {
    if (isr < 32) {
        exception_handler(isr);
    }

    unsigned char irq = isr - 32;
    switch(irq) {
        case 0:
            pit_interrupt_handler();
            PIC_sendEOI(irq);
            break;
        default:
            printf("unhandled irq\n");
            printf("IRQ: %d\n", irq);
            PIC_sendEOI(irq);
            break;
    }
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08;
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}

void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)(sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1);

    for (uint8_t vector = 0; vector < 48; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}