#include "mocc.h"
#include "logger.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define MOCC_DEFAULT_CAPACITY 8

#ifdef ASSERT_ENABLED
#define DEFFENSIVE_ASSERT(expr, error)                                                             \
    do                                                                                             \
    {                                                                                              \
        if (!(expr))                                                                               \
        {                                                                                          \
            LOG_ERROR("Assertion failed: (" #expr "): " #error);                                   \
            return error;                                                                          \
        }                                                                                          \
    } while (0)
#else
#define DEFFENSIVE_ASSERT(expr, error) ((void)0)
#endif
struct mocc_object
{
    void* _back;
    void* _front;
    void* _tail;
    size_t _size;
    size_t _capacity;
    size_t _element_size;
    pthread_mutex_t _mutex;
};

static mocc_error __mocc_lock(mocc_object* me)
{
    int err;

    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INVALID_ARGUMENT);

    err = pthread_mutex_lock(&me->_mutex);

    DEFFENSIVE_ASSERT(0 == err, MOCC_ERROR_INTERNAL);

    return MOCC_OK;
}

static mocc_error __mocc_unlock(mocc_object* me)
{
    int err;
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INVALID_ARGUMENT);

    err = pthread_mutex_unlock(&me->_mutex);

    DEFFENSIVE_ASSERT(0 == err, MOCC_ERROR_INTERNAL);

    return MOCC_OK;
}

mocc_error mocc_ctor(size_t element_size, size_t initial_capacity, mocc_object** object)
{
    mocc_object* mocc = NULL;
    void* buffer = NULL;
    int err;

    DEFFENSIVE_ASSERT(element_size > 0, MOCC_ERROR_INVALID_ARGUMENT);

    DEFFENSIVE_ASSERT(object, MOCC_ERROR_INVALID_ARGUMENT);

    mocc = calloc(1, sizeof(mocc_object));
    DEFFENSIVE_ASSERT(mocc, MOCC_ERROR_OUT_OF_MEMORY);

    buffer = calloc(initial_capacity, element_size);
    DEFFENSIVE_ASSERT(buffer, MOCC_ERROR_OUT_OF_MEMORY);

    mocc->_element_size = element_size;
    mocc->_front = buffer;
    mocc->_back = (char*)buffer - element_size;
    mocc->_tail = (char*)buffer + initial_capacity * element_size;
    mocc->_size = 0;
    mocc->_capacity = initial_capacity;

    err = pthread_mutex_init(&mocc->_mutex, NULL);

    if (MOCC_OK != err)
    {
        free(buffer);
        free(mocc);
    }

    DEFFENSIVE_ASSERT(MOCC_OK == err, MOCC_ERROR_INTERNAL);

    *object = mocc;

    return MOCC_OK;
}

mocc_error mocc_dtor(mocc_object* me)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INVALID_ARGUMENT);

    DEFFENSIVE_ASSERT(0 == pthread_mutex_destroy(&me->_mutex), MOCC_ERROR_INTERNAL);

    if (me->_front)
    {
        free(me->_front);
    }

    free(me);

    me = NULL;

    return MOCC_OK;
}

mocc_error mocc_size(mocc_object* me, size_t* size)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INVALID_ARGUMENT);
    *size = me->_size;

    return MOCC_OK;
}

mocc_error mocc_capacity(mocc_object* me, size_t* capacity)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INVALID_ARGUMENT);
    *capacity = me->_capacity;

    return MOCC_OK;
}

mocc_error mocc_reserve(mocc_object* me, size_t capacity)
{
    void* new_front = NULL;

    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INVALID_ARGUMENT);
    DEFFENSIVE_ASSERT(capacity > 0, MOCC_ERROR_INVALID_ARGUMENT);

    new_front = realloc(me->_front, capacity * me->_element_size);
    DEFFENSIVE_ASSERT(new_front, MOCC_ERROR_OUT_OF_MEMORY);

    me->_front = new_front;
    me->_tail = (char*)me->_front + capacity * me->_element_size;
    me->_capacity = capacity;

    if (me->_size == 0)
    {
        me->_back = (char*)me->_front - me->_element_size;
    }
    else
    {
        me->_back = (char*)me->_front + (me->_size - 1) * me->_element_size;
    }

    return MOCC_OK;
}

mocc_error mocc_shrink_to_fit(mocc_object* me)
{
    void* new_front = NULL;

    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INVALID_ARGUMENT);

    if (me->_size == 0)
    {
        free(me->_front);
        me->_front = NULL;
        me->_back = NULL;
        me->_tail = NULL;
        me->_capacity = 0;
        return MOCC_OK;
    }

    new_front = realloc(me->_front, me->_size * me->_element_size);
    DEFFENSIVE_ASSERT(new_front, MOCC_ERROR_OUT_OF_MEMORY);

    me->_front = new_front;
    me->_tail = (char*)me->_front + me->_size * me->_element_size;
    me->_back = (char*)me->_front + (me->_size - 1) * me->_element_size;
    me->_capacity = me->_size;

    return MOCC_OK;
}

mocc_error mocc_push_back(mocc_object* me, const void* element)
{
    mocc_error err;

    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INVALID_ARGUMENT);
    DEFFENSIVE_ASSERT(element, MOCC_ERROR_INVALID_ARGUMENT);

    DEFFENSIVE_ASSERT(me->_size <= me->_capacity, MOCC_ERROR_INTERNAL);

    if (me->_size == me->_capacity)
    {
        DEFFENSIVE_ASSERT(me->_size == 0 || (char*)me->_back + me->_element_size == me->_tail,
                          MOCC_ERROR_INTERNAL);

        err = mocc_reserve(me, me->_capacity == 0 ? 1 : me->_capacity * 2);
        DEFFENSIVE_ASSERT(me->_size < me->_capacity, MOCC_ERROR_INTERNAL);

        if (err != MOCC_OK)
            return err;
    }

    me->_back = (char*)me->_back + me->_element_size;

    memcpy(me->_back, element, me->_element_size);
    me->_size++;

    return MOCC_OK;
}

mocc_error mocc_pop_back(mocc_object* me)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INVALID_ARGUMENT);
    DEFFENSIVE_ASSERT(me->_size > 0, MOCC_ERROR_EMPTY);

    me->_back = (char*)me->_back - me->_element_size;
    me->_size--;

    return MOCC_OK;
}

mocc_error mocc_erase(mocc_object* me, size_t index)
{
    void* target = NULL;
    void* next = NULL;
    size_t bytes_to_move;

    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INVALID_ARGUMENT);
    DEFFENSIVE_ASSERT(index < me->_size, MOCC_ERROR_INVALID_INDEX);

    DEFFENSIVE_ASSERT(me->_size > 0, MOCC_ERROR_EMPTY);

    target = (char*)me->_front + index * me->_element_size;

    if (index < me->_size - 1)
    {
        next = (char*)target + me->_element_size;
        bytes_to_move = ((char*)me->_back - (char*)next) + me->_element_size;

        memmove(target, next, bytes_to_move);
    }
    else
    {
        me->_back = (char*)me->_back - me->_element_size;
    }

    me->_size--;

    return MOCC_OK;
}

mocc_error mocc_at(mocc_object* me, size_t index, void** element)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INVALID_ARGUMENT);
    DEFFENSIVE_ASSERT(index < me->_size, MOCC_ERROR_INVALID_INDEX);

    DEFFENSIVE_ASSERT(me->_size > 0, MOCC_ERROR_EMPTY);

    *element = (char*)me->_front + index * me->_element_size;

    return MOCC_OK;
}

mocc_error mocc_front(mocc_object* me, void** element)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INVALID_ARGUMENT);
    DEFFENSIVE_ASSERT(element, MOCC_ERROR_INVALID_ARGUMENT);
    DEFFENSIVE_ASSERT(me->_size > 0, MOCC_ERROR_EMPTY);

    *element = me->_front;

    return MOCC_OK;
}

mocc_error mocc_back(mocc_object* me, void** element)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INVALID_ARGUMENT);
    DEFFENSIVE_ASSERT(element, MOCC_ERROR_INVALID_ARGUMENT);
    DEFFENSIVE_ASSERT(me->_size > 0, MOCC_ERROR_EMPTY);

    *element = me->_back;

    return MOCC_OK;
}

mocc_error mocc_clear(mocc_object* me)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INVALID_ARGUMENT);

    me->_size = 0;
    if (me->_front)
    {
        me->_back = (char*)me->_front - me->_element_size;
    }
    else
    {
        me->_back = NULL;
    }

    return MOCC_OK;
}

mocc_error mocc_safe_push_back(mocc_object* me, const void* element)
{
    mocc_error err;

    __mocc_lock(me);
    err = mocc_push_back(me, element);
    __mocc_unlock(me);

    return err;
}

mocc_error mocc_safe_pop_back(mocc_object* me)
{
    mocc_error err;

    __mocc_lock(me);
    err = mocc_pop_back(me);
    __mocc_unlock(me);

    return err;
}

mocc_error mocc_safe_erase(mocc_object* me, size_t index)
{
    mocc_error err;

    __mocc_lock(me);
    err = mocc_erase(me, index);
    __mocc_unlock(me);

    return err;
}

mocc_error mocc_safe_at(mocc_object* me, size_t index, void** element)
{

    mocc_error err;

    __mocc_lock(me);
    err = mocc_at(me, index, element);
    __mocc_unlock(me);

    return err;
}

mocc_error mocc_safe_front(mocc_object* me, void** element)
{
    mocc_error err;

    __mocc_lock(me);
    err = mocc_front(me, element);
    __mocc_unlock(me);

    return err;
}

mocc_error mocc_safe_back(mocc_object* me, void** element)
{
    mocc_error err;

    __mocc_lock(me);
    err = mocc_back(me, element);
    __mocc_unlock(me);

    return err;
}

mocc_error mocc_safe_clear(mocc_object* me)
{
    mocc_error err;

    __mocc_lock(me);
    err = mocc_clear(me);
    __mocc_unlock(me);

    return err;
}

mocc_error mocc_safe_size(mocc_object* me, size_t* size)
{
    mocc_error err;

    __mocc_lock(me);
    err = mocc_size(me, size);
    __mocc_unlock(me);

    return err;
}

mocc_error mocc_safe_capacity(mocc_object* me, size_t* capacity)
{
    mocc_error err;

    __mocc_lock(me);
    err = mocc_capacity(me, capacity);
    __mocc_unlock(me);

    return err;
}
