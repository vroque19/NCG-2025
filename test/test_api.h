#ifndef __TEST_API_H__
#define __TEST_API_H__
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

void assert_equal_uint8(uint8_t actual, uint8_t expected, const char *message);
void assert_equal_int(int actual, int expected, const char *message);
void assert_true(bool condition, const char *message);
void assert_false(bool condition, const char *message);
#endif