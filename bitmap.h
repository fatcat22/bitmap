#include <stdint.h>

static inline uint8_t* build_bitmap(int bit_size)
{
	const int bit_tab_length = (bit_size + 8) >> 3;
	uint8_t* bit_tab = malloc(bit_tab_length);

	if (0 == bit_tab)
		return 0;

	memset(bit_tab, 0, bit_tab_length);
	return bit_tab;
}

static inline void destroy_bitmap(uint8_t** _bit_tab)
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

static inline void clear_bits(uint8_t *tab, int start, int len)
{
	int end, mask, end1;

	end = start + len;
	tab += start >> 3;
	mask = 0xff << (start & 7);
	if ((start & ~7) == (end & ~7)) {
		if (start < end) {
			mask &= ~(0xff << (end & 7));
			*tab &= ~mask;
		}
	} else {
			*tab++ &= ~mask;
			start = (start + 8) & ~7;
			end1 = end & ~7;
			while (start < end1) {
				*tab++ = 0;
				start += 8;
			}
			if (start < end) {
				*tab &= 0xff << (end & 7);
			}
		}
}

static inline void set_bits(uint8_t* tab, int start, int len)
{
    int end, mask, end1;

    end = start + len;
    tab += start >> 3;
    mask = 0xff << (start & 7);
    if ((start & ~7) == (end & ~7)) {
        if (start < end) {
            mask &= ~(0xff << (end & 7));
            *tab |= mask;
        }
    } else {
        *tab++ |= mask;
        start = (start + 8) & ~7;
        end1 = end & ~7;
        while (start < end1) {
            *tab++ = 0xff;
            start += 8;
        }
        if (start < end) {
            mask = ~(0xff << (end & 7));
            *tab |= mask;
        }
    }
}

static inline int get_bit(uint8_t* tab, int pos)
{
	tab += pos >> 3;
	return (*tab >> (pos & 7)) & 1;
}

static inline int is_any_bits_set(uint8_t* tab, int start, int len)
{
	int end, mask, end1;

	end = start + len;
	tab += start >> 3;
	mask = 0xff << (start & 7);
	if ((start & ~7) == (end & ~7)) {
		if (start < end) {
			mask &= ~(0xff << (end & 7));
			return (*tab & mask) == 0 ? 0 : 1;
		}
	} else {
		if ((*tab & mask) != 0)
			return 1;

		++tab;
		start = (start + 8) & ~7;
		end1 = end & ~7;
		while (start < end1) {
			if (*tab != 0)
				return 1;

			++tab;
			start += 8;
		}
		if (start < end) {
			mask = ~(0xff << (end & 7));
			if ((*tab & mask) != 0)
				return 1;
		}
	}

	return 0;
}
