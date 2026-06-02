#include "mocc.h"
#include <assert.h>

struct mocc_object
{
    void* data;
    size_t size;
    size_t capacity;
    size_t element_size;
};

const char* mocc_error_string(mocc_error error)
{

    switch (error)
    {
        case MOCC_ERROR_OUT_OF_MEMORY:
            return "[MOCC] Out of memory";

        case MOCC_ERROR_INVALID_ARGUMENT:
            return "[MOCC] Invalid argument";

        case MOCC_ERROR_INVALID_INDEX:
            return "[MOCC] Invalid index";

        case MOCC_ERROR_EMPTY:
            return "[MOCC] Container is empty";

        case MOCC_ERROR_INTERNAL:
            return "[MOCC] Internal error";

        case MOCC_OK:
            return "[MOCC] No error";
    }

    return "[MOCC] Unknown error";
}

static mocc_error __mocc_compact(mocc_object* me)
{

    assert(me);
    return MOCC_OK;
}

static mocc_error __mocc_collect_garbage(mocc_object* me)
{

    assert(me);
    return MOCC_OK;
}

static mocc_error __mocc_pool_initialize(mocc_object* me, size_t block_size, size_t block_count)
{

    assert(me);
    (void)block_size;
    (void)block_count;
    return MOCC_OK;
}

static mocc_error __mocc_pool_release(mocc_object* me)
{

    assert(me);
    return MOCC_OK;
}

static mocc_error __mocc_lock(mocc_object* me)
{

    assert(me);
    return MOCC_OK;
}

static mocc_error __mocc_unlock(mocc_object* me)
{

    assert(me);
    return MOCC_OK;
}

mocc_error mocc_ctor(size_t element_size, size_t initial_capacity, mocc_object** object)
{
    (void)element_size;
    (void)initial_capacity;
    (void)object;

    return MOCC_OK;
}

mocc_error mocc_dtor(mocc_object* me)
{
    assert(me);

    return MOCC_OK;
}

mocc_error mocc_size(mocc_object* me, size_t* size)
{
    assert(me);
    (void)size;

    return MOCC_OK;
}

mocc_error mocc_capacity(mocc_object* me, size_t* capacity)
{
    assert(me);
    (void)capacity;

    return MOCC_OK;
}

mocc_error mocc_reserve(mocc_object* me, size_t capacity)
{
    assert(me);
    (void)capacity;

    return MOCC_OK;
}

mocc_error mocc_resize(mocc_object* me, size_t size)
{
    assert(me);
    (void)size;

    return MOCC_OK;
}

mocc_error mocc_shrink_to_fit(mocc_object* me)
{
    assert(me);

    return MOCC_OK;
}

mocc_error mocc_push_back(mocc_object* me, const void* element)
{
    assert(me);
    (void)element;

    return MOCC_OK;
}

mocc_error mocc_pop_back(mocc_object* me)
{
    assert(me);

    return MOCC_OK;
}

mocc_error mocc_insert(mocc_object* me, size_t index, const void* element)
{
    assert(me);
    (void)index;
    (void)element;

    return MOCC_OK;
}

mocc_error mocc_erase(mocc_object* me, size_t index)
{
    assert(me);
    (void)index;

    return MOCC_OK;
}

mocc_error mocc_at(mocc_object* me, size_t index, void** element)
{
    assert(me);
    (void)index;
    (void)element;

    return MOCC_OK;
}

mocc_error mocc_front(mocc_object* me, void** element)
{
    assert(me);
    (void)element;

    return MOCC_OK;
}

mocc_error mocc_back(mocc_object* me, void** element)
{
    assert(me);
    (void)element;

    return MOCC_OK;
}

mocc_error mocc_clear(mocc_object* me)
{
    assert(me);

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
