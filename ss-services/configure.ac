AC_PREREQ(2.61)
AC_INIT([pd-mapper], 1.0.0)
AM_INIT_AUTOMAKE([-Wall gnu foreign])
AM_MAINTAINER_MODE
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])


AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O
AM_PROG_AS
AC_PROG_LIBTOOL
AC_PROG_AWK
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
LT_INIT

AS_CASE([$host],
    [arm*], [ARM=yes],
    [ARM=no]
)

# libjson
PKG_CHECK_MODULES([LIBJSON], [json-c])
AC_SUBST([LIBJSON_CFLAGS])
AC_SUBST([LIBJSON_LIBS])

AM_CONDITIONAL(ARM, [test "x$ARM" = "xyes"])
PKG_CHECK_MODULES([QMIF], [qmi-framework])
AC_SUBST([CFLAGS])
AC_SUBST([CC])
AC_SUBST([QMIF_CFLAGS])
AC_SUBST([QMIF_LIBS])
AC_CONFIG_FILES([Makefile pd-mapper/Makefile pd-mapper/pd-mapper-svc/Makefile pd-mapper/libpdmapper/Makefile pd-notifier/Makefile pd-notifier/libpdnotifier/Makefile api-test/Makefile])
AC_OUTPUT
