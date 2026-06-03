
#include "mocc.h"
#include "unity.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define SAFE_PUSH_THREAD_COUNT 4
#define SAFE_PUSH_ITERATIONS 100
#define SAFE_PUSH_TOTAL (SAFE_PUSH_THREAD_COUNT * SAFE_PUSH_ITERATIONS)
#define SAFE_POP_THREAD_COUNT 5
#define SAFE_POP_ATTEMPTS 80
#define SAFE_READ_THREAD_COUNT 3
#define SAFE_READ_ITERATIONS 500

static mocc_object* g_mocc = NULL;

static const size_t DEFAULT_CAPACITY = 8;

void setUp(void) { g_mocc = NULL; }

void tearDown(void)
{
    if (g_mocc)
    {
        mocc_dtor(g_mocc);
        g_mocc = NULL;
    }
}

static void __helper_push_values(int values[], size_t count)
{
    size_t index;

    for (index = 0; index < count; ++index)
    {
        mocc_push_back(g_mocc, &values[index]);
    }
}

static void __helper_assert_contents(int expected[], size_t count)
{
    size_t index;
    void* ptr = NULL;

    for (index = 0; index < count; ++index)
    {
        TEST_ASSERT_EQUAL(MOCC_OK, mocc_at(g_mocc, index, &ptr));
        TEST_ASSERT_EQUAL_INT(expected[index], *(int*)ptr);
    }
}

/* ---------------------------
 * Create / Destroy
 * --------------------------*/
void test_ctor_invalid_size(void)
{
    TEST_ASSERT_EQUAL(MOCC_ERROR_INVALID_ARGUMENT, mocc_ctor(0, DEFAULT_CAPACITY, &g_mocc));
}

void test_ctor_invalid_output_pointer(void)
{
    TEST_ASSERT_EQUAL(MOCC_ERROR_INVALID_ARGUMENT, mocc_ctor(sizeof(int), DEFAULT_CAPACITY, NULL));
}

void test_create_and_destroy(void)
{
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc));
    TEST_ASSERT_NOT_NULL(g_mocc);
}

void test_destroy_null_object(void)
{
    TEST_ASSERT_EQUAL(MOCC_ERROR_INVALID_ARGUMENT, mocc_dtor(NULL));
}

/* ---------------------------
 * Size / Capacity
 * --------------------------*/
void test_size_capacity_initial(void)
{
    size_t size = 0;
    size_t cap = 0;

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_size(g_mocc, &size));
    TEST_ASSERT_EQUAL(0, size);
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_capacity(g_mocc, &cap));
    TEST_ASSERT_TRUE(cap >= DEFAULT_CAPACITY);
}

void test_reserve_invalid_capacity(void)
{
    size_t cap;

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    TEST_ASSERT_EQUAL(MOCC_ERROR_INVALID_ARGUMENT, mocc_reserve(g_mocc, 0));
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_capacity(g_mocc, &cap));
    TEST_ASSERT_TRUE(cap >= DEFAULT_CAPACITY);
}

void test_shrink_to_fit_empty_container(void)
{
    size_t size = 1;
    size_t cap = 1;

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_shrink_to_fit(g_mocc));
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_size(g_mocc, &size));
    TEST_ASSERT_EQUAL(0, size);
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_capacity(g_mocc, &cap));
    TEST_ASSERT_EQUAL(0, cap);
}

void test_reserve_grows_capacity(void)
{
    size_t cap;

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_reserve(g_mocc, 100));
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_capacity(g_mocc, &cap));
    TEST_ASSERT_TRUE(cap >= 100);
}

/* ---------------------------
 * Push / Pop
 * --------------------------*/
void test_push_back_and_size(void)
{
    int values[2] = {10, 20};
    size_t size = 0;

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    __helper_push_values(values, 2);

    TEST_ASSERT_EQUAL(MOCC_OK, mocc_size(g_mocc, &size));
    TEST_ASSERT_EQUAL(2, size);
    __helper_assert_contents(values, 2);
}

void test_push_back_capacity_growth(void)
{
    int values[12];
    size_t cap = 0;
    size_t size = 0;
    size_t index;

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    for (index = 0; index < 12; ++index)
    {
        values[index] = (int)index + 1;
    }

    __helper_push_values(values, 12);
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_size(g_mocc, &size));
    TEST_ASSERT_EQUAL(12, size);
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_capacity(g_mocc, &cap));
    TEST_ASSERT_TRUE(cap >= 12);
    __helper_assert_contents(values, 12);
}

void test_pop_back_decrements_size(void)
{
    int values[3] = {5, 7, 9};
    size_t size = 0;
    void* ptr = NULL;

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    __helper_push_values(values, 3);

    TEST_ASSERT_EQUAL(MOCC_OK, mocc_pop_back(g_mocc));
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_size(g_mocc, &size));
    TEST_ASSERT_EQUAL(2, size);
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_back(g_mocc, &ptr));
    TEST_ASSERT_EQUAL_INT(7, *(int*)ptr);
}

void test_pop_back_to_empty(void)
{
    int value = 42;
    size_t size = 1;

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    mocc_push_back(g_mocc, &value);
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_pop_back(g_mocc));
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_size(g_mocc, &size));
    TEST_ASSERT_EQUAL(0, size);
    TEST_ASSERT_EQUAL(MOCC_ERROR_EMPTY, mocc_front(g_mocc, (void**)&value));
}

/* ---------------------------
 * At / Front / Back
 * --------------------------*/
void test_at_front_back(void)
{
    int values[3] = {1, 2, 3};
    void* ptr = NULL;

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    __helper_push_values(values, 3);

    TEST_ASSERT_EQUAL(MOCC_OK, mocc_front(g_mocc, &ptr));
    TEST_ASSERT_EQUAL_INT(1, *(int*)ptr);
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_back(g_mocc, &ptr));
    TEST_ASSERT_EQUAL_INT(3, *(int*)ptr);
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_at(g_mocc, 1, &ptr));
    TEST_ASSERT_EQUAL_INT(2, *(int*)ptr);
}

void test_front_back_empty_error(void)
{
    void* ptr = NULL;

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    TEST_ASSERT_EQUAL(MOCC_ERROR_EMPTY, mocc_front(g_mocc, &ptr));
    TEST_ASSERT_EQUAL(MOCC_ERROR_EMPTY, mocc_back(g_mocc, &ptr));
}

/* ---------------------------
 * Erase
 * --------------------------*/
void test_erase_first_element(void)
{
    int values[3] = {1, 2, 3};
    int expected[2] = {2, 3};

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    __helper_push_values(values, 3);

    TEST_ASSERT_EQUAL(MOCC_OK, mocc_erase(g_mocc, 0));
    __helper_assert_contents(expected, 2);
}

void test_erase_middle_element(void)
{
    int values[4] = {10, 20, 30, 40};
    int expected[3] = {10, 30, 40};

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    __helper_push_values(values, 4);

    TEST_ASSERT_EQUAL(MOCC_OK, mocc_erase(g_mocc, 1));
    __helper_assert_contents(expected, 3);
}

void test_erase_last_element(void)
{
    int values[3] = {1, 2, 3};
    int expected[2] = {1, 2};
    void* ptr = NULL;
    size_t size = 0;

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    __helper_push_values(values, 3);

    TEST_ASSERT_EQUAL(MOCC_OK, mocc_erase(g_mocc, 2));
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_size(g_mocc, &size));
    TEST_ASSERT_EQUAL(2, size);
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_back(g_mocc, &ptr));
    TEST_ASSERT_EQUAL_INT(2, *(int*)ptr);
    __helper_assert_contents(expected, 2);
}

void test_erase_invalid_index(void)
{
    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    TEST_ASSERT_EQUAL(MOCC_ERROR_INVALID_INDEX, mocc_erase(g_mocc, 5));
}

/* ---------------------------
 * Clear
 * --------------------------*/
void test_clear_preserves_capacity_and_reuse(void)
{
    int values[3] = {1, 2, 3};
    int next_values[2] = {10, 20};
    size_t cap = 0;
    size_t size = 0;

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    __helper_push_values(values, 3);

    TEST_ASSERT_EQUAL(MOCC_OK, mocc_clear(g_mocc));
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_capacity(g_mocc, &cap));
    TEST_ASSERT_TRUE(cap >= DEFAULT_CAPACITY);

    __helper_push_values(next_values, 2);
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_size(g_mocc, &size));
    TEST_ASSERT_EQUAL(2, size);
    __helper_assert_contents(next_values, 2);
}

/* ---------------------------
 * Invalid access
 * --------------------------*/
void test_invalid_at_out_of_bounds(void)
{
    void* ptr = NULL;

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    TEST_ASSERT_EQUAL(MOCC_ERROR_INVALID_INDEX, mocc_at(g_mocc, 0, &ptr));
    TEST_ASSERT_EQUAL(MOCC_ERROR_INVALID_INDEX, mocc_at(g_mocc, 1, &ptr));
}

void test_pop_back_empty(void)
{
    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    TEST_ASSERT_EQUAL(MOCC_ERROR_EMPTY, mocc_pop_back(g_mocc));
}

/* ---------------------------
 * Safe wrappers
 * --------------------------*/
void test_safe_push_back_and_safe_front_back(void)
{
    int value = 99;
    void* ptr = NULL;
    size_t size = 0;
    size_t cap = 0;

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_safe_push_back(g_mocc, &value));
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_safe_size(g_mocc, &size));
    TEST_ASSERT_EQUAL(1, size);
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_safe_capacity(g_mocc, &cap));
    TEST_ASSERT_TRUE(cap >= DEFAULT_CAPACITY);
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_safe_front(g_mocc, &ptr));
    TEST_ASSERT_EQUAL_INT(99, *(int*)ptr);
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_safe_back(g_mocc, &ptr));
    TEST_ASSERT_EQUAL_INT(99, *(int*)ptr);
}

typedef struct push_thread_args
{
    mocc_object* mocc;
    int start_value;
} push_thread_args_t;

typedef struct pop_thread_args
{
    mocc_object* mocc;
    int attempts;
} pop_thread_args_t;

static void* __helper_push_thread(void* arg)
{
    push_thread_args_t* args = (push_thread_args_t*)arg;
    int index;

    for (index = 0; index < SAFE_PUSH_ITERATIONS; ++index)
    {
        int value = args->start_value + index;
        TEST_ASSERT_EQUAL(MOCC_OK, mocc_safe_push_back(args->mocc, &value));
    }

    return NULL;
}

static void* __helper_pop_thread(void* arg)
{
    pop_thread_args_t* args = (pop_thread_args_t*)arg;
    int index;

    for (index = 0; index < args->attempts; ++index)
    {
        mocc_safe_pop_back(args->mocc);
    }

    return NULL;
}

typedef struct size_thread_args
{
    mocc_object* mocc;
    int iterations;
} size_thread_args_t;

static void* __helper_size_thread(void* arg)
{
    size_thread_args_t* args = (size_thread_args_t*)arg;
    int index;
    size_t size;
    size_t cap;

    for (index = 0; index < args->iterations; ++index)
    {
        TEST_ASSERT_EQUAL(MOCC_OK, mocc_safe_size(args->mocc, &size));
        TEST_ASSERT_EQUAL(MOCC_OK, mocc_safe_capacity(args->mocc, &cap));
    }

    return NULL;
}

void test_safe_clear_and_safe_at(void)
{
    int values[2] = {3, 4};
    void* ptr = NULL;

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);
    __helper_push_values(values, 2);
    TEST_ASSERT_EQUAL(MOCC_OK, mocc_safe_clear(g_mocc));
    TEST_ASSERT_EQUAL(MOCC_ERROR_INVALID_INDEX, mocc_safe_at(g_mocc, 0, &ptr));
}

void test_safe_concurrent_push_back(void)
{
    pthread_t threads[SAFE_PUSH_THREAD_COUNT];
    push_thread_args_t args[SAFE_PUSH_THREAD_COUNT];
    char found[SAFE_PUSH_TOTAL] = {0};
    void* ptr = NULL;
    size_t size = 0;
    int index;
    int value;

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);

    for (index = 0; index < SAFE_PUSH_THREAD_COUNT; ++index)
    {
        args[index].mocc = g_mocc;
        args[index].start_value = index * SAFE_PUSH_ITERATIONS;
        TEST_ASSERT_EQUAL(
            0, pthread_create(&threads[index], NULL, __helper_push_thread, &args[index]));
    }

    for (index = 0; index < SAFE_PUSH_THREAD_COUNT; ++index)
    {
        TEST_ASSERT_EQUAL(0, pthread_join(threads[index], NULL));
    }

    TEST_ASSERT_EQUAL(MOCC_OK, mocc_size(g_mocc, &size));
    TEST_ASSERT_EQUAL(SAFE_PUSH_TOTAL, size);

    for (index = 0; index < SAFE_PUSH_TOTAL; ++index)
    {
        TEST_ASSERT_EQUAL(MOCC_OK, mocc_at(g_mocc, index, &ptr));
        value = *(int*)ptr;
        TEST_ASSERT_TRUE(value >= 0 && value < SAFE_PUSH_TOTAL);
        TEST_ASSERT_FALSE(found[value]);
        found[value] = 1;
    }

    for (index = 0; index < SAFE_PUSH_TOTAL; ++index)
    {
        TEST_ASSERT_TRUE(found[index]);
    }
}

void test_safe_concurrent_pop_back(void)
{
    pthread_t threads[SAFE_POP_THREAD_COUNT];
    pop_thread_args_t args[SAFE_POP_THREAD_COUNT];
    size_t size = 0;
    int index;

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);

    for (index = 0; index < SAFE_PUSH_TOTAL; ++index)
    {
        mocc_push_back(g_mocc, &index);
    }

    for (index = 0; index < SAFE_POP_THREAD_COUNT; ++index)
    {
        args[index].mocc = g_mocc;
        args[index].attempts = SAFE_POP_ATTEMPTS;
        TEST_ASSERT_EQUAL(0,
                          pthread_create(&threads[index], NULL, __helper_pop_thread, &args[index]));
    }

    for (index = 0; index < SAFE_POP_THREAD_COUNT; ++index)
    {
        TEST_ASSERT_EQUAL(0, pthread_join(threads[index], NULL));
    }

    TEST_ASSERT_EQUAL(MOCC_OK, mocc_size(g_mocc, &size));
    TEST_ASSERT_EQUAL(0, size);
}

void test_safe_concurrent_size_reads(void)
{
    pthread_t threads[SAFE_READ_THREAD_COUNT];
    size_thread_args_t args[SAFE_READ_THREAD_COUNT];
    int index;
    size_t size = 0;
    void* result = NULL;

    mocc_ctor(sizeof(int), DEFAULT_CAPACITY, &g_mocc);

    for (index = 0; index < SAFE_PUSH_TOTAL; ++index)
    {
        mocc_push_back(g_mocc, &index);
    }

    for (index = 0; index < SAFE_READ_THREAD_COUNT; ++index)
    {
        args[index].mocc = g_mocc;
        args[index].iterations = SAFE_READ_ITERATIONS;
        TEST_ASSERT_EQUAL(
            0, pthread_create(&threads[index], NULL, __helper_size_thread, &args[index]));
    }

    for (index = 0; index < SAFE_READ_THREAD_COUNT; ++index)
    {
        TEST_ASSERT_EQUAL(0, pthread_join(threads[index], &result));
    }

    TEST_ASSERT_EQUAL(MOCC_OK, mocc_size(g_mocc, &size));
    TEST_ASSERT_EQUAL(SAFE_PUSH_TOTAL, size);
}

/* ---------------------------
 * Main
 * --------------------------*/
int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_ctor_invalid_size);
    RUN_TEST(test_ctor_invalid_output_pointer);
    RUN_TEST(test_create_and_destroy);
    RUN_TEST(test_destroy_null_object);
    RUN_TEST(test_size_capacity_initial);
    RUN_TEST(test_reserve_invalid_capacity);
    RUN_TEST(test_shrink_to_fit_empty_container);
    RUN_TEST(test_reserve_grows_capacity);
    RUN_TEST(test_push_back_and_size);
    RUN_TEST(test_push_back_capacity_growth);
    RUN_TEST(test_pop_back_decrements_size);
    RUN_TEST(test_pop_back_to_empty);
    RUN_TEST(test_at_front_back);
    RUN_TEST(test_front_back_empty_error);
    RUN_TEST(test_erase_first_element);
    RUN_TEST(test_erase_middle_element);
    RUN_TEST(test_erase_last_element);
    RUN_TEST(test_erase_invalid_index);
    RUN_TEST(test_clear_preserves_capacity_and_reuse);
    RUN_TEST(test_invalid_at_out_of_bounds);
    RUN_TEST(test_pop_back_empty);
    RUN_TEST(test_safe_push_back_and_safe_front_back);
    RUN_TEST(test_safe_clear_and_safe_at);
    RUN_TEST(test_safe_concurrent_push_back);
    RUN_TEST(test_safe_concurrent_pop_back);
    RUN_TEST(test_safe_concurrent_size_reads);

    return UNITY_END();
}
