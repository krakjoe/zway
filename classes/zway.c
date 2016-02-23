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

#ifndef HAVE_ZWAY_CLASS_ZWAY
#define HAVE_ZWAY_CLASS_ZWAY

#include "php.h"
#include <ZWayLib.h>
#include "zway.h"
#include "zwlog.h"

#include "ext/spl/spl_exceptions.h"

static inline zend_object* php_zway_create(zend_class_entry *ce) {
	php_zway_t *pz = (php_zway_t*) ecalloc(1, sizeof(php_zway_t) + zend_object_properties_size(ce));

	zend_object_std_init(&pz->std, ce);
	object_properties_init(&pz->std, ce);

	pz->std.handlers = &ZWay_handlers;
	
	return &pz->std;
}

static inline void php_zway_free(zend_object *zobj) {
	php_zway_t *pz = php_zway_fetch_from(zobj);

	if (pz->port)
		zend_string_release(pz->port);
	if (pz->cfolder)
		zend_string_release(pz->cfolder);
	if (pz->tfolder)
		zend_string_release(pz->tfolder);
	if (pz->zfolder)
		zend_string_release(pz->zfolder);
	if (pz->name)
		zend_string_release(pz->name);

	if (!Z_ISUNDEF(pz->zwlog)) {
		zval_ptr_dtor(&pz->zwlog);
	}

	zway_terminate(&pz->zw);

	zend_object_std_dtor(zobj);
}

ZEND_BEGIN_ARG_INFO_EX(php_zway_construct_arginfo, 0, 5, 5)
	ZEND_ARG_TYPE_INFO(0, port, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, cfolder, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, tfolder, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, zfolder, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, zwlog, ZWay\\ZWLog, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_zway_isSomething_arginfo, 0, 0, _IS_BOOL, NULL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_zway_getString_arginfo, 0, 0, IS_STRING, NULL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_zway_getLogger_arginfo, 0, 0, IS_OBJECT, "ZWay\\ZWLog", 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_zway_no_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(ZWay, __construct) {
	php_zway_t *pz = php_zway_fetch(getThis());

	zend_string *port = CG(empty_string), 
				*cfolder = CG(empty_string), 
				*tfolder = CG(empty_string), 
				*zfolder = CG(empty_string), 
				*name = CG(empty_string);

	zval *zwlog = NULL;
	ZWError zerror = NoError;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "SSSSSO", &port, &cfolder, &tfolder, &zfolder, &name, &zwlog, ZWLog_ce) != SUCCESS || !zwlog) {
		return;
	}

	zerror = zway_init(&pz->zw, ZSTR_VAL(port), ZSTR_VAL(cfolder), ZSTR_VAL(tfolder), ZSTR_VAL(zfolder), ZSTR_VAL(name), php_zwlog_fetch_zw(zwlog));
	
	if (zerror != NoError) {
		zend_throw_exception_ex(spl_ce_RuntimeException, zerror, zstrerror(zerror));
		return;
	}

	pz->port    = zend_string_copy(port);
	pz->cfolder = zend_string_copy(cfolder);
	pz->tfolder = zend_string_copy(tfolder);
	pz->zfolder = zend_string_copy(zfolder);
	pz->name    = zend_string_copy(name);

	ZVAL_COPY(&pz->zwlog, zwlog);
}

PHP_METHOD(ZWay, getPort) {
	php_zway_t *pz = php_zway_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_STR(pz->port);
}

PHP_METHOD(ZWay, getConfigFolder) {
	php_zway_t *pz = php_zway_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_STR(pz->cfolder);
}

PHP_METHOD(ZWay, getTranslationsFolder) {
	php_zway_t *pz = php_zway_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_STR(pz->tfolder);
}

PHP_METHOD(ZWay, getZDDXFolder) {
	php_zway_t *pz = php_zway_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_STR(pz->zfolder);
}

PHP_METHOD(ZWay, getName) {
	php_zway_t *pz = php_zway_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_STR(pz->name);
}

PHP_METHOD(ZWay, getLogger) {
	php_zway_t *pz = php_zway_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	ZVAL_COPY(return_value, &pz->zwlog);
}

PHP_METHOD(ZWay, discover) {
	php_zway_t *pz = php_zway_fetch(getThis());
	ZWError zerror = NoError;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	zerror = zway_discover(pz->zw);

	if (zerror != NoError) {
		zend_throw_exception_ex(
			spl_ce_RuntimeException, zerror, zstrerror(zerror));
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

PHP_METHOD(ZWay, isIdle) {
	php_zway_t *pz = php_zway_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_BOOL(zway_is_idle(pz->zw));
}

PHP_METHOD(ZWay, isRunning) {
	php_zway_t *pz = php_zway_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_BOOL(zway_is_running(pz->zw));
}

static inline void php_zway_terminate(void *arg) {
	php_zway_t *pz = (php_zway_t*) arg;
}

PHP_METHOD(ZWay, start) {
	php_zway_t *pz = php_zway_fetch(getThis());
	ZWError zerror = NoError;
	
	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	zerror = zway_start(pz->zw, (ZTerminationCallback) php_zway_terminate, pz);

	if (zerror != NoError) {
		zend_throw_exception_ex(
			spl_ce_RuntimeException, zerror, zstrerror(zerror));
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

PHP_METHOD(ZWay, stop) {
	php_zway_t *pz = php_zway_fetch(getThis());
	ZWError zerror = NoError;
	
	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	zerror = zway_stop(pz->zw);

	if (zerror != NoError) {
		zend_throw_exception_ex(
			spl_ce_RuntimeException, zerror, zstrerror(zerror));
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

static zend_function_entry php_zway_methods[] = {
	PHP_ME(ZWay, __construct, php_zway_construct_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(ZWay, getPort, php_zway_getString_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(ZWay, getConfigFolder, php_zway_getString_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(ZWay, getTranslationsFolder, php_zway_getString_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(ZWay, getZDDXFolder, php_zway_getString_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(ZWay, getName, php_zway_getString_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(ZWay, getLogger, php_zway_getLogger_arginfo, ZEND_ACC_PUBLIC)

	PHP_ME(ZWay, discover, php_zway_no_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(ZWay, isIdle, php_zway_isSomething_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(ZWay, isRunning, php_zway_isSomething_arginfo, ZEND_ACC_PUBLIC)

	PHP_ME(ZWay, start, php_zway_isSomething_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(ZWay, stop, php_zway_no_arginfo, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

PHP_MINIT_FUNCTION(ZWay_ce) {
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "ZWay", "ZWay", php_zway_methods);	

	ZWay_ce = zend_register_internal_class(&ce);
	ZWay_ce->create_object = php_zway_create;

	memcpy(&ZWay_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	ZWay_handlers.offset = XtOffsetOf(php_zway_t, std);
	ZWay_handlers.free_obj = php_zway_free;
}
#endif

