cmake_minimum_required(VERSION 3.2)
project(
    icowin 
    VERSION 0.1
    DESCRIPTION "Simple command line tool for working with .ico, .cur and .ani files "
    LANGUAGES C)

include(CTest)
add_subdirectory(dependencies)
add_subdirectory(tests)

add_executable(icowin src/cli.c src/utils.c src/format/ico.c src/main.c)
