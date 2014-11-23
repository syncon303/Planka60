/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * scan matrix
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"

#define DERP 0

#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static uint16_t matrix[MATRIX_ROWS];
static uint16_t matrix_debouncing[MATRIX_ROWS];

#if DERP
static matrix_row_t read_cols(void);
static void init_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);
#else
static uint16_t read_rows(void);
static void init_rows(void);
static void unselect_cols(void);
static void select_col(uint8_t col);
#endif

inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    // initialize row and col
#if DERP
	unselect_rows();
    init_cols();
#else
    unselect_cols();
    init_rows();
#endif

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
}
#if DERP
uint8_t matrix_scan(void)
{
	uint8_t bit_cnt = 0, virt_row = 0;
	uint8_t spill = 0;
	static uint8_t BIT_MASK = (1<<MATRIX_ROWS) - 1;
    static matrix_row_t MATRIX_MASK = (1<<MATRIX_COLS) - 1;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
		matrix_row_t cols = spill;
		while (1) {
			select_row(virt_row++);
            _delay_us(3);  // settling delay
			uint8_t virt_col = BIT_MASK & read_cols();
	        unselect_rows();
			cols |= (virt_col << bit_cnt);
			bit_cnt += MATRIX_ROWS;
			if (bit_cnt >= MATRIX_COLS) {
				cols &= MATRIX_MASK;
				if (bit_cnt)
					spill = (virt_col >> (MATRIX_ROWS + MATRIX_COLS - bit_cnt ));
				else
					spill = 0;
				bit_cnt -= MATRIX_COLS; 
				break;
			}
		}

        //matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                debug("bounce!: "); debug_hex(debouncing); debug("\n");
            }
            debouncing = DEBOUNCE;
        }
    }

    if (debouncing) {
        if (--debouncing) {
            _delay_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}
#else

uint8_t matrix_scan(void)
{
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {  // 0-16
        select_col(col);
        _delay_us(3); // TODO: Determine the correct value needed here.
		uint16_t rows = read_rows();

        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {  // 0-5
            bool prev_bit = matrix_debouncing[row] & ((uint16_t)1<<col);
            bool curr_bit = rows & (1<<row);
            if (prev_bit != curr_bit) {
                matrix_debouncing[row] ^= ((uint16_t)1<<col);
                if (debouncing) {
                    dprint("bounce!: "); dprintf("%02X", debouncing); dprintln();
                }
                debouncing = DEBOUNCE;
            }
        }
        unselect_cols();
    }

    if (debouncing) {
        if (--debouncing) {
            _delay_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}
#endif

bool matrix_is_modified(void)
{
    if (debouncing) return false;
    return true;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print("\nr/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row); print(": ");
        pbin_reverse16(matrix_get_row(row));
        print("\n");
    }
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop16(matrix[i]);
    }
    return count;
}

/* Column pin configuration
 * col: 0   1   2   3   4   5   6   7   8   9   10  11  12  13
 * pin: F0  F1  E6  C7  C6  B6  D4  B1  B0  B5  B4  D7  D6  B3  (Rev.A)
 * pin:                                 B7                      (Rev.B)
 */
#if 1

static void  init_rows(void)
{
    // Input with pull-up(DDR:0, PORT:1)
    DDRD  &= ~0b10111100;
    PORTD |= 0b10111100;
}

static matrix_row_t read_rows(void)
{
    return (PIND&(1<<2) ? 0 : (1<<0)) |
           (PIND&(1<<3) ? 0 : (1<<1)) |
           (PIND&(1<<4) ? 0 : (1<<2)) |
           (PIND&(1<<5) ? 0 : (1<<3)) |
           (PIND&(1<<7) ? 0 : (1<<4));
}
#else
static void  init_rows(void)
{
    // Input with pull-up(DDR:0, PORT:1)
    DDRF  &= ~(1<<7);
    PORTF |=  (1<<7);
    DDRE  &= ~(1<<6);
    PORTE |=  (1<<6);
    DDRD  &= ~(1<<1 | 1<<0);
    PORTD |=  (1<<1 | 1<<0);
    DDRC  &= ~(1<<7 | 1<<6);
    PORTC |=  (1<<7 | 1<<6);
    DDRB  &= ~(1<<7 | 1<<6 | 1<< 5 | 1<<4 | 1<<3 | 1<<2 | 1<<1 | 1<<0);
    PORTB |=  (1<<7 | 1<<6 | 1<< 5 | 1<<4 | 1<<3 | 1<<2 | 1<<1 | 1<<0);
}

static matrix_row_t read_rows(void)
{
    return (PINE&(1<<6) ? 0 : (1<<0)) |
           (PINB&(1<<0) ? 0 : (1<<1)) |
           (PINB&(1<<1) ? 0 : (1<<2)) |
           (PINB&(1<<2) ? 0 : (1<<3)) |
           (PIND&(1<<1) ? 0 : (1<<4)) |
           (PIND&(1<<0) ? 0 : (1<<5)) |
           (PINB&(1<<3) ? 0 : (1<<6)) |
           (PINB&(1<<7) ? 0 : (1<<7)) |
           (PINB&(1<<6) ? 0 : (1<<8)) | 
           (PINB&(1<<5) ? 0 : (1<<9)) |
           (PINB&(1<<4) ? 0 : (1<<10)) |
           (PINC&(1<<6) ? 0 : (1<<11)) |
           (PINC&(1<<7) ? 0 : (1<<12)) |
           (PINF&(1<<7) ? 0 : (1<<13));
}

#endif
/* Row pin configuration
 * row: 0   1   2   3   4
 * pin: D0  D1  D2  D3  D5
 */
#if 1

static void unselect_cols(void)
{
    // Hi-Z(DDR:0, PORT:0) to unselect
    DDRF  &= ~(1<<7);
    PORTF &= ~(1<<7);
    DDRE  &= ~(1<<6);
    PORTE &= ~(1<<6);
    DDRD  &= ~(1<<1 | 1<<0);
    PORTD &= ~(1<<1 | 1<<0);
    DDRC  &= ~(1<<7 | 1<<6);
    PORTC &= ~(1<<7 | 1<<6);
    DDRB  &= ~(1<<7 | 1<<6 | 1<< 5 | 1<<4 | 1<<3 | 1<<2 | 1<<1 | 1<<0);
    PORTB &= ~(1<<7 | 1<<6 | 1<< 5 | 1<<4 | 1<<3 | 1<<2 | 1<<1 | 1<<0);
}


static void select_col(uint8_t row)
{
    // Output low(DDR:1, PORT:0) to select
    switch (row) {
        case 0:
            DDRE  |= (1<<6);
            PORTE &= ~(1<<6);
            break;
        case 1:
            DDRB  |= (1<<0);
            PORTB &= ~(1<<0);
            break;
        case 2:
            DDRB  |= (1<<1);
            PORTB &= ~(1<<1);
            break;
        case 3:
            DDRB  |= (1<<2);
            PORTB &= ~(1<<2);
            break;
        case 4:
            DDRD  |= (1<<1);
            PORTD &= ~(1<<1);
            break;
        case 5:
            DDRD  |= (1<<0);
            PORTD &= ~(1<<0);
            break;
        case 6:
            DDRB  |= (1<<3);
            PORTB &= ~(1<<3);
            break;
        case 7:
            DDRB  |= (1<<7);
            PORTB &= ~(1<<7);
            break;
        case 8:
            DDRB  |= (1<<6);
            PORTB &= ~(1<<6);
            break;
        case 9:
            DDRB  |= (1<<5);
            PORTB &= ~(1<<5);
            break;
        case 10:
            DDRB  |= (1<<4);
            PORTB &= ~(1<<4);
            break;
        case 11:
            DDRC  |= (1<<6);
            PORTC &= ~(1<<6);
            break;
        case 12:
            DDRC  |= (1<<7);
            PORTC &= ~(1<<7);
            break;
        case 13:
            DDRF  |= (1<<7);
            PORTF &= ~(1<<7);
            break;
    }
}
#else
static void unselect_cols(void)
{
    // Hi-Z(DDR:0, PORT:0) to unselect
    DDRD  |= 0b10111100;
    PORTD |= 0b10111100;
}

static void select_col(uint8_t row)
{
    // Output low(DDR:1, PORT:0) to select
    switch (row) {
        case 0:
            PORTD &= ~(1<<2);
            break;
        case 1:
            PORTD &= ~(1<<3);
            break;
        case 2:
            PORTD &= ~(1<<4);
            break;
        case 3:
            PORTD &= ~(1<<5);
            break;
        case 4:
            PORTD &= ~(1<<7);
            break;
    }
}

#endif