#include <stdint.h>
#include <stddef.h>
#include <terminal.h>
#include <string.h>
#include <stdio.h>

// returns an 8-bit colour entry according to the VGA 16 colour pallet for the foreground and background
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)  {
	return fg | bg << 4;
}

// returns a 16-bit entry with an ASCII character as the high byte and the colour entry as the low byte
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t terminal_row;
size_t terminal_column;
size_t scroll_back_ln;
size_t scroll_offset;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;
uint16_t scroll_back_buffer[SCROLLBACK_MAX_LINES*VGA_WIDTH];

// clear the screen and the scrollback buffer, set all location variables to 0
void terminal_clear() {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}

    for (size_t y = 0; y < SCROLLBACK_MAX_LINES; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			scroll_back_buffer[index] = vga_entry(' ', terminal_color);
		}
	}

    terminal_row = 0;
    terminal_column = 0;
    scroll_back_ln = 0;
    scroll_offset = 0;
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

// scrolls the terminal when writes overrun the screen buffer
void terminal_scroll() {
    for (int row = 1; row < VGA_HEIGHT; ++row) {
        for (int col = 0; col < VGA_WIDTH; ++col) {
            terminal_buffer[(row - 1) * VGA_WIDTH + col] = terminal_buffer[row * VGA_WIDTH + col];
        }
    }

    // Clear the last line
    uint16_t blank = ' ' | (0x07 << 8);  // space char with light grey on black
    for (int col = 0; col < VGA_WIDTH; ++col) {
        terminal_buffer[TRUE_VGA_HEIGHT * VGA_WIDTH + col] = blank;
    }
}

void terminal_update_cursor() {
    vga_set_cursor(terminal_row, terminal_column);
}

// render a viewport of the scrollback buffer
void terminal_render_view() {
    if (scroll_back_ln < TRUE_VGA_HEIGHT) {
        // Not enough lines yet, copy what is available
        for (size_t y = 0; y < scroll_back_ln; y++) {
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                size_t src_index = y * VGA_WIDTH + x;
                size_t dst_index = y * VGA_WIDTH + x;
                terminal_buffer[dst_index] = scroll_back_buffer[src_index];
            }
        }
        // Clear remaining lines if any
        for (size_t y = scroll_back_ln; y < VGA_HEIGHT; y++) {
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                terminal_buffer[y * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
            }
        }
    } else if (scroll_back_ln >= TRUE_VGA_HEIGHT + (scroll_offset-1)) {
        // compute starting line in scrollback buffer to display
        size_t start_line = scroll_back_ln - TRUE_VGA_HEIGHT - scroll_offset + 1;

        for (size_t y = 0; y < TRUE_VGA_HEIGHT; y++) {
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                size_t src_index = (start_line + y) * VGA_WIDTH + x;
                size_t dst_index = y * VGA_WIDTH + x;
                terminal_buffer[dst_index] = scroll_back_buffer[src_index];
            }
        }
    }
}

// sets the line to scroll to
void scroll_to_line(uint16_t line) {
    if (scroll_back_ln <= TRUE_VGA_HEIGHT) return; // No scrollback yet

    if (line < TRUE_VGA_HEIGHT)
        line = (TRUE_VGA_HEIGHT-1);
    if (line > scroll_back_ln)
        line = scroll_back_ln;

    scroll_offset = scroll_back_ln - line;

    // Clamp to max scroll offset
    if (scroll_offset > scroll_back_ln - (TRUE_VGA_HEIGHT-1))
        scroll_offset = scroll_back_ln - (TRUE_VGA_HEIGHT-1);

    terminal_render_view();
}

void terminal_scroll_up(void) {
    scroll_to_line(scroll_back_ln-(++scroll_offset));
}

void terminal_scroll_down(void) {
    scroll_to_line(scroll_back_ln-(--scroll_offset));
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
    scroll_back_buffer[scroll_back_ln * VGA_WIDTH + x] = vga_entry(c, color);

    if (scroll_offset == 0 && y < VGA_HEIGHT) {
        terminal_buffer[index] = vga_entry(c, color);
    }
}

// puts a character to the screen
void terminal_putchar(char c) {
    while (terminal_row >= TRUE_VGA_HEIGHT) {
        terminal_scroll();
        terminal_row--;
    }

    if (c == '\n') {
        terminal_row++;
        terminal_column = 0;

        scroll_back_ln++;
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
            scroll_back_ln++;
        }

        return;
    }

    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

    terminal_column++;
    if (terminal_column >= VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
        scroll_back_ln++;
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