cmake_minimum_required (VERSION 2.6)
project (RpiEmbedded)

set(CMAKE_CURRENT_SOURCE_DIR .)

SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_PREFIX install)

# specify the cross compiler
set(COMPILER_PATH /usr/local/arm-bcm2708/bin/)
set(COMPILER_NAME arm-linux-gnueabihf)

SET(CMAKE_C_COMPILER   ${COMPILER_PATH}${COMPILER_NAME}-gcc)
SET(CMAKE_CXX_COMPILER ${COMPILER_PATH}${COMPILER_NAME}-g++)

#SET(CMAKE_C_COMPILER   gcc)
#SET(CMAKE_CXX_COMPILER g++)

# where is the target environment
SET(CMAKE_FIND_ROOT_PATH /usr/local/angstrom/arm/)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(INCLUDE include)

add_subdirectory (include)
add_subdirectory (lib)
add_subdirectory (examples)
add_subdirectory (apps)
