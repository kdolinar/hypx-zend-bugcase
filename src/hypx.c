#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>

#ifdef ZTS
#include "TSRM.h"
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "zend.h"
#include "php_ini.h"
#include "zend_API.h"
#include "php_hypx.h"

#include "inquiry.h"
#include "expert.h"

static int le_hypx_strategy;

ZEND_BEGIN_ARG_INFO_EX(arginfo_phxy_log, 0, 0, 3)
    ZEND_ARG_INFO(0, level)
    ZEND_ARG_INFO(0, scope)
    ZEND_ARG_INFO(0, format)
ZEND_END_ARG_INFO()

static zend_function_entry hypx_functions[] = {
    {NULL, NULL, NULL}
};

/* HypxInquiry class properties */
static zend_class_entry * hypx_inquiry_ce;

ZEND_BEGIN_ARG_INFO_EX(arginfo_hypx_inquiry__construct, 0, 0, 1)
    ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

static zend_function_entry hypx_inquiry_methods[];
zend_object_handlers hypx_inquiry_object_handlers;

typedef struct _hypx_inquiry_object {
    zend_object std;
    hypx_therapeutic_inquiry inquiry;
} hypx_inquiry_object;

ZEND_BEGIN_ARG_INFO_EX(arginfo_hypx_inquiry_comrads_get_label, 0, 0, 1)
    ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

/* HypxRadiosInquiry class properties */
static zend_class_entry * hypx_inquiry_radios_ce;

static zend_function_entry hypx_inquiry_radios_methods[];
zend_object_handlers hypx_inquiry_radios_object_handlers;

/* HypxExpert class properties */
static zend_class_entry * hypx_expert_ce;

ZEND_BEGIN_ARG_INFO_EX(arginfo_hypx_expert_make_inquiry, 0, 0, 1)
    ZEND_ARG_INFO(0, phase)
ZEND_END_ARG_INFO()

static zend_function_entry hypx_expert_methods[];

/* the module entry stuff */
zend_module_entry hypx_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_HYPX_EXTNAME,
    hypx_functions,
    PHP_MINIT(hypx),
    PHP_MSHUTDOWN(hypx),
    PHP_RINIT(hypx),
    PHP_RSHUTDOWN(hypx),
    NULL,
#if ZEND_MODULE_API_NO >= 20010901
    PHP_HYPX_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_HYPX
ZEND_GET_MODULE(hypx)
#endif

/* the class binding constructors and destructors */
static zend_object_value hypx_inquiry_object_common_create_handler(
    zend_class_entry *type, 
    zend_objects_free_object_storage_t storage, 
    zend_object_handlers * handlers TSRMLS_DC
) {
    zend_object_value retval = { 0, NULL };

    hypx_inquiry_object * obj;
    obj = (hypx_inquiry_object *) emalloc(sizeof(struct _hypx_inquiry_object));
    if (obj == NULL) return retval;
    zend_object_std_init((zend_object *)obj,type TSRMLS_CC);

#if PHP_VERSION_ID < 50399
    zval *tmp;
    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_copy(obj->std.properties, &type->default_properties,
        (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
#else
    object_properties_init((zend_object *)obj, type);
#endif

    retval.handle = zend_objects_store_put(obj, NULL, storage, NULL TSRMLS_CC);
    retval.handlers = handlers;

    return retval;
}

void hypx_inquiry_object_free_storage(void *object TSRMLS_DC) {
    hypx_inquiry_object * obj = (hypx_inquiry_object *) object;
    if (--obj->inquiry->inq_links == 0) 
    zend_objects_free_object_storage(&obj->std TSRMLS_CC);
}

zend_object_value hypx_inquiry_object_create_handler (
    zend_class_entry *type TSRMLS_DC
) {
    return hypx_inquiry_object_common_create_handler(
        type,hypx_inquiry_object_free_storage,
        &hypx_inquiry_object_handlers TSRMLS_CC);
}

zend_object_value hypx_inquiry_radios_object_create_handler (
    zend_class_entry *type TSRMLS_DC
) {
    return hypx_inquiry_object_common_create_handler(
        type,hypx_inquiry_object_free_storage,
        &hypx_inquiry_radios_object_handlers TSRMLS_CC);
}

int hypx_inquiry_object_compare_objects(zval *object1, zval *object2 TSRMLS_DC) {

    hypx_inquiry_object * obj1 = (hypx_inquiry_object *)
        zend_object_store_get_object(object1 TSRMLS_CC);
    hypx_inquiry_object * obj2 = (hypx_inquiry_object *)
        zend_object_store_get_object(object2 TSRMLS_CC);

    if (obj1->inquiry->inq_id < obj2->inquiry->inq_id) return -1;
    if (obj1->inquiry->inq_id > obj2->inquiry->inq_id) return 1;
    return 0;
}

PHP_MINIT_FUNCTION(hypx) {

    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "HypxExpert", hypx_expert_methods);
    hypx_expert_ce = zend_register_internal_class(&ce TSRMLS_CC);
    zend_declare_property_null(hypx_expert_ce, "me", 2, ZEND_ACC_PRIVATE | ZEND_ACC_STATIC);
    hypx_expert_ce->create_object = NULL;

    INIT_CLASS_ENTRY(ce, "HypxInquiry", hypx_inquiry_methods);
    hypx_inquiry_ce = zend_register_internal_class(&ce TSRMLS_CC);
    hypx_inquiry_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
    hypx_inquiry_ce->create_object = hypx_inquiry_object_create_handler;
    memcpy(&hypx_inquiry_object_handlers, zend_get_std_object_handlers(), 
        sizeof(zend_object_handlers));
    hypx_inquiry_object_handlers.compare_objects = hypx_inquiry_object_compare_objects;
    hypx_inquiry_object_handlers.clone_obj = NULL;

    INIT_CLASS_ENTRY(ce, "HypxRadiosInquiry", hypx_inquiry_radios_methods);
    hypx_inquiry_radios_ce = zend_register_internal_class_ex(&ce, hypx_inquiry_ce, NULL TSRMLS_CC);
    hypx_inquiry_radios_ce->create_object = hypx_inquiry_radios_object_create_handler;
    memcpy(&hypx_inquiry_radios_object_handlers, zend_get_std_object_handlers(), 
        sizeof(zend_object_handlers));
    hypx_inquiry_radios_object_handlers.clone_obj = NULL;

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(hypx) {
    return SUCCESS;
}

PHP_RINIT_FUNCTION(hypx) {
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(hypx) {
    return SUCCESS;
}

/* HypxInquiry class methods */
static int hypx_inquiry_class_internal_constructor(
    zval * object, int type, hypx_therapeutic_inquiry inquiry
) {
    struct timeval tm;

    inquiry->inq_type = (unsigned char) type;
    if (gettimeofday(&tm,NULL) == -1) return -1;
    inquiry->inq_id = tm.tv_sec;
    inquiry->inq_id *= 1000000;
    inquiry->inq_id += tm.tv_usec;

    hypx_inquiry_object * obj = (hypx_inquiry_object *)
        zend_object_store_get_object(object TSRMLS_CC);
    obj->inquiry = inquiry;

    return 0;
}

PHP_METHOD(HypxInquiry, __construct) {
    long type;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &type) == FAILURE) RETURN_NULL();

    hypx_therapeutic_inquiry inquiry;
    inquiry = emalloc(sizeof(struct _hypx_therapeutic_inquiry_struct));
    if (inquiry == NULL) RETURN_NULL();
    inquiry->inq_content = NULL;
    inquiry->inq_links = 1;
    inquiry->inq_numalt = 0;

    zval * object = getThis();
    if (hypx_inquiry_class_internal_constructor(object,type,inquiry)) RETURN_NULL();
}

static zend_function_entry hypx_inquiry_methods[] = {
    PHP_ME(HypxInquiry, __construct, arginfo_hypx_inquiry__construct, ZEND_ACC_PROTECTED | ZEND_ACC_CTOR)
    {NULL, NULL, NULL}
};

/* HypxRadiosInquiry class methods */
static int hypx_radios_inquiry_class_internal_constructor(zval * object) {
    struct _hypx_comrads_inquiry_content * content;

    hypx_therapeutic_inquiry inquiry;
    inquiry = emalloc(sizeof(struct _hypx_therapeutic_inquiry_struct));
    if (inquiry == NULL) return -1;

    if (hypx_inquiry_class_internal_constructor(object,
        HYPX_INQUIRY_TYPE_RADIOS,inquiry)) return -1;
    inquiry->inq_links = 1;
    inquiry->inq_numalt = 0;

    content = emalloc(sizeof(struct _hypx_comrads_inquiry_content));
    if (content == NULL) return -1;
    content->crinq_numbuttons = 0;
    content->crinq_question = "";
    inquiry->inq_content = content;

    return 0;
}

PHP_METHOD(HypxRadiosInquiry, __construct) {
    zval * object = getThis();
    if (hypx_radios_inquiry_class_internal_constructor(object)) RETURN_NULL();
}

PHP_METHOD(HypxRadiosInquiry, getQuestion) {
    zval * object = getThis();
    hypx_inquiry_object * obj = (hypx_inquiry_object *)
        zend_object_store_get_object(object TSRMLS_CC);
    if (obj->inquiry == NULL) RETURN_NULL();

    struct _hypx_comrads_inquiry_content * content = 
        (struct _hypx_comrads_inquiry_content *)
            obj->inquiry->inq_content;
    if (content == NULL) RETURN_NULL();
    if (content->crinq_question == NULL) RETURN_NULL();
    RETURN_STRING(content->crinq_question, 1);
}

PHP_METHOD(HypxRadiosInquiry, getLabel) {
    long i;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &i) == FAILURE) RETURN_NULL();

    zval * object = getThis();
    hypx_inquiry_object * obj = (hypx_inquiry_object *)
        zend_object_store_get_object(object TSRMLS_CC);
    if (obj->inquiry == NULL) RETURN_NULL();

    struct _hypx_comrads_inquiry_content * content = 
        (struct _hypx_comrads_inquiry_content *)
            obj->inquiry->inq_content;
    if (content == NULL) RETURN_NULL();

    if (i < 0 || i >= content->crinq_numbuttons) RETURN_NULL();
    if (content->crinq_label[i] == NULL) RETURN_NULL();
    RETURN_STRING(content->crinq_label[i], 1);
}

static zend_function_entry hypx_inquiry_radios_methods[] = {
    PHP_ME(HypxRadiosInquiry, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(HypxRadiosInquiry, getQuestion, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(HypxRadiosInquiry, getLabel, arginfo_hypx_inquiry_comrads_get_label, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

PHP_METHOD(HypxExpert, get) {
    zval * z;
    if ((z = zend_read_static_property(hypx_expert_ce,"me",2,0)) == NULL) RETURN_NULL();
    if (Z_TYPE_P(z) == IS_NULL) {
        if (object_init_ex(return_value,hypx_expert_ce) != SUCCESS) RETURN_NULL();
        if (zend_update_static_property(hypx_expert_ce,"me",2,return_value) != SUCCESS) RETURN_NULL();
    } else {
        RETVAL_ZVAL(z, 0, 0);
    }
}

PHP_METHOD(HypxExpert, makeInquiry) {
    long phase;
    zend_class_entry * ce;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &phase) == FAILURE) RETURN_NULL();
    hypx_therapeutic_inquiry inquiry = hypx_expert_make_inquiry(phase);
    if (inquiry == NULL) RETURN_NULL();

    switch (inquiry->inq_type) {
        case HYPX_INQUIRY_TYPE_RADIOS: ce = hypx_inquiry_radios_ce;
            if (object_init_ex(return_value,hypx_inquiry_radios_ce) != SUCCESS) RETURN_NULL();
            if (hypx_inquiry_class_internal_constructor(return_value,HYPX_INQUIRY_TYPE_RADIOS,inquiry)) RETURN_NULL();
            break;
        default: RETURN_NULL();
    }
    inquiry->inq_links = 1;
}

static zend_function_entry hypx_expert_methods[] = {
    PHP_ME(HypxExpert, get, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(HypxExpert, makeInquiry, arginfo_hypx_expert_make_inquiry, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};
