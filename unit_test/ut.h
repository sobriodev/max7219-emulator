#ifndef UT_H
#define UT_H

#include "unity.h"

#if defined(__cplusplus)
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/* ---------------------------------- Macros -------------------------------- */
/* -------------------------------------------------------------------------- */

#define TEST_ASSERT_STATUS_EQ(EXP, ACT) TEST_ASSERT_EQUAL_INT32((EXP), (ACT))

/* -------------------------------------------------------------------------- */
/* ---------------------------- Test declarations --------------------------- */
/* ------------ Naming convention: UT_{MODULE}_{FUNCTION}_{CONDITION} ------- */
/* -------------------------------------------------------------------------- */

/* HANDLE module */
void UT_HANDLE_Alloc_AfterFirstAllocationZeroHandleIsReturned(void);
void UT_HANDLE_Alloc_HandlesAreReturnedInAscendingOrder(void);
void UT_HANDLE_Alloc_NothingIsDoneWhenNullPointerIsPassed(void);
void UT_HANDLE_AllocWithAllocator_AllocErrReturnedWhenAllocFnFails(void);
void UT_HANDLE_Dealloc_HandleCanBeUsedSecondTimeAfterItIsFreed(void);

#if defined(__cplusplus)
}
#endif

#endif // UT_H
