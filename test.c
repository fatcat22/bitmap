#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bitmap.h"

void test_bitmap_build()
{
	const int org_length = 9;
	const int align_byte = 2;
	int i;
	
	uint8_t* bittab = build_bitmap(org_length);
	for (i = 0; i < align_byte; ++i)
	{
		if (0 != bittab[i])
		{
			printf("build_bitmap test failed. %d\n", bittab[i]);
			exit(0);
		}
	}
}

void test_bitmap_destroy()
{
	uint8_t* bittab = build_bitmap(9);

	destroy_bitmap(&bittab);
	/*FIXME: how to test memory is freed?*/
	if (0 != bittab)
	{
		printf("destroy_bitmap test failed.\n");
		exit(0);
	}
}

void test_bitmap_set()
{
#define BITMAP_SIZE 27
#define TEST_DATA_BYTES ((BITMAP_SIZE + 8) >> 3)
	struct {
		int begin;
		int length;
	} set_order[] = {
		{0, 1}, {3, 2}, {6, 3}, {10, 1}, {12, 3}, {16, 8}, {25, 2}
	};
	uint8_t test_data[TEST_DATA_BYTES] = {0xD9, 0x75, 0xFF, 6};
	uint8_t* bit_tab = build_bitmap(BITMAP_SIZE);
	int i;

	for (i = 0; i < sizeof(set_order)/sizeof(set_order[0]); ++i)
	{
		set_bits(bit_tab, set_order[i].begin, set_order[i].length);
	}

	if (memcmp(test_data, bit_tab, TEST_DATA_BYTES) != 0)
	{
		printf("set_bits test failed.\n");
		exit(0);
	}

#undef BITMAP_SIZE
#undef TEST_DATA_BYTES
}

void test_bitmap_clear()
{
#define BITMAP_SIZE 27
#define TEST_DATA_BYTES ((BITMAP_SIZE + 8) >> 3)
	struct {
		int begin;
		int length;
	} clear_order[] = {
		{0, 2}, {6, 5}, {14, 1}, {16, 9}
	};
	uint8_t test_data[TEST_DATA_BYTES] = {0x3C, 0xB8, 0, 6};
	uint8_t* bit_tab = build_bitmap(BITMAP_SIZE);
	int i;

	set_bits(bit_tab, 0, BITMAP_SIZE);

	for (i = 0; i < sizeof(clear_order)/sizeof(clear_order[0]); ++i)
		clear_bits(bit_tab, clear_order[i].begin, clear_order[i].length);

	if (memcmp(test_data, bit_tab, TEST_DATA_BYTES) != 0)
	{
		printf("clear_bits test failed.\n");
		exit(0);
	}
#undef BITMAP_SIZE
#undef TEST_DATA_BYTES
}

void test_bitmap_get()
{
#define BITMAP_SIZE 10
	struct {
		int start;
		int length;
	}set_order[] = {
		{0, 1}, {2, 3}, {7, 1}, {9, 1}
	};
	uint8_t result[BITMAP_SIZE] = {1, 0, 1, 1, 1, 0, 0, 1, 0, 1};
	uint8_t* bit_tab = build_bitmap(BITMAP_SIZE);
	int i;

	for (i = 0; i < sizeof(set_order)/sizeof(set_order[0]); ++i)
		set_bits(bit_tab, set_order[i].start, set_order[i].length);

	for (i = 0; i < BITMAP_SIZE; ++i)
	{
		if (get_bit(bit_tab, i) != result[i])
		{
			printf("get_bit test failed.\n");
			exit(0);
		}
	}
#undef BITMAP_SIZE
}

void test_bitmap_any_bits_set()
{
#define BITMAP_SIZE 19
	struct {
		int start;
		int length;
	}set_order[] = {
		{0, 1}, {3, 1}, {5, 3}, {11, 1}, {18, 1}
	};
	struct {
		int pos;
		int length;
		int be_set;
	}result[] = {
		{0, 3, 1}, {1, 2, 0}, {2, 3, 1}, {4, 1, 0}, {4, 5, 1}, {5, 3, 1}, {8, 10, 1}, {0xc, 5, 0}, {18, 1, 1}
	};
	uint8_t* bit_tab = build_bitmap(BITMAP_SIZE);
	int i;

	for (i = 0; i < sizeof(set_order)/sizeof(set_order[0]); ++i)
	{
		set_bits(bit_tab, set_order[i].start, set_order[i].length);
	}

	for (i = 0; i < sizeof(result)/sizeof(result[0]); ++i)
	{
		if (result[i].be_set != is_any_bits_set(bit_tab, result[i].pos, result[i].length))
		{
			printf("is_any_bits_set test failed.\n");
			exit(0);
		}
	}
}

int main()
{
	test_bitmap_build();

	test_bitmap_destroy();

	test_bitmap_set();
	
	test_bitmap_clear();

	test_bitmap_get();

	test_bitmap_any_bits_set();

	printf("test success.\n");
	return 0;
}
