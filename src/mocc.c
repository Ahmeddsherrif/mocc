#include "mocc.h"

mocc_error __mocc_compact(mocc_object* me)
{

    (void)me;
    return MOCC_OK;
}

mocc_error __mocc_collect_garbage(mocc_object* me)
{

    (void)me;
    return MOCC_OK;
}

mocc_error __mocc_pool_initialize(mocc_object* me, size_t block_size, size_t block_count)
{

    (void)me;
    (void)block_size;
    (void)block_count;
    return MOCC_OK;
}

mocc_error __mocc_pool_release(mocc_object* me)
{

    (void)me;
    return MOCC_OK;
}

mocc_error __mocc_lock(mocc_object* me)
{

    (void)me;
    return MOCC_OK;
}

mocc_error __mocc_unlock(mocc_object* me)
{

    (void)me;
    return MOCC_OK;
}

struct mocc_object
{
    size_t size;
    size_t capacity;
};

mocc_error mocc_create(size_t element_size, size_t initial_capacity, mocc_object** object)
{
    (void)element_size;
    (void)initial_capacity;
    (void)object;

    return MOCC_OK;
}

mocc_error mocc_destroy(mocc_object* me)
{
    (void)me;

    return MOCC_OK;
}

mocc_error mocc_size(mocc_object* me, size_t* size)
{
    (void)me;
    (void)size;

    return MOCC_OK;
}

mocc_error mocc_capacity(mocc_object* me, size_t* capacity)
{
    (void)me;
    (void)capacity;

    return MOCC_OK;
}

mocc_error mocc_reserve(mocc_object* me, size_t capacity)
{
    (void)me;
    (void)capacity;

    return MOCC_OK;
}

mocc_error mocc_resize(mocc_object* me, size_t size)
{
    (void)me;
    (void)size;

    return MOCC_OK;
}

mocc_error mocc_shrink_to_fit(mocc_object* me)
{
    (void)me;

    return MOCC_OK;
}

mocc_error mocc_push_back(mocc_object* me, const void* element)
{
    (void)me;
    (void)element;

    return MOCC_OK;
}

mocc_error mocc_pop_back(mocc_object* me)
{
    (void)me;

    return MOCC_OK;
}

mocc_error mocc_insert(mocc_object* me, size_t index, const void* element)
{
    (void)me;
    (void)index;
    (void)element;

    return MOCC_OK;
}

mocc_error mocc_erase(mocc_object* me, size_t index)
{
    (void)me;
    (void)index;

    return MOCC_OK;
}

mocc_error mocc_at(mocc_object* me, size_t index, void** element)
{
    (void)me;
    (void)index;
    (void)element;

    return MOCC_OK;
}

mocc_error mocc_front(mocc_object* me, void** element)
{
    (void)me;
    (void)element;

    return MOCC_OK;
}

mocc_error mocc_back(mocc_object* me, void** element)
{
    (void)me;
    (void)element;

    return MOCC_OK;
}

mocc_error mocc_clear(mocc_object* me)
{
    (void)me;

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
