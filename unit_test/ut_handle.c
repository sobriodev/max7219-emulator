#include "ut.h"
#include "unity.h"
#include "handle.h"

/* -------------------------------------------------------------------------- */
/* ------------------------------ Private macros ---------------------------- */
/* -------------------------------------------------------------------------- */

#define TEST_ASSERT_HANDLE_EQ(EXP, ACT) TEST_ASSERT_EQUAL_INT64((EXP), (ACT))
#define TEST_ASSERT_SIZE_EQ(EXP, ACT)   TEST_ASSERT_EQUAL_size_t((EXP), (ACT))

/* -------------------------------------------------------------------------- */
/* ---------------------------- Private functions --------------------------- */
/* -------------------------------------------------------------------------- */

/* The function stub to return NULL pointer for testing purposes */
static void* FailureAllocator(size bytes)
{
    (void)bytes;
    return NULL;
}

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

    /* Dealloc handle to prevent memory leaks */
    HANDLE_Dealloc(&handle);
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

    /* Dealloc handles to prevent memory leaks */
    for (size i = 0; i < HANDLE_LUT_DEFAULT_SIZE; ++i) {
        handle = i;
        HANDLE_Dealloc(&handle);
    }
}

void UT_HANDLE_Alloc_NothingIsDoneWhenNullPointerIsPassed(void)
{
    HANDLE_Status status;
    status = HANDLE_Alloc(NULL, sizeof(u8));

    TEST_ASSERT_STATUS_EQ(HANDLE_StatusNullPtr, status);
}

void UT_HANDLE_AllocWithAllocator_AllocErrReturnedWhenAllocFnFails(void)
{
    HANDLE_Init();

    HANDLE_Id handle = HANDLE_INVALID;

    HANDLE_Status status;
    status = HANDLE_AllocWithAllocator(&handle, sizeof(u64), FailureAllocator);

    TEST_ASSERT_HANDLE_EQ(HANDLE_INVALID, handle);
    TEST_ASSERT_STATUS_EQ(HANDLE_StatusMemError, status);
}

void UT_HANDLE_Dealloc_HandleCanBeUsedSecondTimeAfterItIsFreed(void)
{
    HANDLE_Init();

    HANDLE_Id handle;
    HANDLE_Alloc(&handle, sizeof(i32));

    /* Dealloc handle and alloc second time */
    HANDLE_Dealloc(&handle);
    HANDLE_Status status = HANDLE_Alloc(&handle, 10 * sizeof(u8));

    /* Eventually check if the handle is reused after freeing */
    TEST_ASSERT_HANDLE_EQ(0, handle);
    TEST_ASSERT_STATUS_EQ(HANDLE_StatusOk, status);

    HANDLE_Dealloc(&handle);
}

void UT_HANDLE_Dealloc_HandleIsInvalidatedAfterItIsFreed(void)
{
    HANDLE_Init();

    HANDLE_Id handle = HANDLE_INVALID;
    HANDLE_Status allocStatus = HANDLE_Alloc(&handle, sizeof(u32));
    HANDLE_Status deallocStatus = HANDLE_Dealloc(&handle);

    TEST_ASSERT_STATUS_EQ(HANDLE_StatusOk, allocStatus);
    TEST_ASSERT_STATUS_EQ(HANDLE_StatusOk, deallocStatus);
    TEST_ASSERT_HANDLE_EQ(HANDLE_INVALID, handle);
}

void UT_HANDLE_Dealloc_NothingIsDoneWhenNullPointerIsPassed(void)
{
    HANDLE_Status status;
    status = HANDLE_Dealloc(NULL);

    TEST_ASSERT_STATUS_EQ(HANDLE_StatusNullPtr, status);
}

void UT_HANDLE_Dealloc_ErrStatusIsReturnedWhenInvalidHandleIsPassed(void)
{
    HANDLE_Init();

    HANDLE_Id someHugeHandle = 0xFFFF;
    HANDLE_Status status;
    status = HANDLE_Dealloc(&someHugeHandle);

    TEST_ASSERT_STATUS_EQ(HANDLE_StatusWrongHandle, status);
}

void UT_HANDLE_CountFree_ByDefaultReturnedNumberEqualsDefaultSize(void)
{
    HANDLE_Init();

    size numberOfFreeHandles = HANDLE_CountFree();
    TEST_ASSERT_SIZE_EQ(HANDLE_LUT_DEFAULT_SIZE, numberOfFreeHandles);
}

void UT_HANDLE_CountFree_ReturnedNumberIsCorrectAfterAlloc(void)
{
    HANDLE_Init();

    HANDLE_Id firstHandle;
    HANDLE_Id secondHandle;
    HANDLE_Alloc(&firstHandle, sizeof(u32));
    HANDLE_Alloc(&secondHandle, 100);

    size freeHandles = HANDLE_CountFree();
    TEST_ASSERT_SIZE_EQ(HANDLE_LUT_DEFAULT_SIZE - 2, freeHandles);

    /* Dealloc handles to prevent memory leaks */
    HANDLE_Dealloc(&firstHandle);
    HANDLE_Dealloc(&secondHandle);
}

void UT_HANDLE_CountFree_ReturnedNumberIsCorrectAfterDealloc(void)
{
    HANDLE_Init();

    HANDLE_Id handle;
    HANDLE_Alloc(&handle, 1000);
    HANDLE_Dealloc(&handle);

    size freeHandles = HANDLE_CountFree();
    TEST_ASSERT_SIZE_EQ(HANDLE_LUT_DEFAULT_SIZE, freeHandles);
}

void UT_HANDLE_CountAll_ByDefaultCorrectNumberIsReturned(void)
{
    HANDLE_Init();

    size allHandles = HANDLE_CountAll();
    TEST_ASSERT_SIZE_EQ(HANDLE_LUT_DEFAULT_SIZE, allHandles);
}

void UT_HANDLE_DeallocAll_HandlesAreFreedAfterOperation(void)
{
    HANDLE_Init();

    HANDLE_Id handle;
    HANDLE_Alloc(&handle, 100);
    HANDLE_Alloc(&handle, 200);
    HANDLE_Alloc(&handle, 100);
    HANDLE_Alloc(&handle, 150);

    /* Call dealloc function and check if all handles are freed */
    HANDLE_DeallocAll();
    TEST_ASSERT_SIZE_EQ(HANDLE_LUT_DEFAULT_SIZE, HANDLE_CountFree());
}
