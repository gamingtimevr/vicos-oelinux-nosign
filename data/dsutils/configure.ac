AC_PREREQ(2.61)
AC_INIT([dsutils],
        1.0.0)
AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE
AC_CONFIG_SRCDIR([src/ds_util.c])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LIBTOOL
AC_PROG_AWK
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
PKG_PROG_PKG_CONFIG

# Checks for libraries.
PKG_CHECK_MODULES([DIAG], [diag])
AC_SUBST([DIAG_CFLAGS])
AC_SUBST([DIAG_LIBS])

# Library configs

AC_ARG_WITH([lib_path],
      AC_HELP_STRING([--with-lib-path=@<:@dir@:>@],
         [Specify the location of the libraries]),
      [std_libdir=$withval],
      with_lib_path=no)

if test "x$with_lib_path" != "xno"; then
   LDFLAGS="${LDFLAGS} -L${std_libdir}"
fi

AC_ARG_WITH([stderr],
      AC_HELP_STRING([--with-stderr=@<:@dir@:>@],
         [Enable stderr logging]),
      [],
      with_stderr=no)

if test "x$with_stderr" != "xno"; then
   CPPFLAGS="${CPPFLAGS} -DFEATURE_DATA_LOG_STDERR"
fi

AC_ARG_WITH([qxdm],
      AC_HELP_STRING([--with-qxdm=@<:@dir@:>@],
         [Enable qxdm logging]),
      [],
      with_qxdm=no)

if test "x$with_qxdm" != "xno"; then
CPPFLAGS="${CPPFLAGS} -DFEATURE_DATA_LOG_QXDM"
fi

AC_ARG_WITH([glib],
      AC_HELP_STRING([--with-glib],
         [enable glib, building HLOS systems which use glib]))

if (test "x${with_glib}" = "xyes"); then
        AC_DEFINE(ENABLE_USEGLIB, 1, [Define if HLOS systems uses glib])
        PKG_CHECK_MODULES(GTHREAD, gthread-2.0 >= 2.16, dummy=yes,
                                AC_MSG_ERROR(GThread >= 2.16 is required))
        PKG_CHECK_MODULES(GLIB, glib-2.0 >= 2.16, dummy=yes,
                                AC_MSG_ERROR(GLib >= 2.16 is required))
        GLIB_CFLAGS="$GLIB_CFLAGS $GTHREAD_CFLAGS"
        GLIB_LIBS="$GLIB_LIBS $GTHREAD_LIBS"

        AC_SUBST(GLIB_CFLAGS)
        AC_SUBST(GLIB_LIBS)
fi

AC_ARG_WITH([common_includes],
      AC_HELP_STRING([--with-common-includes=@<:@dir@:>@],
         [Specify the location of the common headers]),
      [common_incdir=$withval],
      with_common_includes=no)

if test "x$with_common_includes" != "xno"; then
   CFLAGS="${CFLAGS} -I${common_incdir}"
fi

AC_ARG_WITH(sanitized-headers,
      AS_HELP_STRING([--with-sanitized-headers=DIR],
         [Specify the location of the sanitized Linux headers]),
      [CPPFLAGS="$CPPFLAGS -idirafter $withval"])

AM_CONDITIONAL(USE_GLIB, test "x${with_glib}" = "xyes")

AC_ARG_ENABLE(target,
  [AS_HELP_STRING([--enable-target=TARGET], [Specify the target product to build])],
  [TARGET=$enableval],
  [TARGET=none]
)

if test "x$TARGET" = "x9615-cdp"; then
  CPPFLAGS="${CPPFLAGS} -DFEATURE_DATA_TARGET_MDM9X15 -DFEATURE_DS_UTILS_NON_EMULATOR"
  CFLAGS="${CFLAGS} -DFEATURE_DATA_TARGET_MDM9X15 -DFEATURE_DS_UTILS_NON_EMULATOR"

elif test "x$TARGET" = "xmdm9625"; then
  CPPFLAGS="${CPPFLAGS} -DFEATURE_DATA_TARGET_MDM9X25 -DFEATURE_DS_UTILS_NON_EMULATOR"
  CFLAGS="${CFLAGS} -DFEATURE_DATA_TARGET_MDM9X25 -DFEATURE_DS_UTILS_NON_EMULATOR"

elif test "x$TARGET" = "xmdm9635"; then
  CPPFLAGS="${CPPFLAGS} -DFEATURE_DATA_TARGET_MDM9X35 -DFEATURE_DS_UTILS_NON_EMULATOR"
  CFLAGS="${CFLAGS} -DFEATURE_DATA_TARGET_MDM9X35 -DFEATURE_DS_UTILS_NON_EMULATOR"

elif test "x$TARGET" = "xmdm9640"; then
  CPPFLAGS="${CPPFLAGS} -DFEATURE_DS_UTILS_NON_EMULATOR"
  CFLAGS="${CFLAGS} -DFEATURE_DS_UTILS_NON_EMULATOR"

elif test "x$TARGET" = "xmdm9650"; then
  CPPFLAGS="${CPPFLAGS} -DFEATURE_DS_UTILS_NON_EMULATOR"
  CFLAGS="${CFLAGS} -DFEATURE_DS_UTILS_NON_EMULATOR"

elif test "x$TARGET" = "xapq8096"; then
  CPPFLAGS="${CPPFLAGS} -DFEATURE_DS_UTILS_NON_EMULATOR"
  CFLAGS="${CFLAGS} -DFEATURE_DS_UTILS_NON_EMULATOR"

elif test "x$TARGET" = "x8x96autofusion"; then
  CPPFLAGS="${CPPFLAGS} -DFEATURE_DATA_TARGET_MCTM -DFEATURE_DS_UTILS_NON_EMULATOR"
  CFLAGS="${CFLAGS} -DFEATURE_DATA_TARGET_MCTM -DFEATURE_DS_UTILS_NON_EMULATOR"
fi

AC_SUBST([CFLAGS])
AC_SUBST([CPPFLAGS])
AC_SUBST([CC])
AC_SUBST([CXX])
AC_SUBST([ARCH])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_HEADER_STDC
AC_C_INLINE
AC_TYPE_INT64_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_MALLOC

AC_CONFIG_FILES([ \
        Makefile \
        src/Makefile \
        dsutils.pc
        ])
AC_OUTPUT
