set( SOURCES

    # Main
    main.cc
    
    # Scanner
    scanner/tokens.cc
    scanner/scanner.cc
    reader/reader.cc

    # Parsing
    parser/parser.cc
    ast/printer.cc

    # Driver
    driver/driver.cc

    # Utils
    error/error.cc
)


add_executable(
    riftlang 
    ${SOURCES}
)
target_compile_options(riftlang PRIVATE -Wpedantic -Wall -Wextra -Werror -Wmisleading-indentation)

add_library(riftlib STATIC ${SOURCES})
