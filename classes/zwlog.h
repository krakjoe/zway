/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: krakjoe@php.net                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef HAVE_ZWAY_CLASS_ZWLOG_H
#define HAVE_ZWAY_CLASS_ZWLOG_H
zend_class_entry *ZWLog_ce;
zend_object_handlers ZWLog_handlers;

typedef struct _php_zwlog_t {
	ZWLog zw;
	zend_long level;
	zend_object std;
} php_zwlog_t;

#define php_zwlog_fetch_from(z) (php_zwlog_t*) ((char*) z - XtOffsetOf(php_zwlog_t, std))
#define php_zwlog_fetch(z) (php_zwlog_fetch_from(Z_OBJ_P(z)))
#define php_zwlog_fetch_zw(z) ((php_zwlog_fetch(z))->zw)

PHP_MINIT_FUNCTION(ZWLog_ce);

#define PHP_ZWLOG_DEBUG Debug
#define PHP_ZWLOG_INFO  Information
#define PHP_ZWLOG_WARN  Warning
#define PHP_ZWLOG_ERROR Error
#define PHP_ZWLOG_CRITICAL Critical
#define PHP_ZWLOG_SILENT Silent

#endif
