#include "unity.h"
#include "mocc.h"


void setUp(void) {
}


void tearDown(void) {
}

void test_add_should_return_correct_sum(void) {
    TEST_ASSERT_EQUAL_INT(8, mocc_function(5, 3));
    TEST_ASSERT_EQUAL_INT(8, mocc_function(5, 3));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_add_should_return_correct_sum);

    return UNITY_END();
}