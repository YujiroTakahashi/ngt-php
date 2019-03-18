#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/json/php_json.h"
#include "php_ngt.h"
#include "main/SAPI.h"


#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "SAPI.h"

ZEND_DECLARE_MODULE_GLOBALS(ngt)

/* {{{ PHP_INI
*/
PHP_INI_BEGIN()
	STD_PHP_INI_ENTRY("ngt.model_dir",  NULL, PHP_INI_SYSTEM, OnUpdateString, model_dir, zend_ngt_globals, ngt_globals)
PHP_INI_END()
/* }}} */

/* Handlers */
static zend_object_handlers ngt_object_handlers;

/* Class entries */
zend_class_entry *php_ngt_sc_entry;

/* {{{ proto void ngt::CreateDB()
 */
PHP_METHOD(ngt, CreateDB)
{
	char *path;
	size_t path_len;
	zend_long dimension;
	zend_long edgeSizeForCreation = 10;
	zend_long edgeSizeForSearch = 40;
	char *distanceType;
	size_t distanceType_len = 0;
	char *objectType;
	size_t objectType_len = 0;
	char *defaultDistanceType = "L2";
	char *defaultObjectType = "Float";

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "sl|llss", 
			&path, &path_len, &dimension, &edgeSizeForCreation, &edgeSizeForSearch,
			&distanceType, distanceType_len, &objectType, &objectType_len)
	) {
		return;
	}

	if (0 == distanceType_len) {
		distanceType = defaultDistanceType;
	}
	if (0 == objectType_len) {
		objectType = defaultObjectType;
	}

	NgtCreateDB(
    	path,
    	dimension,
    	edgeSizeForCreation,
    	edgeSizeForSearch,
    	distanceType,
    	objectType
	);
}
/* }}} */

/* {{{ proto void ngt::__construct()
 */
PHP_METHOD(ngt, __construct)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();

	ngt_obj = Z_NGT_P(object);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	ngt_obj->ngt = NgtCreate();
}
/* }}} */

/* {{{ proto void ngt::open(String filename[], bool readOnly])
 */
PHP_METHOD(ngt, open)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();
	char *filename;
	size_t filename_len;
	zend_bool readonly=0;

	ngt_obj = Z_NGT_P(object);

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s|b", &filename, &filename_len, &readonly)) {
		return;
	}
	NgtOpen(ngt_obj->ngt, filename, (readonly? 1:0));
}
/* }}} */

/* {{{ proto void ngt::insert(array data[[, int objectCount,] int numThreads])
 */
PHP_METHOD(ngt, insert)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();
	zval *array;
	zend_long objectCount = 1;
	zend_long numThreads = 4;

	HashTable *ht;
	float *data;
	int idx, size;

	ngt_obj = Z_NGT_P(object);
	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z|ll", &array, &objectCount, &numThreads)) {
		return;
	}

	ht = Z_ARRVAL_P(array);
	size = zend_hash_num_elements(ht);
	data = (float*)safe_emalloc(sizeof(float), size, 0);

	for (idx=0; idx<size; idx++) {
		zval *value = zend_hash_get_current_data(ht);
		data[idx] = (float)zval_get_double(value);
		zend_hash_move_forward(ht);
	}

	NgtInsert(ngt_obj->ngt, data, objectCount, numThreads);

	efree((float*)data);
}
/* }}} */

/* {{{ proto mixed ngt::search(array query[[[, int row], float epsilon], int edgeSize])
 */
PHP_METHOD(ngt, search)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();
	zval *array;
	zend_long row = 10;
	double epsilon = 0.1;
	zend_long edgeSize = -1;

	HashTable *ht;
	float *query;
	int idx, size;
	NGTStr json;

	ngt_obj = Z_NGT_P(object);
	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z|ldl", &array, &row, &epsilon, &edgeSize)) {
		return;
	}

	ht = Z_ARRVAL_P(array);
	size = zend_hash_num_elements(ht);
	query = (float*)safe_emalloc(sizeof(float), size, 0);

	for (idx=0; idx<size; idx++) {
		zval *value = zend_hash_get_current_data(ht);
		query[idx] = (float)zval_get_double(value);
		zend_hash_move_forward(ht);
	}

	json = NgtSearch(ngt_obj->ngt, query, row, (float)epsilon, edgeSize);

	array_init(return_value);
	php_json_decode(return_value, json->buff, json->len, 1, PHP_JSON_PARSER_DEFAULT_DEPTH);
	NgtStrFree(json);
	efree((float*)query);
}
/* }}} */


/* {{{ proto mixed ngt::remove(int id)
 */
PHP_METHOD(ngt, remove)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();
	zend_long id;

	ngt_obj = Z_NGT_P(object);

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &id)) {
		return;
	}
	NgtRemove(ngt_obj->ngt, id);
}
/* }}} */

/* {{{ proto mixed ngt::getObject(int id)
 */
PHP_METHOD(ngt, getObject)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();
	zend_long id;
	NGTStr json;

	ngt_obj = Z_NGT_P(object);

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &id)) {
		return;
	}

	json = NgtGetObject(ngt_obj->ngt, id);

	array_init(return_value);
	php_json_decode(return_value, json->buff, json->len, 1, PHP_JSON_PARSER_DEFAULT_DEPTH);
	NgtStrFree(json);
}
/* }}} */


/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_ngt_void, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ngt_id, 0, 0, 1)
	ZEND_ARG_INFO(0, id)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ngt_open, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, readOnly)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ngt_insert, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, objectCount)
	ZEND_ARG_INFO(0, numThreads)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ngt_search, 0, 0, 1)
	ZEND_ARG_INFO(0, query)
	ZEND_ARG_INFO(0, size)
	ZEND_ARG_INFO(0, epsilon)
	ZEND_ARG_INFO(0, edgeSize)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ngt_six, 0, 0, 2)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, dimension)
	ZEND_ARG_INFO(0, edgeSizeForCreation)
	ZEND_ARG_INFO(0, edgeSizeForSearch)
	ZEND_ARG_INFO(0, distanceType)
	ZEND_ARG_INFO(0, objectType)
ZEND_END_ARG_INFO()

/* }}} */


/* {{{ php_sngt_class_methods */
static zend_function_entry php_ngt_class_methods[] = {
	PHP_ME(ngt, CreateDB,     arginfo_ngt_six,      ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(ngt, __construct,  arginfo_ngt_void,	    ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(ngt, open,         arginfo_ngt_open,	    ZEND_ACC_PUBLIC)
	PHP_ME(ngt, insert,       arginfo_ngt_insert,   ZEND_ACC_PUBLIC)
	PHP_ME(ngt, search,       arginfo_ngt_search,   ZEND_ACC_PUBLIC)
	PHP_ME(ngt, remove,       arginfo_ngt_id,       ZEND_ACC_PUBLIC)
	PHP_ME(ngt, getObject,    arginfo_ngt_id,       ZEND_ACC_PUBLIC)

	PHP_FE_END
};
/* }}} */

static void php_ngt_object_free_storage(zend_object *object) /* {{{ */
{
	php_ngt_object *intern = php_ngt_from_obj(object);

	if (!intern) {
		return;
	}

	if (intern->ngt) {
		NgtFree(intern->ngt);
		intern->ngt = NULL;
	}

	zend_object_std_dtor(&intern->zo);
}
/* }}} */

static zend_object *php_ngt_object_new(zend_class_entry *class_type) /* {{{ */
{
	php_ngt_object *intern;

	/* Allocate memory for it */
	int ngtsize = NgtSize();
	intern = ecalloc(1, sizeof(php_ngt_object) + zend_object_properties_size(class_type) + ngtsize);

	zend_object_std_init(&intern->zo, class_type);
	object_properties_init(&intern->zo, class_type);

	intern->zo.handlers = &ngt_object_handlers;

	return &intern->zo;
}
/* }}} */


/* {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(ngt)
{
	zend_class_entry ce;

	memcpy(&ngt_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	/* Register Ngt Class */
	INIT_CLASS_ENTRY(ce, "Croco\\NGT\\Index", php_ngt_class_methods);
	ce.create_object = php_ngt_object_new;
	ngt_object_handlers.offset = XtOffsetOf(php_ngt_object, zo);
	ngt_object_handlers.clone_obj = NULL;
	ngt_object_handlers.free_obj = php_ngt_object_free_storage;
	php_ngt_sc_entry = zend_register_internal_class(&ce);

	REGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
*/
PHP_MSHUTDOWN_FUNCTION(ngt)
{
	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
*/
PHP_MINFO_FUNCTION(ngt)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "SentencePieceProcessor support", "enabled");
	php_info_print_table_row(2, "SentencePieceProcessor module version", PHP_NGT_VERSION);
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ PHP_GINIT_FUNCTION
*/
static PHP_GINIT_FUNCTION(ngt)
{
	memset(ngt_globals, 0, sizeof(*ngt_globals));
}
/* }}} */

/* {{{ ngt_module_entry
*/
zend_module_entry ngt_module_entry = {
	STANDARD_MODULE_HEADER,
	"ngt",
	NULL,
	PHP_MINIT(ngt),
	PHP_MSHUTDOWN(ngt),
	NULL,
	NULL,
	PHP_MINFO(ngt),
	PHP_NGT_VERSION,
	PHP_MODULE_GLOBALS(ngt),
	PHP_GINIT(ngt),
	NULL,
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_NGT
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(ngt)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
