#include <stdint.h>

uint8_t* build_bitmap(int bit_size);
void destroy_bitmap(uint8_t** _bit_tab);

void clear_bits(uint8_t *tab, int start, int len);
void set_bits(uint8_t* tab, int start, int len);
int get_bit(uint8_t* tab, int pos);
int is_any_bits_set(uint8_t* tab, int start, int len);
