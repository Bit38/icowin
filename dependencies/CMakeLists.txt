if (BUILD_TESTING)
    message("Adding munit")
    add_library(munit munit/munit.c munit/munit.h)
endif()