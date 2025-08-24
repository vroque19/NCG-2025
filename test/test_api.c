#include "test_api.h"
// Assertion function for uint8_t equality
void assert_equal_uint8(uint8_t actual, uint8_t expected, const char *message) {
    if (actual == expected) {
        printf("✅ PASS: %s (Actual: %u, Expected: %u)\n", message, actual, expected);
    } else {
        printf("❌ FAIL: %s (Actual: %u, Expected: %u)\n", message, actual, expected);
    }
}
// Assertion function for int equality
void assert_equal_int(int actual, int expected, const char *message) {
    if (actual == expected) {
        printf("✅ PASS: %s (Actual: %d, Expected: %d)\n", message, actual, expected);
    } else {
        printf("❌ FAIL: %s (Actual: %d, Expected: %d)\n", message, actual, expected);
    }
}
// Assertion function for boolean conditions
void assert_true(bool condition, const char *message) {
    if (condition) {
        printf("✅ PASS: %s\n", message);
    } else {
        printf("❌ FAIL: %s\n", message);
    }
}

// Dummy assert_false for convenience in tests
void assert_false(bool condition, const char *message) {
    assert_true(!condition, message);
}