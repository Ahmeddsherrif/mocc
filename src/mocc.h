#ifndef MOCC_H
#define MOCC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>

    /*---------------------------------------------------------------------------
     * Types
     *---------------------------------------------------------------------------*/

    typedef struct mocc_object mocc_object;

    typedef enum
    {
        MOCC_OK = 0,
        MOCC_ERROR_OUT_OF_MEMORY,
        MOCC_ERROR_INVALID_ARGUMENT,
        MOCC_ERROR_INVALID_INDEX,
        MOCC_ERROR_EMPTY,
        MOCC_ERROR_INTERNAL
    } mocc_error;

    /*---------------------------------------------------------------------------
     * Lifecycle
     *---------------------------------------------------------------------------*/

    /**
     * Create a container instance.
     *
     * @in  element_size     Size of a single element in bytes.
     * @in  initial_capacity Initial capacity of the container.
     * @out object           Created object.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_ctor(size_t element_size, size_t initial_capacity, mocc_object** object);

    /**
     * Destroy a container instance.
     *
     * @inout me Object to destroy.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_dtor(mocc_object* me);

    /*---------------------------------------------------------------------------
     * Capacity
     *---------------------------------------------------------------------------*/

    /**
     * Get current number of elements.
     *
     * @in  me   Container object.
     * @out size Current element count.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_size(mocc_object* me, size_t* size);

    /**
     * Get current capacity.
     *
     * @in  me        Container object.
     * @out capacity  Current capacity.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_capacity(mocc_object* me, size_t* capacity);

    /**
     * @brief
     *
     */
    /**
     * @in me Container object.
     * @in capacity Requested capacity.
     * @return MOCC_OK on success.
     */
    mocc_error mocc_reserve(mocc_object* me, size_t capacity);

    /**
     * Reduce capacity to match size.
     *
     * @in me Container object.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_shrink_to_fit(mocc_object* me);

    /*---------------------------------------------------------------------------
     * Element Operations
     *---------------------------------------------------------------------------*/

    /**
     * Append an element.
     *
     * @in me      Container object.
     * @in element Element to append.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_push_back(mocc_object* me, const void* element);

    /**
     * Remove last element.
     *
     * @in me Container object.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_pop_back(mocc_object* me);

    /**
     * Erase element at index.
     *
     * @in me    Container object.
     * @in index Element index.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_erase(mocc_object* me, size_t index);

    /**
     * Get element at index.
     *
     * @in  me      Container object.
     * @in  index   Element index.
     * @out element Pointer to element storage.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_at(mocc_object* me, size_t index, void** element);

    /**
     * Get first element.
     *
     * @in  me      Container object.
     * @out element Pointer to first element.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_front(mocc_object* me, void** element);

    /**
     * Get last element.
     *
     * @in  me      Container object.
     * @out element Pointer to last element.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_back(mocc_object* me, void** element);

    /**
     * Remove all elements.
     *
     * @in me Container object.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_clear(mocc_object* me);

    /*---------------------------------------------------------------------------
     * Concurrent Element Operations
     *---------------------------------------------------------------------------*/

    /**
     * Thread-safe append.
     *
     * @in me      Container object.
     * @in element Element to append.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_safe_push_back(mocc_object* me, const void* element);

    /**
     * Thread-safe remove last element.
     *
     * @in me Container object.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_safe_pop_back(mocc_object* me);

    /**
     * Thread-safe erase element at index.
     *
     * @in me    Container object.
     * @in index Element index.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_safe_erase(mocc_object* me, size_t index);

    /**
     * Thread-safe access by index.
     *
     * @in  me      Container object.
     * @in  index   Element index.
     * @out element Pointer to element storage.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_safe_at(mocc_object* me, size_t index, void** element);

    /**
     * Thread-safe access to first element.
     *
     * @in  me      Container object.
     * @out element Pointer to first element.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_safe_front(mocc_object* me, void** element);

    /**
     * Thread-safe access to last element.
     *
     * @in  me      Container object.
     * @out element Pointer to last element.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_safe_back(mocc_object* me, void** element);

    /**
     * Thread-safe clear.
     *
     * @in me Container object.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_safe_clear(mocc_object* me);

    /**
     * Thread-safe size query.
     *
     * @in  me   Container object.
     * @out size Current element count.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_safe_size(mocc_object* me, size_t* size);

    /**
     * Thread-safe capacity query.
     *
     * @in  me       Container object.
     * @out capacity Current capacity.
     *
     * @return MOCC_OK on success.
     */
    mocc_error mocc_safe_capacity(mocc_object* me, size_t* capacity);

#ifdef __cplusplus
}
#endif

#endif /* MOCC_H */