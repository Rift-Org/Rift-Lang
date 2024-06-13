set(
    SOURCES
    main.cc

    # Unit Tests
    test/parser.cc
    test/scanner.cc

    # Mock Tests
)

add_executable(
    riftlangtest
    ${SOURCES}
)

target_compile_options(riftlangtest PRIVATE -Wpedantic -Wall -Wextra -Werror)
add_test(NAME riftlangtest COMMAND riftlangtest)

# Link against all libs
target_link_libraries(riftlangtest gtest gmock riftlib)