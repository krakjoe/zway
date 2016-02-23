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

#ifndef HAVE_ZWAY_CLASS_ZWAY_H
#define HAVE_ZWAY_CLASS_ZWAY_H
zend_class_entry *ZWay_ce;
zend_object_handlers ZWay_handlers;

typedef struct _php_zway_t {
	ZWay zw;
	zend_string *port;
	zend_string *cfolder;
	zend_string *tfolder;
	zend_string *zfolder;
	zend_string *name;
	zval zwlog;
	zend_object std;
} php_zway_t;

#define php_zway_fetch_from(z) (php_zway_t*) ((char*) z - XtOffsetOf(php_zway_t, std))
#define php_zway_fetch(z) php_zway_fetch_from(Z_OBJ_P(z))

PHP_MINIT_FUNCTION(ZWay_ce);
#endif
