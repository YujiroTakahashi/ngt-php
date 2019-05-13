dnl $Id$
dnl config.m4 for extension ngt

PHP_ARG_ENABLE(ngt, whether to enable ngt support,
dnl Make sure that the comment is aligned:
[  --enable-ngt           Enable ngt support])

if test "$PHP_NGT" != "no"; then
  PHP_REQUIRE_CXX()

  # --with-ngt -> check with-path
  SEARCH_PATH="/usr/local /usr"
  SEARCH_FOR="/include/NGT/Index.h"
  if test -r $PHP_NGT/$SEARCH_FOR; then # path given as parameter
    NGT_DIR=$PHP_NGT
  else # search default path list
    AC_MSG_CHECKING([for ngt files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        NGT_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi

  if test -z "$NGT_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the ngt distribution, "$PHP_NGT"])
  fi

  # --with-ngt -> add include path
  PHP_ADD_INCLUDE($NGT_DIR/include/ngt)

  # --with-ngt -> check for lib and symbol presence
  LIBNAME="ngt"
  LIBSYMBOL="NGT"

  PHP_SUBST(NGT_SHARED_LIBADD)

  PHP_ADD_LIBRARY(stdc++, 1, NGT_SHARED_LIBADD)
  PHP_ADD_LIBRARY(ngt, 1, NGT_SHARED_LIBADD)
  PHP_ADD_LIBRARY(croco_ngt, 1, NGT_SHARED_LIBADD)
  CFLAGS="-O3 -funroll-loops"
  CXXFLAGS="-pthread -std=c++14 -O3 -funroll-loops"

  PHP_NEW_EXTENSION(ngt, php_ngt.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
