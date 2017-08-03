#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([MQCrypto], [1.0.0], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([src/mqcrypto.cpp])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lbz2':
AC_CHECK_LIB([bz2], [BZ2_bzWriteOpen])
PKG_CHECK_MODULES([openssl], openssl, [], AC_MSG_ERROR([Failed to find libssl-dev.]))
PKG_CHECK_MODULES([libsodium], libsodium, [], AC_MSG_ERROR([Failed to find libsodium.]))

# Checks for header files.
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([fcntl.h float.h inttypes.h limits.h malloc.h memory.h netinet/in.h stddef.h stdint.h stdlib.h string.h sys/ioctl.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([dup2 memchr memmove memset sqrt strerror])

AC_CONFIG_FILES([Makefile 
		src/Makefile 
		src/sage/Makefile 
		src/sage/MPKC/Makefile
		src/sage/MPKC/MatsumotoImai/Makefile
		src/sage/MPKC/MatsumotoImai/Sflash/Makefile
		src/sage/MPKC/UOV/Makefile
		src/sage/MPKC/Utils/Makefile])
AC_OUTPUT
