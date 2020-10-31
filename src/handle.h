#ifndef HANDLE_H
#define HANDLE_H

#include "common.h"

#include <stdlib.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/* ---------------------------------- Macros -------------------------------- */
/* -------------------------------------------------------------------------- */

/* Default size for look-up table */
#define HANDLE_LUT_DEFAULT_SIZE 10

/* Macro to indicate invalid handle */
#define HANDLE_INVALID -1

/* -------------------------------------------------------------------------- */
/* -------------------------------- Data types ------------------------------ */
/* -------------------------------------------------------------------------- */

/**
 * @brief An enum to represent status codes for module
 */
typedef enum
{
    HANDLE_StatusOk = 0,     /**< OK */
    HANDLE_StatusNullPtr,    /**< Null pointer was passed to API function */
    HANDLE_StatusMemError,   /**< Memory allocation errror */
    HANDLE_StatusWrongHandle /**< Wrong handle */
} HANDLE_Status;

/**
 * @brief Handle instance type
 */
typedef i64 HANDLE_Id;

/**
 * @brief Memory allocation function type
 */
typedef void* (*HANDLE_MemAllocator)(size bytes);

/* -------------------------------------------------------------------------- */
/* ------------------------------- API functions ---------------------------- */
/* -------------------------------------------------------------------------- */

/**
 * @brief Initialize handle module.
 *
 * This function sets up look-up table which is a core part of the module.
 * It should be called only once during application startup.
 *
 * @note You must explicitely call this function before use of any API
 * operation. Otherwise the handle mapping will not behave correctly and
 * unexpected results are guaranted (including memory leaks).
 */
void HANDLE_Init(void);

/**
 * @brief Wrap piece of memory with an unique handle.
 *
 * The function allocates requested number of bytes and connects it with
 * an unique handle which can then be used to manipulate memory contents
 * indirectly. This practice can avoid common problems with bare memory access,
 * e.g. dangling or NULL pointers.
 *
 * @param handle    The buffer in which allocated handle is stored
 * @param bytes     Memory bytes to be allocated
 * @param allocator The function which allocates memory (can be malloc if more
 * sophisticated allocator is not needed)
 *
 * @return Instance of HANDLE_Status. The function possible return values are:
 * - HANDLE_StatusNullPtr when null pointer was passed to function
 * - HANDLE_StatusMemError when there was a memory  allocation error
 * - HANDLE_StatusOk after success
 *
 * The handle has to be deallocated manually using provided API functions.
 *
 * @see HANDLE_MemAllocator to get memory allocator correct type
 */
HANDLE_Status HANDLE_AllocWithAllocator(
        HANDLE_Id* handle,
        size bytes,
        HANDLE_MemAllocator allocator);

/**
 * @brief Allocate handle using default memory allocator.
 *
 * This function is HANDLE_AllocWithAllocator wrapper which uses a default
 * memory allocation function.
 *
 * @param handle The buffer in which allocated handle is stored
 * @param bytes  Memory bytes to be allocated
 *
 * @return The function returns the same status codes as
 * HANDLE_AllocWithAllocator. See HANDLE_AllocWithAllocator for more information
 */
static inline HANDLE_Status HANDLE_Alloc(HANDLE_Id* handle, size bytes)
{
    return HANDLE_AllocWithAllocator(handle, bytes, malloc);
}

/**
 *
 * @brief Deallocate handle.
 *
 * The function deallocates handle and frees connected memory block.
 * By deaallocating the same handle can be used multiple times.
 *
 * @param handle Pointer to allocated handle. It will be invalidated eventually
 * @return Instance of HANDLE_Status. Possible return codes are:
 * - HANDLE_StatusNullPtr when NULL pointer was passed
 * - HANDLE_StatusWrongHandle when handle is not valid (e.g. not exists or
 * it is freed actually)
 * - HANDLE_StatusOk after success
 */
HANDLE_Status HANDLE_Dealloc(HANDLE_Id* handle);

/**
 * @brief Count free memory handles.
 *
 * This function returns the number of handles which are not used and thus
 * can point to a newly allocated memory.
 *
 * @return The number of free handle instances
 */
size HANDLE_CountFree(void);

/**
 * @brief Count all number of handles available across the system.
 *
 * The function returns the number of all memory handles (both allocated and
 * free) and can be used to determine how many LUT entries are used at the time.
 *
 * @return The number of all handles used
 */
size HANDLE_CountAll(void);

/**
 * @brief Deallocate all handles.
 *
 * The function deallocates all handles and frees memory blocks connected
 * to them. This is a convenient way to free all resources when application need
 * to be terminated.
 */
void HANDLE_DeallocAll(void);

#if defined(__cplusplus)
}
#endif

#endif // HANDLE_H
