#include <munit/munit.h>
#include <stdlib.h>
#include "../src/utils.h"

MunitResult test_to_human(const MunitParameter params[], void* user_data_or_fixture){
    char buf[25];

    munit_assert_string_equal(to_human(0, buf, 25), "0B");
    munit_assert_string_equal(to_human(1000, buf, 25), "1000B");
    munit_assert_string_equal(to_human(1024, buf, 25), "1KB");
    munit_assert_string_equal(to_human(2048, buf, 25), "2KB");
    munit_assert_string_equal(to_human(1024*1024, buf, 25), "1MB");
    munit_assert_string_equal(to_human(1024*1024*5.5, buf, 25), "5MB");
    munit_assert_string_equal(to_human(1024*1024*5.9, buf, 25), "5MB");
    munit_assert_string_equal(to_human(0xFFFFFFFF, buf, 25), "3GB");

    return MUNIT_OK;
}

MunitTest tests[] = {
  {
    "/to_human",
    test_to_human,
    NULL, 
    NULL, 
    MUNIT_TEST_OPTION_NONE, 
    NULL 
  },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite suite = {
  "/utils-test", 
  tests,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE 
};

int main(int argc, char** argv) {
   return munit_suite_main(&suite, NULL, argc, argv);
}
