set( SOURCES
    # AST
    ast/printer.cc

    # Driver
    driver/driver.cc

    # Scanner
    scanner/scanner.cc
    scanner/tokens.cc
)


add_executable(
    riftlang 
    ${SOURCES}
)
target_compile_options(riftlang PRIVATE -Wpedantic -Wall -Wextra -Werror)

