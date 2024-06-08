set(
    SOURCES
    main.cc

    # Driver
    driver/driver.cc
    driver/driver.hh
)

add_executable(
    riftlangtest
    ${SOURCES}
)

target_compile_options(riftlangtest PRIVATE -Wpedantic -Wall -Wextra -Werror)
add_test(NAME riftlangtest COMMAND riftlangtest)