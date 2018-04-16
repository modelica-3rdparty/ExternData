dnl Init
AC_PREREQ([2.63])
AC_INIT([ExternData],[2.3.4],[https://github.com/tbeu/ExternData/issues],[ExternData],[https://github.com/tbeu/ExternData])

AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_MACRO_DIR([m4])

if test -z "$CFLAGS"; then
  CFLAGS="-Os"
fi

AC_LANG([C])
AC_PROG_CC
AC_PROG_CPP
AC_PROG_MAKE_SET
LT_INIT([disable-shared])

CPPFLAGS="$CPPFLAGS -DHAVE_MEMMOVE=1 -DHAVE_ZLIB=1 -DHAVE_HDF5=1 -DNDEBUG -DXML_POOR_ENTROPY -I../../C-Sources/ -I../../C-Sources/bsxml-json -I../../C-Sources/expat/lib -I../../C-Sources/hdf5/include -I../../C-Sources/inih -I../../C-Sources/libxls/include -I../../C-Sources/minizip -I../../C-Sources/modelica -I../../C-Sources/zlib -I../../C-Sources/parson"

if test "$libdir" = '${exec_prefix}/lib'; then
  # It is hard to detect where to put the libraries if we cross-compile. Let the tool decide where to put it.
  # Use Resources/Library/ as the default.
  libdir=`pwd`/../../Library/
fi

AC_OUTPUT([Makefile])
