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

#ifndef HAVE_ZWAY_CLASS_ZWLOG
#define HAVE_ZWAY_CLASS_ZWLOG

#include "php.h"
#include <ZWayLib.h>
#include <ZLog.h>

#include "ext/spl/spl_exceptions.h"

#include "zwlog.h"

static inline zend_object* php_zwlog_create(zend_class_entry *ce) {
	php_zwlog_t *pz = (php_zwlog_t*) ecalloc(1, sizeof(php_zwlog_t) + zend_object_properties_size(ce));

	zend_object_std_init(&pz->std, ce);
	object_properties_init(&pz->std, ce);

	pz->std.handlers = &ZWLog_handlers;
	
	return &pz->std;
}

static inline void php_zwlog_free(zend_object *zobj) {
	php_zwlog_t *pz = php_zwlog_fetch_from(zobj);

	zlog_close(pz->zw);

	zend_object_std_dtor(zobj);
}

ZEND_BEGIN_ARG_INFO_EX(php_zwlog_construct_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(ZWLog, __construct) {
	php_zwlog_t *pz = php_zwlog_fetch(getThis());
	zend_long level = PHP_ZWLOG_DEBUG;
	zend_string *file = CG(empty_string),
				*mode = CG(empty_string);

	if (ZEND_NUM_ARGS() > 1) {
		if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "SS|l", &file, &mode, &level) != SUCCESS) {
			return;
		}
	} else if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|l", &level) != SUCCESS) {
		return;
	}

	pz->level = level;

	if ((file && ZSTR_LEN(file)) && (mode && ZSTR_LEN(mode))) {
		FILE *stdio = fopen(ZSTR_VAL(file), ZSTR_VAL(mode));
		if (!stdio) {
			zend_throw_exception_ex(spl_ce_RuntimeException, errno, "cannot open %s(%s) for logging",
				ZSTR_VAL(file), ZSTR_VAL(mode));
		} else pz->zw = zlog_create(stdio, pz->level);
	} else pz->zw = zlog_create_syslog(pz->level);
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_zwlog_getLevel_arginfo, 0, 0, IS_LONG, NULL, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(ZWLog, getLevel) {
	php_zwlog_t *pz = php_zwlog_fetch(getThis());
	
	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_LONG(pz->level);
}

ZEND_BEGIN_ARG_INFO_EX(php_zwlog_error_arginfo, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, source, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, level, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, error, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(ZWLog, error) {
	php_zwlog_t *pz = php_zwlog_fetch(getThis());
	zend_string *source = CG(empty_string),
				*message = CG(empty_string);
	zend_long level = pz->level,
			  error = NoError;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "SlS|l", &source, &level, &message, &error) != SUCCESS) {
		return;
	}

	zlog_write(pz->zw, ZSTR_VAL(source), level, ZSTR_VAL(message), (ZWError) error);
}

static zend_function_entry php_zwlog_methods[] = {
	PHP_ME(ZWLog, __construct, php_zwlog_construct_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(ZWLog, getLevel, php_zwlog_getLevel_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(ZWLog, error, php_zwlog_error_arginfo, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

PHP_MINIT_FUNCTION(ZWLog_ce) {
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "ZWay", "ZWLog", php_zwlog_methods);	

	ZWLog_ce = zend_register_internal_class(&ce);
	ZWLog_ce->create_object = php_zwlog_create;

	memcpy(&ZWLog_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	ZWLog_handlers.offset = XtOffsetOf(php_zwlog_t, std);
	ZWLog_handlers.free_obj = php_zwlog_free;

	zend_declare_class_constant_long(ZWLog_ce, ZEND_STRL("DEBUG"), PHP_ZWLOG_DEBUG);
	zend_declare_class_constant_long(ZWLog_ce, ZEND_STRL("INFO"), PHP_ZWLOG_INFO);
	zend_declare_class_constant_long(ZWLog_ce, ZEND_STRL("WARN"), PHP_ZWLOG_WARN);
	zend_declare_class_constant_long(ZWLog_ce, ZEND_STRL("ERROR"), PHP_ZWLOG_ERROR);
	zend_declare_class_constant_long(ZWLog_ce, ZEND_STRL("CRITICAL"), PHP_ZWLOG_CRITICAL);
	zend_declare_class_constant_long(ZWLog_ce, ZEND_STRL("SILENT"), PHP_ZWLOG_SILENT);
}
#endif
