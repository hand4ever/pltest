/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_pltest.h"

#include "standard/md5.h"

#include "funcom.c"

/* If you declare any globals in php_pltest.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(pltest)
*/

/* True global resources - no need for thread safety here */
static int le_pltest;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("pltest.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_pltest_globals, pltest_globals)
    STD_PHP_INI_ENTRY("pltest.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_pltest_globals, pltest_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_pltest_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_pltest_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "pltest", arg);

	RETURN_STR(strg);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_pltest_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_pltest_init_globals(zend_pltest_globals *pltest_globals)
{
	pltest_globals->global_value = 0;
	pltest_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(pltest)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(pltest)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(pltest)
{
#if defined(COMPILE_DL_PLTEST) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(pltest)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(pltest)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "pltest support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ pltest_functions[]
 *
 * Every user visible function must have an entry in pltest_functions[].
 */
const zend_function_entry pltest_functions[] = {
	PHP_FE(confirm_pltest_compiled,	NULL)		/* For testing, remove later. */
    PHP_FE(greeting, NULL)
    PHP_FE(pltest_add, NULL)
    PHP_FE(greeting_word, NULL) /* include arguments */
    PHP_FE(pltest_md5salt, NULL)
    PHP_FE(pltest_todesign, NULL)
	PHP_FE_END	/* Must be the last line in pltest_functions[] */
};

/**
 * test 1
 */
PHP_FUNCTION(greeting) {
    //char *str = "~Greeting world!!!\n";
    //char *str2 = test1();
    //strcat(str, str2); 
    char *str = getWord();

    php_printf(str);
}

/**
 * test 2 with arguments
 */
PHP_FUNCTION(pltest_add) {
    int a;
    int b;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &a, &b) == FAILURE) {
        return ;
    }
    int c;

    php_printf("a = %d, ", a);
    printf("b = %d", b);
    c = a + b;
    RETURN_LONG(c);
}
/**
 * test string
 */
PHP_FUNCTION(greeting_word) {
    zend_string *prefix, *subject, *result;
    zval *string;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "Sz", &prefix, &string) == FAILURE) {
        return ;
    }
    subject = zval_get_string(string);
    if (zend_binary_strncmp(ZSTR_VAL(prefix), ZSTR_LEN(prefix), ZSTR_VAL(subject), ZSTR_LEN(subject), ZSTR_LEN(prefix)) == 0) {
        RETURN_STR(subject); 
    }
    result = strpprintf(0, "%s %s", ZSTR_VAL(prefix), ZSTR_VAL(subject));
    RETURN_STR(result);
}

PHP_FUNCTION(pltest_md5salt) {
    zend_string *arg, *salt;
    char *tmp= "pltest_md5salt";
    char *tmp2;
    
    size_t salt_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &salt) == FAILURE) {
        return ;
    }
    salt_len = ZSTR_LEN(salt);
    
    tmp2 = emalloc(strlen(tmp) + salt_len);
    //strcpy(tmp2, "hello");
    //strcat(tmp2, " world");

    strcpy(tmp2, tmp);
    strcat(tmp2, ZSTR_VAL(salt));

    arg = zend_string_init(tmp2, strlen(tmp2), 0);
    efree(tmp2);

    /**
     * copy from standard/md5.c
     */
    char md5str[33] = "hello";//pt_md5(arg);
    /*
    PHP_MD5_CTX context;
    unsigned char digest[16];
    md5str[0] = '\0';

    PHP_MD5Init(&context);
    PHP_MD5Update(&context, ZSTR_VAL(arg), ZSTR_LEN(arg));
    PHP_MD5Final(digest, &context);

    make_digest_ex(md5str, digest, 16);
    */

    php_printf(md5str);
    zend_string_release(arg);    
    return;
}
PHP_FUNCTION(pltest_todesign) {
   php_printf("pltest_todesign"); 
}


/* }}} */


/* {{{ pltest_module_entry
 */
zend_module_entry pltest_module_entry = {
	STANDARD_MODULE_HEADER,
	"pltest",
	pltest_functions,
	PHP_MINIT(pltest),
	PHP_MSHUTDOWN(pltest),
	PHP_RINIT(pltest),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(pltest),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(pltest),
	PHP_PLTEST_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PLTEST
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(pltest)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
