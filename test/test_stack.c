#include "stack.h"
#include <stdio.h>

void print_tower_state(const char* name, tower_stack *tower) {
    printf("  %s (Rings: %u, Top Index: %d): ", name, tower->ring_count, tower->top_idx);
    if (is_empty_tower(tower)) {
        printf("[Empty]\n");
    } else {
        for (int i = 0; i <= tower->top_idx; i++) {
            printf("%d ", tower->rings[i]);
        }
        printf("\n");
    }
}

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


// Test Case 1: stack init
void test_stack_init(tower_stack *tower) {
    init_stack(tower);
    printf("--- Test Case 1: Stack (id > 0) Initialization ---\n");
    assert_equal_uint8(tower->tower_id, 0, "Tower ID is 0");
    tower->top_idx = -1;
    assert_true(is_empty_tower(tower), "TC1.1: Stack 0 is empty after initialization.");
    assert_false(is_full_tower(tower), "TC1.2: Tower is not full after initialization");
    assert_equal_int(tower->top_idx, -1, "TC1.3: Top index is -1 after initialization");
    assert_equal_uint8((int)tower->ring_count, 0, "TC1.4: Ring count is 0 after initialization.");
    print_tower_state("test tower: ", tower);
}

// Test Case 2: stack push
void test_stack_push(tower_stack *tower) {
    printf("\n--- Test Case 3: Pushing rings (LIFO) ---\n");
    push_tower(tower, 3);
    assert_equal_uint8(tower->rings[tower->top_idx], 3, "TC2.1: Pushed 3 to tower");
    push_tower(tower, 2);
    assert_equal_uint8(tower->rings[tower->top_idx], 2, "TC2.2: Pushed 2 to tower");
    push_tower(tower, 1);
    assert_equal_uint8(tower->rings[tower->top_idx], 1, "TC2.3: Pushed 1 to tower");
    print_tower_state("test tower: ", tower);
}
// Test Case 3: Stack Pop (removing rings, LIFO)
void test_stack_pop(tower_stack *tower) {
    printf("\n--- Test Case 3: Popping rings (LIFO) ---\n");

    uint8_t popped_ring;

    popped_ring = pop_tower(tower);
    assert_equal_uint8(popped_ring, 1, "TC3.1: Popped ring is size 1 (LIFO).");
    assert_equal_uint8(tower->top_idx, 1, "TC3.2: Top index is 1 after popping one ring.");
    assert_equal_uint8(tower->ring_count, 2, "TC3.3: ring count is 2 after popping one ring.");
    print_tower_state("test tower", tower);

    popped_ring = pop_tower(tower);
    assert_equal_uint8(popped_ring, 2, "TC3.4: Popped ring is size 2.");
    assert_equal_uint8(tower->top_idx, 0, "TC3.5: Top index is 0 after popping two rings.");
    assert_equal_uint8(tower->ring_count, 1, "TC3.6: ring count is 1 after popping two rings.");
    print_tower_state("test tower", tower);

    popped_ring = pop_tower(tower);
    assert_equal_uint8(popped_ring, 3, "TC3.7: Popped ring is size 3.");
    assert_true(is_empty_tower(tower), "TC3.8: Stack is empty after popping all rings.");
    assert_equal_uint8(tower->top_idx, -1, "TC3.9: Top index is -1 after popping all rings.");
    assert_equal_uint8(tower->ring_count, 0, "TC3.10: ring count is 0 after popping all rings.");
    print_tower_state("test tower", tower);
}
// Test Case 4: stack overflow
void test_stack_overflow(tower_stack *tower) {
    printf("\n--- Test Case 4: Stack Overflow ---\n");
    init_stack(tower);
    // Fill the stack to max
    for (int i = 0; i < MAX_RINGS; i++) {
        push_tower(tower, i + 1);
    }
    assert_true(is_full_tower(tower), "TC4.1: Stack is full after pushing MAX_RINGS.");
    // Try to push one more
    int result = push_tower(tower, 99);
    assert_equal_int(result, -1, "TC4.2: Pushing to full stack returns error.");
    print_tower_state("test tower", tower);
}

// Test Case 5: stack underflow
void test_stack_underflow(tower_stack *tower) {
    printf("\n--- Test Case 5: Stack Underflow ---\n");
    init_stack(tower);
    assert_true(is_empty_tower(tower), "TC5.1: Stack is empty before pop.");
    int popped = pop_tower(tower);
    assert_equal_uint8(popped, -1, "TC5.2: Popping from empty stack returns error.");
    print_tower_state("test tower", tower);
}

// Test Case 6: stack get id
void test_stack_get_id(tower_stack *tower) {
    printf("\n--- Test Case 6: Stack Get ID ---\n");
    init_stack(tower);
    tower->tower_id = 42;
    assert_equal_uint8(tower->tower_id, 42, "TC6.1: Tower ID is set and retrieved correctly.");
    print_tower_state("test tower", tower);
}

// Test Case 7: stack is empty
void test_stack_is_empty(tower_stack *tower) {
    printf("\n--- Test Case 7: Stack Is Empty ---\n");
    init_stack(tower);
    assert_true(is_empty_tower(tower), "TC7.1: Stack is empty after init.");
    push_tower(tower, 1);
    assert_false(is_empty_tower(tower), "TC7.2: Stack is not empty after push.");
    pop_tower(tower);
    assert_true(is_empty_tower(tower), "TC7.3: Stack is empty after pop.");
    print_tower_state("test tower", tower);
}

// Test Case 8: stack ring count
void test_stack_ring_count(tower_stack *tower) {
    printf("\n--- Test Case 8: Stack Ring Count ---\n");
    init_stack(tower);
    assert_equal_uint8(tower->ring_count, 0, "TC8.1: Ring count is 0 after init.");
    push_tower(tower, 1);
    assert_equal_uint8(tower->ring_count, 1, "TC8.2: Ring count is 1 after one push.");
    push_tower(tower, 2);
    assert_equal_uint8(tower->ring_count, 2, "TC8.3: Ring count is 2 after two pushes.");
    pop_tower(tower);
    assert_equal_uint8(tower->ring_count, 1, "TC8.4: Ring count is 1 after one pop.");
    pop_tower(tower);
    assert_equal_uint8(tower->ring_count, 0, "TC8.5: Ring count is 0 after popping all.");
    print_tower_state("test tower", tower);
}

int main() {
    tower_stack test_tower;
    test_stack_init(&test_tower);
    test_stack_push(&test_tower);
    test_stack_pop(&test_tower);
    test_stack_overflow(&test_tower);
    test_stack_underflow(&test_tower);
    test_stack_get_id(&test_tower);
    test_stack_is_empty(&test_tower);
    test_stack_ring_count(&test_tower);
}