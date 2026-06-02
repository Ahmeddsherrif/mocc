#include "mocc.h"
#include "unity.h"

#include <string.h>

static mocc_object* g_mocc = NULL;

void setUp(void) { g_mocc = NULL; }

void tearDown(void)
{
    if (g_mocc)
    {
        mocc_destroy(g_mocc);
        g_mocc = NULL;
    }
}

/* ---------------------------
 * Create / Destroy
 * --------------------------*/
void test_create_and_destroy(void)
{
    mocc_error err = mocc_create(sizeof(int), 4, &g_mocc);

    TEST_ASSERT_EQUAL(MOCC_OK, err);
    TEST_ASSERT_NOT_NULL(g_mocc);

    err = mocc_destroy(g_mocc);
    TEST_ASSERT_EQUAL(MOCC_OK, err);

    g_mocc = NULL;
}

/* ---------------------------
 * Size / Capacity
 * --------------------------*/
void test_size_capacity_initial(void)
{
    size_t size = 0, cap = 0;

    mocc_create(sizeof(int), 8, &g_mocc);

    TEST_ASSERT_EQUAL(MOCC_OK, mocc_size(g_mocc, &size));
    TEST_ASSERT_EQUAL(0, size);

    TEST_ASSERT_EQUAL(MOCC_OK, mocc_capacity(g_mocc, &cap));
    TEST_ASSERT_TRUE(cap >= 8);
}

void test_push_back_and_size(void)
{
    int a = 10, b = 20;
    size_t size;

    mocc_create(sizeof(int), 2, &g_mocc);

    mocc_push_back(g_mocc, &a);
    mocc_push_back(g_mocc, &b);

    mocc_size(g_mocc, &size);

    TEST_ASSERT_EQUAL(2, size);
}

/* ---------------------------
 * At / Front / Back
 * --------------------------*/
void test_at_front_back(void)
{
    int a = 1, b = 2, c = 3;
    void* ptr = NULL;

    mocc_create(sizeof(int), 2, &g_mocc);

    mocc_push_back(g_mocc, &a);
    mocc_push_back(g_mocc, &b);
    mocc_push_back(g_mocc, &c);

    mocc_front(g_mocc, &ptr);
    TEST_ASSERT_EQUAL_INT(1, *(int*)ptr);

    mocc_back(g_mocc, &ptr);
    TEST_ASSERT_EQUAL_INT(3, *(int*)ptr);

    mocc_at(g_mocc, 1, &ptr);
    TEST_ASSERT_EQUAL_INT(2, *(int*)ptr);
}

/* ---------------------------
 * Insert / Erase
 * --------------------------*/
void test_insert_middle(void)
{
    int a = 1, b = 3, mid = 2;
    void* ptr = NULL;

    mocc_create(sizeof(int), 2, &g_mocc);

    mocc_push_back(g_mocc, &a);
    mocc_push_back(g_mocc, &b);

    mocc_insert(g_mocc, 1, &mid);

    mocc_at(g_mocc, 1, &ptr);

    TEST_ASSERT_EQUAL_INT(2, *(int*)ptr);
}

void test_erase_element(void)
{
    int a = 1, b = 2, c = 3;
    void* ptr = NULL;

    mocc_create(sizeof(int), 3, &g_mocc);

    mocc_push_back(g_mocc, &a);
    mocc_push_back(g_mocc, &b);
    mocc_push_back(g_mocc, &c);

    mocc_erase(g_mocc, 1);

    mocc_at(g_mocc, 1, &ptr);

    TEST_ASSERT_EQUAL_INT(3, *(int*)ptr);
}

/* ---------------------------
 * Pop Back
 * --------------------------*/
void test_pop_back(void)
{
    int a = 5, b = 10;
    size_t size;

    mocc_create(sizeof(int), 2, &g_mocc);

    mocc_push_back(g_mocc, &a);
    mocc_push_back(g_mocc, &b);

    mocc_pop_back(g_mocc);

    mocc_size(g_mocc, &size);

    TEST_ASSERT_EQUAL(1, size);
}

/* ---------------------------
 * Clear
 * --------------------------*/
void test_clear(void)
{
    int a = 1, b = 2;
    size_t size;

    mocc_create(sizeof(int), 2, &g_mocc);

    mocc_push_back(g_mocc, &a);
    mocc_push_back(g_mocc, &b);

    mocc_clear(g_mocc);

    mocc_size(g_mocc, &size);

    TEST_ASSERT_EQUAL(0, size);
}

/* ---------------------------
 * Reserve / Shrink
 * --------------------------*/
void test_reserve(void)
{
    size_t cap;

    mocc_create(sizeof(int), 2, &g_mocc);

    TEST_ASSERT_EQUAL(MOCC_OK, mocc_reserve(g_mocc, 100));

    mocc_capacity(g_mocc, &cap);

    TEST_ASSERT_TRUE(cap >= 100);
}

void test_shrink_to_fit(void)
{
    int a = 1;
    size_t cap;
    size_t size;

    mocc_create(sizeof(int), 10, &g_mocc);

    mocc_push_back(g_mocc, &a);
    mocc_push_back(g_mocc, &a);

    mocc_shrink_to_fit(g_mocc);

    mocc_capacity(g_mocc, &cap);
    mocc_size(g_mocc, &size);

    TEST_ASSERT_TRUE(cap >= size);
}

/* ---------------------------
 * Error cases
 * --------------------------*/
void test_invalid_index_erase(void)
{
    mocc_create(sizeof(int), 2, &g_mocc);

    TEST_ASSERT_EQUAL(MOCC_ERROR_INVALID_INDEX, mocc_erase(g_mocc, 999));
}

void test_invalid_at(void)
{
    void* ptr = NULL;

    mocc_create(sizeof(int), 2, &g_mocc);

    TEST_ASSERT_EQUAL(MOCC_ERROR_INVALID_INDEX, mocc_at(g_mocc, 0, &ptr));
}

/* ---------------------------
 * Safe vs Unsafe consistency
 * --------------------------*/
void test_safe_push_and_size(void)
{
    int a = 7;
    size_t size;

    mocc_create(sizeof(int), 2, &g_mocc);

    mocc_safe_push_back(g_mocc, &a);

    mocc_safe_size(g_mocc, &size);

    TEST_ASSERT_EQUAL(1, size);
}

void test_safe_at(void)
{
    int a = 42;
    void* ptr = NULL;

    mocc_create(sizeof(int), 2, &g_mocc);

    mocc_safe_push_back(g_mocc, &a);

    mocc_safe_at(g_mocc, 0, &ptr);

    TEST_ASSERT_EQUAL_INT(42, *(int*)ptr);
}

/* ---------------------------
 * Main
 * --------------------------*/
int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_create_and_destroy);
    /*RUN_TEST(test_size_capacity_initial);
    RUN_TEST(test_push_back_and_size);
    RUN_TEST(test_at_front_back);
    RUN_TEST(test_insert_middle);
    RUN_TEST(test_erase_element);
    RUN_TEST(test_pop_back);
    RUN_TEST(test_clear);
    RUN_TEST(test_reserve);
    RUN_TEST(test_shrink_to_fit);
    RUN_TEST(test_invalid_index_erase);
    RUN_TEST(test_invalid_at);
    RUN_TEST(test_safe_push_and_size);
    RUN_TEST(test_safe_at);*/

    return UNITY_END();
}