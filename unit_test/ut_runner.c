#include "ut.h"
#include "unity.h"

#include <stdio.h>

int main(void)
{
    UNITY_BEGIN();

    /* HANDLE module */
    RUN_TEST(UT_HANDLE_Alloc_AfterFirstAllocationZeroHandleIsReturned);

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
