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

/** An enum to represent status codes for module */
typedef enum
{
    HANDLE_StatusOk = 0,     /**< OK */
    HANDLE_StatusNullPtr,    /**< Null pointer was passed to API function */
    HANDLE_StatusMemError,   /**< Memory allocation errror */
    HANDLE_StatusWrongHandle /**< Wrong handle */
} HANDLE_Status;

/** Handle instance */
typedef i64 HANDLE_Id;

/** Memory allocation function type */
typedef void* (*HANDLE_MemAllocator)(size bytes);

/* -------------------------------------------------------------------------- */
/* ------------------------------- API functions ---------------------------- */
/* -------------------------------------------------------------------------- */

void HANDLE_Init(void);

HANDLE_Status HANDLE_AllocWithAllocator(
        HANDLE_Id* handle,
        size bytes,
        HANDLE_MemAllocator allocator);

static inline HANDLE_Status HANDLE_Alloc(HANDLE_Id* handle, size bytes)
{
    return HANDLE_AllocWithAllocator(handle, bytes, malloc);
}

HANDLE_Status HANDLE_Dealloc(HANDLE_Id* handle);

size HANDLE_CountFree(void);

#if defined(__cplusplus)
}
#endif

#endif // HANDLE_H
