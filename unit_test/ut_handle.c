#include "ut.h"
#include "unity.h"
#include "handle.h"

/* -------------------------------------------------------------------------- */
/* ------------------------------ Private macros ---------------------------- */
/* -------------------------------------------------------------------------- */

#define TEST_ASSERT_HANDLE_EQ(EXP, ACT) TEST_ASSERT_EQUAL_UINT64((EXP), (ACT))

/* -------------------------------------------------------------------------- */
/* ---------------------------------- Tests --------------------------------- */
/* -------------------------------------------------------------------------- */

void UT_HANDLE_Alloc_AfterFirstAllocationZeroHandleIsReturned(void)
{
    HANDLE_Id handle;
    HANDLE_Status status = HANDLE_Alloc(&handle, sizeof(u32));

    TEST_ASSERT_STATUS_EQ(HANDLE_StatusOk, status);
    TEST_ASSERT_HANDLE_EQ(0, handle);
}
