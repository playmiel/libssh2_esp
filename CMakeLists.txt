set(LIBSSH2_DIR libssh2)

set(CSOURCES    src/agent.c
                src/bcrypt_pbkdf.c
                src/channel.c
                src/comp.c
                src/chacha.c
                src/cipher-chachapoly.c
                src/crypt.c
                src/crypto.c
                src/global.c
                src/hostkey.c
                src/keepalive.c
                src/kex.c
                src/knownhost.c
                src/mac.c
                src/misc.c
                src/packet.c
                src/pem.c
                src/poly1305.c
                src/publickey.c
                src/scp.c
                src/session.c
                src/sftp.c
                src/transport.c
                src/userauth.c
                src/userauth_kbd_packet.c
                src/version.c
                
                src/blowfish.c
                src/mbedtls.c
                )

set(INCLUDES    src
                .)

idf_component_register( SRCS ${CSOURCES}
                        INCLUDE_DIRS ${INCLUDES}
                        REQUIRES mbedtls esp_netif)

# Differences in platform data type sizes generate print formating warnings.
# Disable treatment of these warnings as errors.
idf_build_set_property(COMPILE_OPTIONS "-Wno-error=format" APPEND)

idf_build_set_property(COMPILE_OPTIONS "-Wno-error=maybe-uninitialized" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Wno-error=uninitialized" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Wno-error=narrowing" APPEND)
# Set the crypto engine for libssh2
idf_build_get_property(crypto_mbedtls CONFIG_LIBSSH2_CRYPTO_ENGINE_MBEDTLS)

if(crypto_mbedtls)
  idf_build_set_property(COMPILE_DEFINITIONS "-DLIBSSH2_MBEDTLS" APPEND)
endif()

# Compile libssh2 with debug logging
idf_build_get_property(logging CONFIG_LIBSSH2_DEBUG_ENABLE)
if(logging)
  set(DEBUG_LOGGING_DEFAULT ON)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src" ${CMAKE_MODULE_PATH})

include(CheckFunctionExists)
include(CheckSymbolExists)
include(CheckIncludeFiles)
include(CMakePushCheckState)
include(FeatureSummary)
include(CheckFunctionExistsMayNeedLibrary)
include(CheckNonblockingSocketSupport)

file(READ "src/libssh2.h" _header_contents)
string(REGEX REPLACE ".*#define LIBSSH2_VERSION[ \t]+\"([^\"]+)\".*" "\\1" LIBSSH2_VERSION "${_header_contents}")
string(REGEX REPLACE ".*#define LIBSSH2_VERSION_MAJOR[ \t]+([0-9]+).*" "\\1" LIBSSH2_VERSION_MAJOR "${_header_contents}")
string(REGEX REPLACE ".*#define LIBSSH2_VERSION_MINOR[ \t]+([0-9]+).*" "\\1" LIBSSH2_VERSION_MINOR "${_header_contents}")
string(REGEX REPLACE ".*#define LIBSSH2_VERSION_PATCH[ \t]+([0-9]+).*" "\\1" LIBSSH2_VERSION_PATCH "${_header_contents}")
unset(_header_contents)

if(NOT LIBSSH2_VERSION OR
   NOT LIBSSH2_VERSION_MAJOR MATCHES "^[0-9]+$" OR
   NOT LIBSSH2_VERSION_MINOR MATCHES "^[0-9]+$" OR
   NOT LIBSSH2_VERSION_PATCH MATCHES "^[0-9]+$")
  message(FATAL_ERROR "Unable to parse version from src/libssh2.h")
endif()

set(LIBSSH2_LIBS_SOCKET "")
set(LIBSSH2_LIBS "")
set(LIBSSH2_LIBDIRS "")
set(LIBSSH2_PC_REQUIRES_PRIVATE "")

check_function_exists_may_need_library("socket" HAVE_SOCKET "socket")
if(NEED_LIB_SOCKET)
  list(APPEND LIBSSH2_LIBS_SOCKET "socket")
endif()
check_function_exists_may_need_library("inet_addr" HAVE_INET_ADDR "nsl")
if(NEED_LIB_NSL)
  list(APPEND LIBSSH2_LIBS_SOCKET "nsl")
endif()

# Enable debugging logging by default if the user configured a debug build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(DEBUG_LOGGING_DEFAULT OFF)
endif()
option(ENABLE_DEBUG_LOGGING "Log execution with debug trace" ${DEBUG_LOGGING_DEFAULT})
add_feature_info(Logging ENABLE_DEBUG_LOGGING "Logging of execution with debug trace")
if(ENABLE_DEBUG_LOGGING)
  # Must be visible to the library and tests using internals
  add_definitions("-DLIBSSH2DEBUG")
endif()

option(LIBSSH2_NO_DEPRECATED "Build without deprecated APIs" OFF)
add_feature_info("Without deprecated APIs" LIBSSH2_NO_DEPRECATED "")
if(LIBSSH2_NO_DEPRECATED)
  add_definitions("-DLIBSSH2_NO_DEPRECATED")
endif()

# Auto-detection

# Prefill values with known detection results
# Keep this synced with src/libssh2_setup.h
## Platform checks
check_include_files("inttypes.h" HAVE_INTTYPES_H)
check_include_files("unistd.h" HAVE_UNISTD_H)
check_include_files("sys/time.h" HAVE_SYS_TIME_H)
check_include_files("sys/select.h" HAVE_SYS_SELECT_H)
check_include_files("sys/uio.h" HAVE_SYS_UIO_H)
check_include_files("sys/socket.h" HAVE_SYS_SOCKET_H)
check_include_files("sys/ioctl.h" HAVE_SYS_IOCTL_H)
check_include_files("sys/un.h" HAVE_SYS_UN_H)
check_include_files("arpa/inet.h" HAVE_ARPA_INET_H)
check_include_files("netinet/in.h" HAVE_NETINET_IN_H)

if(HAVE_SYS_TIME_H)
  check_symbol_exists("gettimeofday" "sys/time.h" HAVE_GETTIMEOFDAY)
else()
  check_function_exists("gettimeofday" HAVE_GETTIMEOFDAY)
endif()
check_symbol_exists("strtoll" "stdlib.h" HAVE_STRTOLL)
if(NOT HAVE_STRTOLL)
  # Try _strtoi64() if strtoll() is not available
  check_symbol_exists("_strtoi64" "stdlib.h" HAVE_STRTOI64)
endif()
check_symbol_exists("snprintf" "stdio.h" HAVE_SNPRINTF)
check_symbol_exists("explicit_bzero" "string.h" HAVE_EXPLICIT_BZERO)
check_symbol_exists("explicit_memset" "string.h" HAVE_EXPLICIT_MEMSET)
check_symbol_exists("memset_s" "string.h" HAVE_MEMSET_S)


check_function_exists("poll" HAVE_POLL)
check_function_exists("select" HAVE_SELECT)

# Non-blocking socket support tests. Use a separate, yet unset variable
# for the socket libraries to not link against the other configured
# dependencies which might not have been built yet.
cmake_push_check_state()
list(APPEND CMAKE_REQUIRED_LIBRARIES ${LIBSSH2_LIBS_SOCKET})
check_nonblocking_socket_support()
cmake_pop_check_state()

# Config file

add_definitions("-DHAVE_CONFIG_H")

# Generate libssh2_config.h from template
configure_file("src/libssh2_config_cmake.h.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/libssh2_config.h")
