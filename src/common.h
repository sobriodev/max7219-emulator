#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* ------------------------------------------------------------------------- */
/* -------------------------------- Macros --------------------------------- */
/* ------------------------------------------------------------------------- */

/* Check for NULL generic code */
#define COMMON_NULLPTR_GUARD(PTR, STATUS_CODE) \
    {if ((PTR) == NULL) return (STATUS_CODE);}

/* ------------------------------------------------------------------------- */
/* ------------------------------ Data types ------------------------------- */
/* ------------------------------------------------------------------------- */

/* Unsigned types */
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

/* Signed types */
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef size_t size;

#if defined(__cplusplus)
}
#endif

#endif // COMMON_H
