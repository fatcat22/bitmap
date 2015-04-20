#include "bitmap.h"
#include <stdlib.h>
#include <string.h>

/*if return value < 0, return the return_value in argument.*/
typedef int (* op_block_t)(uint8_t* cur_tab, int mask, uintptr_t* return_value);

static inline uintptr_t bitmap_each_byte(uint8_t* tab, int start, int len, op_block_t op_block)
{

	int end, mask, end1;
	uintptr_t result = 0;

	end = start + len;
	tab += start >> 3;
	mask = 0xff << (start & 7);
	if ((start & ~7) == (end & ~7)) {
		if (start < end) {
			mask &= ~(0xff << (end & 7));

			if (op_block(tab, mask, &result) < 0)
				return result;
		}
	} 
	else {
		if (op_block(tab, mask, &result) < 0)
			return result;
		++tab;
		start = (start + 8) & ~7;
		end1 = end & ~7;
		while (start < end1) {
			if (op_block(tab, 0xff, &result) < 0)
				return result;

			++tab;
			start += 8;
		}
		if (start < end) {
			mask = ~(0xff << (end & 7));
			if (op_block(tab, mask, &result) < 0)
				return result;
		}
	}

	return result;
}


static inline int bitmap_clear(uint8_t* cur_tab, int mask, uintptr_t* return_value)
{
	*cur_tab &= ~mask;
	return 0;
}

static inline int bitmap_set(uint8_t* cur_tab, int mask, uintptr_t* return_value)
{
	*cur_tab |= mask;
}

static inline int bitmap_get(uint8_t* cur_tab, int mask, uintptr_t* return_value)
{
	if ((*cur_tab & mask) == 0) {
		return 0;
	} else {
		*return_value = 1;
		return -1;
	}
}


uint8_t* build_bitmap(int bit_size)
{
	const int bit_tab_length = (bit_size + 8) >> 3;
	uint8_t* bit_tab = malloc(bit_tab_length);

	if (0 == bit_tab)
		return 0;

	memset(bit_tab, 0, bit_tab_length);
	return bit_tab;
}

void destroy_bitmap(uint8_t** _bit_tab)
{
	uint8_t* bit_tab = 0;

	if (0 == _bit_tab)
		return;

	bit_tab = *_bit_tab;
	if (bit_tab)
	{
		free(bit_tab);
		bit_tab = 0;
	}

	*_bit_tab = 0;
}

void clear_bits(uint8_t *tab, int start, int len)
{
	bitmap_each_byte(tab, start, len, bitmap_clear);
}

void set_bits(uint8_t* tab, int start, int len)
{
	bitmap_each_byte(tab, start, len, bitmap_set);
}

int get_bit(uint8_t* tab, int pos)
{
	return bitmap_each_byte(tab, pos, 1, bitmap_get);
}

int is_any_bits_set(uint8_t* tab, int start, int len)
{
	return bitmap_each_byte(tab, start, len, bitmap_get);
}
