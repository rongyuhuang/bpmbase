#include "test_common.h"

TEST_UNIT(sample_test)
{
    int a = 1;
    int b = a;
    H_TEST_ASSERT(a == b);
    int c = a + b;
    H_TEST_ASSERT(c == a + b);

    H_TEST_ASSERT(printf("Please using H_TEST_ASSERT instead of assert, H_ASSERT, CPPUNIT_ASSERT!\n"));
    H_TEST_ASSERT(false || printf("If assert failed, we can use printf to print this some error message, because printf return int!\n"));
}
