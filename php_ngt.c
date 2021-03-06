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
	STD_PHP_INI_ENTRY("ngt.index_dir",  NULL, PHP_INI_SYSTEM, OnUpdateString, index_dir, zend_ngt_globals, ngt_globals)
PHP_INI_END()
/* }}} */

/* Handlers */
static zend_object_handlers ngt_object_handlers;

/* Class entries */
zend_class_entry *php_ngt_sc_entry;

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

/* {{{ proto void ngt::create(int dimension [, int edgeSizeForCreation, int edgeSizeForSearch, string distanceType, string defaultObjectType])
 */
PHP_METHOD(ngt, create)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();
	zend_long dimension;
	zend_long edgeSizeForCreation = 10;
	zend_long edgeSizeForSearch = 40;
	char *distanceType;
	size_t distanceType_len = 0;
	char *objectType;
	size_t objectType_len = 0;
	char *defaultDistanceType = "L2";
	char *defaultObjectType = "Float";

	ngt_obj = Z_NGT_P(object);

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l|llss", 
			&dimension, &edgeSizeForCreation, &edgeSizeForSearch,
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
		ngt_obj->ngt,
    	dimension,
    	edgeSizeForCreation,
    	edgeSizeForSearch,
    	distanceType,
    	objectType
	);
}
/* }}} */


/* {{{ proto string ngt::getObjectString()
 */
PHP_METHOD(ngt, getObjectString)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();
	NGTStr output;

	ngt_obj = Z_NGT_P(object);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	output = NgtGetObjectString(ngt_obj->ngt);

	ZVAL_STRINGL(return_value, output->buff, output->len);
	NgtStrFree(output);
}
/* }}} */

/* {{{ proto string ngt::getDistanceString()
 */
PHP_METHOD(ngt, getDistanceString)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();
	NGTStr output;

	ngt_obj = Z_NGT_P(object);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	output = NgtGetDistanceString(ngt_obj->ngt);

	ZVAL_STRINGL(return_value, output->buff, output->len);
	NgtStrFree(output);
}
/* }}} */


/* {{{ proto string ngt::getLeafNodeString()
 */
PHP_METHOD(ngt, getLeafNodeString)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();
	NGTStr output;

	ngt_obj = Z_NGT_P(object);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	output = NgtGetLeafNodeString(ngt_obj->ngt);

	ZVAL_STRINGL(return_value, output->buff, output->len);
	NgtStrFree(output);
}
/* }}} */


/* {{{ proto string ngt::getInternalNodeString()
 */
PHP_METHOD(ngt, getInternalNodeString)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();
	NGTStr output;

	ngt_obj = Z_NGT_P(object);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	output = NgtGetInternalNodeString(ngt_obj->ngt);

	ZVAL_STRINGL(return_value, output->buff, output->len);
	NgtStrFree(output);
}
/* }}} */

/* {{{ proto void ngt::setObjectString(string object)
 */
PHP_METHOD(ngt, setObjectString)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();
	char *objectstr;
	size_t objectstr_len = 0, size = 0;

	ngt_obj = Z_NGT_P(object);
	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &objectstr, &objectstr_len)) {
		return;
	}

	if (0 == objectstr_len) {
		return;
	}
	size = ('\0' == objectstr[objectstr_len]) ? objectstr_len - 1 : objectstr_len;

	NgtSetObjectString(ngt_obj->ngt, (const char*)objectstr, size);
}
/* }}} */

/* {{{ proto void ngt::setDistanceString(string distance)
 */
PHP_METHOD(ngt, setDistanceString)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();
	char *distance;
	size_t distance_len = 0, size = 0;

	ngt_obj = Z_NGT_P(object);
	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &distance, &distance_len)) {
		return;
	}

	if (0 == distance_len) {
		return;
	}
	size = ('\0' == distance[distance_len]) ? distance_len - 1 : distance_len;

	NgtSetDistanceString(ngt_obj->ngt, (const char*)distance, size);
}
/* }}} */

/* {{{ proto void ngt::setLeafNodeString(string leaf)
 */
PHP_METHOD(ngt, setLeafNodeString)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();
	char *leaf;
	size_t leaf_len = 0, size = 0;

	ngt_obj = Z_NGT_P(object);
	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &leaf, &leaf_len)) {
		return;
	}

	if (0 == leaf_len) {
		return;
	}
	size = ('\0' == leaf[leaf_len]) ? leaf_len - 1 : leaf_len;

	NgtSetLeafNodeString(ngt_obj->ngt, (const char*)leaf, size);
}
/* }}} */

/* {{{ proto void ngt::setInternalNodeString(string internal)
 */
PHP_METHOD(ngt, setInternalNodeString)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();
	char *internal;
	size_t internal_len = 0, size = 0;

	ngt_obj = Z_NGT_P(object);
	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &internal, &internal_len)) {
		RETURN_FALSE;
	}

	if (0 == internal_len) {
		RETURN_FALSE;
	}
	size = ('\0' == internal[internal_len]) ? internal_len - 1 : internal_len;

	NgtSetInternalNodeString(ngt_obj->ngt, (const char*)internal, size);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto string ngt::importIndex()
 */
PHP_METHOD(ngt, importIndex)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();

	ngt_obj = Z_NGT_P(object);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	NgtImportIndex(ngt_obj->ngt);
}
/* }}} */

/* {{{ proto string ngt::exportIndex()
 */
PHP_METHOD(ngt, exportIndex)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();

	ngt_obj = Z_NGT_P(object);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	NgtExportIndex(ngt_obj->ngt);
}
/* }}} */

/* {{{ proto void ngt::insert(array data[, int objectCount, int numThreads])
 */
PHP_METHOD(ngt, insert)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();
	zval *array;
	zend_long objectCount = 1;
	zend_long numThreads = 4;
	smart_str buff = {0};
	HashTable *ht;
	int size;

	ngt_obj = Z_NGT_P(object);
	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z|ll", &array, &objectCount, &numThreads)) {
		return;
	}

	if (NgtEmptyIndex(ngt_obj->ngt)) {
		zend_throw_exception(zend_ce_exception, "Unable to open database.", 0);
		return;
	}

	ht = Z_ARRVAL_P(array);
	size = zend_hash_num_elements(ht);
	if (size != NgtGetDimension(ngt_obj->ngt)) {
		zend_throw_exception(zend_ce_exception, "The number of dimensions does not match.", 0);
		return;
	}

	php_json_encode(&buff, array, PHP_JSON_NUMERIC_CHECK);
	NgtInsert(ngt_obj->ngt, (const char*)ZSTR_VAL(buff.s), ZSTR_LEN(buff.s), objectCount, numThreads);
}
/* }}} */

/* {{{ proto void ngt::insertList(array data[, int numThreads])
 */
PHP_METHOD(ngt, insertList)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();
	zval *array;
	zend_long numThreads = 4;
	HashTable *ht;
	int idx, size, dim;

	ngt_obj = Z_NGT_P(object);
	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z|ll", &array, &numThreads)) {
		return;
	}

	if (NgtEmptyIndex(ngt_obj->ngt)) {
		zend_throw_exception(zend_ce_exception, "Unable to open database.", 0);
		return;
	}

	ht = Z_ARRVAL_P(array);
	size = zend_hash_num_elements(ht);
	dim = NgtGetDimension(ngt_obj->ngt);

	for (idx=0; idx<size; idx++) {
		zval *value = zend_hash_get_current_data(ht);
		smart_str buff = {0};
		HashTable *_ht = Z_ARRVAL_P(value);
		int _size = zend_hash_num_elements(_ht);

		if (dim == _size) {
			php_json_encode(&buff, value, PHP_JSON_NUMERIC_CHECK);
			NgtInsert(ngt_obj->ngt, (const char*)ZSTR_VAL(buff.s), ZSTR_LEN(buff.s), 1, numThreads);
		}
		zend_hash_move_forward(ht);
	}
}
/* }}} */


/* {{{ proto mixed ngt::search(array query[, int row, float epsilon, int edgeSize])
 */
PHP_METHOD(ngt, search)
{
	php_ngt_object *ngt_obj;
	zval *object = getThis();
	zval *array;
	zend_long row = 10;
	double epsilon = 0.1;
	zend_long edgeSize = -1;
	smart_str buff = {0};
	NGTStr json;
	HashTable *ht;
	int size;

	ngt_obj = Z_NGT_P(object);
	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z|ldl", &array, &row, &epsilon, &edgeSize)) {
		return;
	}

	if (NgtEmptyIndex(ngt_obj->ngt)) {
		zend_throw_exception(zend_ce_exception, "Unable to open database.", 0);
		return;
	}

	ht = Z_ARRVAL_P(array);
	size = zend_hash_num_elements(ht);
	if (size != NgtGetDimension(ngt_obj->ngt)) {
		zend_throw_exception(zend_ce_exception, "The number of dimensions does not match.", 0);
		return;
	}

	php_json_encode(&buff, array, PHP_JSON_NUMERIC_CHECK);
	json = NgtSearch(ngt_obj->ngt, (const char*)ZSTR_VAL(buff.s), ZSTR_LEN(buff.s), row, (float)epsilon, edgeSize);

	array_init(return_value);
	php_json_decode(return_value, json->buff, json->len, 1, PHP_JSON_PARSER_DEFAULT_DEPTH);
	NgtStrFree(json);
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

	if (NgtEmptyIndex(ngt_obj->ngt)) {
		zend_throw_exception(zend_ce_exception, "Unable to open database.", 0);
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

	if (NgtEmptyIndex(ngt_obj->ngt)) {
		zend_throw_exception(zend_ce_exception, "Unable to open database.", 0);
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

ZEND_BEGIN_ARG_INFO_EX(arginfo_ngt_data, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ngt_insert, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, objectCount)
	ZEND_ARG_INFO(0, numThreads)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ngt_insertL, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, numThreads)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ngt_search, 0, 0, 1)
	ZEND_ARG_INFO(0, query)
	ZEND_ARG_INFO(0, size)
	ZEND_ARG_INFO(0, epsilon)
	ZEND_ARG_INFO(0, edgeSize)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ngt_five, 0, 0, 2)
	ZEND_ARG_INFO(0, dimension)
	ZEND_ARG_INFO(0, edgeSizeForCreation)
	ZEND_ARG_INFO(0, edgeSizeForSearch)
	ZEND_ARG_INFO(0, distanceType)
	ZEND_ARG_INFO(0, objectType)
ZEND_END_ARG_INFO()

/* }}} */


/* {{{ php_sngt_class_methods */
static zend_function_entry php_ngt_class_methods[] = {
	PHP_ME(ngt, __construct,             arginfo_ngt_void,     ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(ngt, create,                  arginfo_ngt_five,     ZEND_ACC_PUBLIC)
	PHP_ME(ngt, getObjectString,         arginfo_ngt_void,     ZEND_ACC_PUBLIC)
	PHP_ME(ngt, getDistanceString,       arginfo_ngt_void,     ZEND_ACC_PUBLIC)
	PHP_ME(ngt, getLeafNodeString,       arginfo_ngt_void,     ZEND_ACC_PUBLIC)
	PHP_ME(ngt, getInternalNodeString,   arginfo_ngt_void,     ZEND_ACC_PUBLIC)
	PHP_ME(ngt, setObjectString,         arginfo_ngt_data,     ZEND_ACC_PUBLIC)
	PHP_ME(ngt, setDistanceString,       arginfo_ngt_data,     ZEND_ACC_PUBLIC)
	PHP_ME(ngt, setLeafNodeString,       arginfo_ngt_data,     ZEND_ACC_PUBLIC)
	PHP_ME(ngt, setInternalNodeString,   arginfo_ngt_data,     ZEND_ACC_PUBLIC)
	PHP_ME(ngt, importIndex,             arginfo_ngt_void,     ZEND_ACC_PUBLIC)
	PHP_ME(ngt, exportIndex,             arginfo_ngt_void,     ZEND_ACC_PUBLIC)
	PHP_ME(ngt, insert,                  arginfo_ngt_insert,   ZEND_ACC_PUBLIC)
	PHP_ME(ngt, insertList,              arginfo_ngt_insertL,  ZEND_ACC_PUBLIC)
	PHP_ME(ngt, search,                  arginfo_ngt_search,   ZEND_ACC_PUBLIC)
	PHP_ME(ngt, remove,                  arginfo_ngt_id,       ZEND_ACC_PUBLIC)
	PHP_ME(ngt, getObject,               arginfo_ngt_id,       ZEND_ACC_PUBLIC)

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
