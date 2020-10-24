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
    HANDLE_Init();

    HANDLE_Id handle;
    HANDLE_Status status = HANDLE_Alloc(&handle, sizeof(u32));

    TEST_ASSERT_STATUS_EQ(HANDLE_StatusOk, status);
    TEST_ASSERT_HANDLE_EQ(0, handle);
    /* TODO Call dealloc function after test */
}

void UT_HANDLE_Alloc_HandlesAreReturnedInAscendingOrder(void)
{
    HANDLE_Init();

    HANDLE_Id handle;
    HANDLE_Status status;
    for (size i = 0; i < HANDLE_LUT_DEFAULT_SIZE; ++i) {
        status = HANDLE_Alloc(&handle, sizeof(u32));

        /* Both status and id should be correct */
        TEST_ASSERT_STATUS_EQ(HANDLE_StatusOk, status);
        TEST_ASSERT_HANDLE_EQ(i, handle);
    }
    /* TODO Call dealloc function after test */
}

void UT_HANDLE_Alloc_NothingIsDoneWhenNullPointerIsPassed(void)
{
    HANDLE_Status status;
    status = HANDLE_Alloc(NULL, sizeof(u8));

    TEST_ASSERT_STATUS_EQ(HANDLE_StatusNullPtr, status);
}
