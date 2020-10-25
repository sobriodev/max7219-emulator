#include "ut.h"
#include "unity.h"

#include <stdio.h>

int main(void)
{
    UNITY_BEGIN();

    /* HANDLE module */
    RUN_TEST(UT_HANDLE_Alloc_AfterFirstAllocationZeroHandleIsReturned);
    RUN_TEST(UT_HANDLE_Alloc_HandlesAreReturnedInAscendingOrder);
    RUN_TEST(UT_HANDLE_Alloc_NothingIsDoneWhenNullPointerIsPassed);
    RUN_TEST(UT_HANDLE_AllocWithAllocator_AllocErrReturnedWhenAllocFnFails);

    return UNITY_END();
}

void setUp()
{
    /* There must be setUp definition */
}
void tearDown()
{
    /* There must be tearDown definition */
}
