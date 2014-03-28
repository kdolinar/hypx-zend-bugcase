#ifndef PHP_HYPX_H
#define PHP_HYPX_H 1

#define PHP_HYPX_VERSION "1.0"
#define PHP_HYPX_EXTNAME "hypx"

PHP_MINIT_FUNCTION(hypx);
PHP_MSHUTDOWN_FUNCTION(hypx);
PHP_RINIT_FUNCTION(hypx);
PHP_RSHUTDOWN_FUNCTION(hypx);

PHP_FUNCTION();

extern zend_module_entry hypx_module_entry;
#define phpext_hypx_ptr &hypx_module_entry

#endif
