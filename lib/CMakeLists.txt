# /////////////////////////////////////////////////////////////
# ///                                                       ///
# ///     ██████╗ ██╗███████╗████████╗                      ///
# ///     ██╔══██╗██║██╔════╝╚══██╔══╝                      ///
# ///     ██████╔╝██║█████╗     ██║                         ///
# ///     ██╔══██╗██║██╔══╝     ██║                         ///
# ///     ██║  ██║██║██║        ██║                         ///
# ///     ╚═╝  ╚═╝╚═╝╚═╝        ╚═╝                         ///
# ///     * RIFT CORE - The official compiler for Rift.     ///
# ///     * Copyright (c) 2024, Rift-Org                    ///
# ///     * License terms may be found in the LICENSE file. ///
# ///                                                       ///
# /////////////////////////////////////////////////////////////

set( SOURCES
    # Scanner
    scanner/tokens.cc
    scanner/scanner.cc
    reader/reader.cc

    # Utils
    utils/arithmetic.cc
    utils/literals.cc

    # AST
    ast/eval.cc
    ast/parser.cc
    ast/printer.cc

    # Driver
    driver/driver.cc

    # Utils
    error/error.cc

    # Main
    main.cc
)


add_executable(
    riftlang 
    ${SOURCES}
)
target_compile_options(riftlang PRIVATE -Wpedantic -Wall -Wextra -Werror -Wmisleading-indentation)

add_library(riftlib STATIC ${SOURCES})
