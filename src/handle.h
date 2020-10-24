#ifndef HANDLE_H
#define HANDLE_H

#include "common.h"

#if defined(__cplusplus)
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/* ---------------------------------- Macros -------------------------------- */
/* -------------------------------------------------------------------------- */

/* Default size for look-up table */
#define HANDLE_LUT_DEFAULT_SIZE 10

/* -------------------------------------------------------------------------- */
/* -------------------------------- Data types ------------------------------ */
/* -------------------------------------------------------------------------- */

/** An enum to represent status codes for module */
typedef enum
{
    HANDLE_StatusOk = 0, /**< OK */
    HANDLE_StatusNullPtr /** < Null pointer was passed to API function */
} HANDLE_Status;

/** Handle instance */
typedef u64 HANDLE_Id;

/* -------------------------------------------------------------------------- */
/* ------------------------------- API functions ---------------------------- */
/* -------------------------------------------------------------------------- */

void HANDLE_Init(void);
HANDLE_Status HANDLE_Alloc(HANDLE_Id* handle, size bytes);

#if defined(__cplusplus)
}
#endif

#endif // HANDLE_H
