#include <stdint.h>
#include <stddef.h>
#include <terminal.h>
#include <string.h>
#include <stdio.h>

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)  {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;
uint16_t* scroll_back_buffer[SCROLLBACK_MAX_LINES*VGA_WIDTH];

void terminal_clear() {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}

    terminal_row = 0;
    terminal_column = 0;
}

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	
	terminal_clear();
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void vga_set_cursor(uint8_t row, uint8_t column) {
    uint16_t pos = row * VGA_WIDTH + column;

    outb(CURSOR_INDEX_REGISTER, CURSOR_LOW_BYTE);
    outb(CURSOR_DATA_REGISTER, (uint8_t)(pos & 0xFF));

    outb(CURSOR_INDEX_REGISTER, CURSOR_HIGH_BYTE);
    outb(CURSOR_DATA_REGISTER, (uint8_t)((pos >> 8) & 0xFF));
}

void terminal_scroll() {
    for (int row = 1; row < VGA_HEIGHT; ++row) {
        for (int col = 0; col < VGA_WIDTH; ++col) {
            terminal_buffer[(row - 1) * VGA_WIDTH + col] = terminal_buffer[row * VGA_WIDTH + col];
        }
    }

    // Clear the last line
    uint16_t blank = ' ' | (0x07 << 8);  // space char with light grey on black
    for (int col = 0; col < VGA_WIDTH; ++col) {
        terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = blank;
    }
}

void terminal_update_cursor() {
    vga_set_cursor(terminal_row, terminal_column);
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
    while (terminal_row >= VGA_HEIGHT-1) {
        terminal_scroll();
        terminal_row--;
    }

    if (c == '\n') {
        terminal_row++;
        terminal_column = 0;
        return;
    } else if (c == '\b') {
        if (terminal_column > 0) {
            terminal_column--;
        } else if (terminal_row > 0) {
            terminal_row--;
            terminal_column = VGA_WIDTH - 1;
        } else {
            // Already at (0,0), nothing to backspace
            return;
        }

        terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
        return;
    } else if (c == '\t') {
        terminal_column = (terminal_column + TAB_SIZE) & ~(TAB_SIZE - 1);
        if (terminal_column >= VGA_WIDTH) {
            terminal_column = 0;
            terminal_row++;
        }

        return;
    }

    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

    terminal_column++;
    if (terminal_column >= VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
    }
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
        terminal_update_cursor();
    }
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}