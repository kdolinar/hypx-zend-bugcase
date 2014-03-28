/**************************************************************
 *                                                            *
 *                      Hypno-Antology                        *
 *                                                            *
 *                            by                              *
 *                                                            *
 *                       Jesej d.o.o.                         *
 *                                                            *
 *                                                            *
 *   This is  a copy righted software.  Any reproduction or   *
 *   any  other  way  of usage  is  strictly  prohibited by   *
 *   European copy right law and patent marks ....            *
 *                                                            *
 *   File:        php_hypx.h                                  *
 *                                                            *
 *   Description: the main hypx php extension header file.    *
 *                                                            *
 *   Authors:     Kajetan Dolinar, Janez-Tomaz Krmelj         *
 *                                                            *
 **************************************************************/


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
