dnl $Id$
dnl config.m4 for extension zway

PHP_ARG_WITH(zway, for zway support,
[  --with-zway             Include zway support])

if test "$PHP_ZWAY" != "no"; then
  SEARCH_PATH="/usr/src/z-way-server" 
  SEARCH_FOR="/libzway/ZWayLib.h"

  if test -r $PHP_ZWAY/$SEARCH_FOR; then
    ZWAY_DIR=$PHP_ZWAY
  else
    AC_MSG_CHECKING([for zway files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        ZWAY_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi

  if test -z "$ZWAY_DIR"; then
    AC_MSG_RESULT([not found in $ZWAY_DIR])
    AC_MSG_ERROR([Please reinstall the z-way-server distribution in $ZWAY_DIR])
  fi

  PHP_ADD_INCLUDE($ZWAY_DIR/libzway)

  PHP_ADD_LIBRARY_WITH_PATH(zcommons, $ZWAY_DIR/libs, ZWAY_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(zenocean, $ZWAY_DIR/libs, ZWAY_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(zway,     $ZWAY_DIR/libs, ZWAY_SHARED_LIBADD)

  PHP_SUBST(ZWAY_SHARED_LIBADD)
 
  PHP_ADD_BUILD_DIR($ext_builddir/classes, 1)

  PHP_NEW_EXTENSION(zway, module.c classes/zwlog.c classes/zway.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
