#ifndef PHP_NGT_H
#define PHP_NGT_H

#include "ngt_api.h"

#define PHP_NGT_VERSION	"0.1.0"

extern zend_module_entry ngt_module_entry;
#define phpext_ngt_ptr &ngt_module_entry

ZEND_BEGIN_MODULE_GLOBALS(ngt)
	char *index_dir;
ZEND_END_MODULE_GLOBALS(ngt)

#ifdef ZTS
# define NGT_G(v) TSRMG(ngt_globals_id, zend_ngt_globals *, v)
# ifdef COMPILE_DL_NGT
ZEND_TSRMLS_CACHE_EXTERN()
# endif
#else
# define NGT_G(v) (ngt_globals.v)
#endif

typedef struct {
    zend_object zo;
	zval error;
    NgtHandle ngt;
} php_ngt_object;

static inline php_ngt_object *php_ngt_from_obj(zend_object *obj) {
	return (php_ngt_object*)((char*)(obj) - XtOffsetOf(php_ngt_object, zo));
}

#define Z_NGT_P(zv) php_ngt_from_obj(Z_OBJ_P((zv)))


#endif  /* PHP_NGT_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
