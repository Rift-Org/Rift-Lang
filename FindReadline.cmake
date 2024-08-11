# Try to find readline include dirs and libraries

# Variables used by this module:
# Readline_ROOT_DIR - Set this to the root installation of readline

# Variables defined by this module:
# READLINE_FOUND - System has readline, include and lib dirs found
# Readline_INCLUDE_DIR - The readline include directories
# Readline_LIBRARY - The readline library

find_path(Readline_ROOT_DIR
    NAMES include/readline/readline.h
)

find_path(Readline_INCLUDE_DIR
    NAMES readline/readline.h
    HINTS ${Readline_ROOT_DIR}/include
)

find_library(Readline_LIBRARY
    NAMES readline
    HINTS ${Readline_ROOT_DIR}/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Readline
    DEFAULT_MSG
    Readline_INCLUDE_DIR
    Readline_LIBRARY
)

mark_as_advanced(
    Readline_ROOT_DIR
    Readline_INCLUDE_DIR
    Readline_LIBRARY
)

# Set READLINE_FOUND for backwards compatibility
set(READLINE_FOUND ${Readline_FOUND})