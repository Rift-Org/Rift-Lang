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
    ast/env.cc
    ast/parser.cc
    ast/printer.cc
    ast/eval.cc

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

target_compile_options(riftlang PRIVATE -Wno-gcc-compat)
target_compile_definitions(riftlang PRIVATE ABSL_USES_STD_ANY=1)

target_link_libraries(riftlang PRIVATE readline)
# target_link_libraries(riftlang absl::base absl::strings absl::hash absl::algorithm absl::memory absl::flat_hash_map absl::container_common absl::container_memory)

add_library(riftlib STATIC ${SOURCES})


