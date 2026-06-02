#include "mocc.h"
#include "logger.h"

#include <stdlib.h>
#include <string.h>

#define DEFFENSIVE_ASSERT(expr, error)                                                             \
    do                                                                                             \
    {                                                                                              \
        if (!(expr))                                                                               \
        {                                                                                          \
            LOG_ERROR("Assertion failed: (" #expr "): " #error);                                   \
            return error;                                                                          \
        }                                                                                          \
    } while (0)

struct mocc_object
{
    void* _back;
    void* _front;
    void* _tail;
    size_t _size;
    size_t _capacity;
    size_t _element_size;
};

static mocc_error __mocc_lock(mocc_object* me)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INTERNAL);
    return MOCC_OK;
}

static mocc_error __mocc_unlock(mocc_object* me)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INTERNAL);
    return MOCC_OK;
}

mocc_error mocc_ctor(size_t element_size, mocc_object** object)
{
    DEFFENSIVE_ASSERT(element_size > 0, MOCC_ERROR_INVALID_ARGUMENT);
    DEFFENSIVE_ASSERT(object, MOCC_ERROR_INVALID_ARGUMENT);

    mocc_object* mocc = calloc(1, sizeof(element_size));
    DEFFENSIVE_ASSERT(mocc, MOCC_ERROR_OUT_OF_MEMORY);

    mocc->_element_size = element_size;

    *object = mocc;

    return MOCC_OK;
}

mocc_error mocc_dtor(mocc_object* me)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INTERNAL);

    free(me);

    me = NULL;

    return MOCC_OK;
}

mocc_error mocc_size(mocc_object* me, size_t* size)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INTERNAL);
    *size = me->_size;

    return MOCC_OK;
}

mocc_error mocc_capacity(mocc_object* me, size_t* capacity)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INTERNAL);
    *capacity = me->_capacity;

    return MOCC_OK;
}

mocc_error mocc_reserve(mocc_object* me, size_t capacity)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INTERNAL);
    (void)capacity;

    return MOCC_OK;
}

mocc_error mocc_shrink_to_fit(mocc_object* me)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INTERNAL);

    return MOCC_OK;
}

mocc_error mocc_push_back(mocc_object* me, const void* element)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INTERNAL);

    if (me->_size >= me->_capacity)
    {
        DEFFENSIVE_ASSERT(me->_back == me->_tail, MOCC_ERROR_INTERNAL);

        mocc_error err = mocc_reserve(me, me->_capacity == 0 ? 1 : me->_capacity * 2);
        if (err != MOCC_OK)
            return err;
    }

    DEFFENSIVE_ASSERT(me->_size < me->_capacity, MOCC_ERROR_INTERNAL);
    DEFFENSIVE_ASSERT(me->_back != me->_tail, MOCC_ERROR_INTERNAL);

    me->_back += me->_element_size;
    memcpy(me->_back, element, me->_element_size);

    me->_size++;

    return MOCC_OK;
}

mocc_error mocc_pop_back(mocc_object* me)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INTERNAL);
    DEFFENSIVE_ASSERT(me->_size > 0, MOCC_ERROR_EMPTY);

    me->_back -= me->_element_size;
    me->_size--;

    return MOCC_OK;
}

mocc_error mocc_insert(mocc_object* me, size_t index, const void* element)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INTERNAL);
    (void)index;
    (void)element;

    return MOCC_OK;
}

mocc_error mocc_erase(mocc_object* me, size_t index)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INTERNAL);
    (void)index;

    return MOCC_OK;
}

mocc_error mocc_at(mocc_object* me, size_t index, void** element)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INTERNAL);
    (void)index;
    (void)element;

    return MOCC_OK;
}

mocc_error mocc_front(mocc_object* me, void** element)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INTERNAL);

    *element = me->_front;

    return MOCC_OK;
}

mocc_error mocc_back(mocc_object* me, void** element)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INTERNAL);

    *element = me->_back;

    return MOCC_OK;
}

mocc_error mocc_clear(mocc_object* me)
{
    DEFFENSIVE_ASSERT(me, MOCC_ERROR_INTERNAL);

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

mocc_error mocc_safe_insert(mocc_object* me, size_t index, const void* element)
{
    mocc_error err;

    __mocc_lock(me);
    err = mocc_insert(me, index, element);
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
