/*
   +----------------------------------------------------------------------+
   | PHP Version 4                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997, 1998, 1999, 2000, 2001 The PHP Group             |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.02 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available at through the world-wide-web at                           |
   | http://www.php.net/license/2_02.txt.                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors:  Derick Rethans <d.rethans@jdimedia.nl>                     |
   +----------------------------------------------------------------------+
 */

#ifndef PHP_XDEBUG_H
#define PHP_XDEBUG_H

#define XDEBUG_VERSION "1.2.0dev"

#include "php.h"

#include "xdebug_handlers.h"
#include "xdebug_hash.h"
#include "xdebug_llist.h"

extern zend_module_entry xdebug_module_entry;
#define phpext_xdebug_ptr &xdebug_module_entry

#ifdef PHP_WIN32
#define PHP_XDEBUG_API __declspec(dllexport)
#else
#define PHP_XDEBUG_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif


/* Memory allocators */
#if 0
#define xdmalloc    emalloc
#define xdcalloc    ecalloc	
#define xdrealloc   erealloc
#define xdfree      efree
#define xdstrdup    estrdup
#else  
#define xdmalloc    malloc
#define xdcalloc    calloc	
#define xdrealloc   realloc
#define xdfree      free
#define xdstrdup    strdup
#endif

PHP_MINIT_FUNCTION(xdebug);
PHP_MSHUTDOWN_FUNCTION(xdebug);
PHP_RINIT_FUNCTION(xdebug);
PHP_RSHUTDOWN_FUNCTION(xdebug);
PHP_MINFO_FUNCTION(xdebug);


PHP_FUNCTION(xdebug_get_function_stack);
PHP_FUNCTION(xdebug_call_function);
PHP_FUNCTION(xdebug_call_file);
PHP_FUNCTION(xdebug_call_line);

PHP_FUNCTION(xdebug_enable);
PHP_FUNCTION(xdebug_disable);
PHP_FUNCTION(xdebug_is_enabled);

PHP_FUNCTION(xdebug_start_trace);
PHP_FUNCTION(xdebug_stop_trace);
PHP_FUNCTION(xdebug_get_function_trace);
PHP_FUNCTION(xdebug_dump_function_trace);
PHP_FUNCTION(xdebug_start_profiling);
PHP_FUNCTION(xdebug_stop_profiling);
PHP_FUNCTION(xdebug_dump_function_profile);
PHP_FUNCTION(xdebug_get_function_profile);

#if MEMORY_LIMIT
PHP_FUNCTION(xdebug_memory_usage);
#endif

typedef struct xdebug_var {
	char *name;
	char *value;
} xdebug_var;

#define XFUNC_UNKNOWN        0
#define XFUNC_NORMAL         1
#define XFUNC_STATIC_MEMBER  2
#define XFUNC_MEMBER         3
#define XFUNC_NEW            4
#define XFUNC_EVAL           5
#define XFUNC_INCLUDE        6
#define XFUNC_INCLUDE_ONCE   7
#define XFUNC_REQUIRE        8
#define XFUNC_REQUIRE_ONCE   9

#define XDEBUG_IS_FUNCTION(f) (f == XFUNC_NORMAL || f == XFUNC_STATIC_MEMBER || f == XFUNC_MEMBER)

#define XDEBUG_REGISTER_LONG_CONSTANT(__c) REGISTER_LONG_CONSTANT(#__c, __c, CONST_CS|CONST_PERSISTENT)

#define XDEBUG_NONE      0
#define XDEBUG_JIT       1
#define XDEBUG_REQ       2

#define XDEBUG_BREAK        1
#define XDEBUG_STEP         2

#define XDEBUG_INTERNAL     1
#define XDEBUG_EXTERNAL     2

#define XDEBUG_PROFILER_LBL    0   /* line by line */
#define XDEBUG_PROFILER_CPU    1   /* sorted by execution time */
#define XDEBUG_PROFILER_NC     2   /* number of function calls */
#define XDEBUG_PROFILER_FS_AV  3   /* sorted by avg. exection time */
#define XDEBUG_PROFILER_FS_SUM 4   /* sorted by total time taken by each function */
#define XDEBUG_PROFILER_FS_NC  5   /* sorted by total number of function calls */
#define XDEBUG_PROFILER_SD_LBL 6   /* hierarchical view of the functions, sorted by line numbers */
#define XDEBUG_PROFILER_SD_CPU 7   /* hierarchical view of the functions, sorted by cpu usage */
#define XDEBUG_PROFILER_SD_NC  8   /* hierarchical view of the functions, sorted by function calls */

#define XDEBUG_PROFILER_MODES  9

#define XDEBUG_PROFILER_LBL_D    "Execution Time Profile (sorted by line numbers)"
#define XDEBUG_PROFILER_CPU_D    "Execution Time Profile (sorted by execution time)"
#define XDEBUG_PROFILER_NC_D     "Execution Time Profile (sorted by number of calls to each function)"
#define XDEBUG_PROFILER_FS_AV_D  "Function Summary Profile (sorted by avg. execution time)"
#define XDEBUG_PROFILER_FS_SUM_D "Function Summary Profile (sorted by total execution time)"
#define XDEBUG_PROFILER_FS_NC_D  "Function Summary Profile (sorted by number of function calls)"
#define XDEBUG_PROFILER_SD_LBL_D "Stack-Dump Profile (sorted by line numbers)"
#define XDEBUG_PROFILER_SD_CPU_D "Stack-Dump Profile (sorted by execution time)"
#define XDEBUG_PROFILER_SD_NC_D  "Stack-Dump Profile (sorted by number of calls to each function)"

#define XDEBUG_MAX_FUNCTION_LEN 1024

typedef struct xdebug_func {
	char *class;
	char *function;
	int   type;
	int   internal;
} xdebug_func;

typedef struct function_stack_entry {
	xdebug_func  function;
	int          user_defined;

	char *filename;
	int   lineno;

	int   arg_done;
	int   varc;
	xdebug_var vars[20];

	unsigned int memory;
	double       time;

	xdebug_hash *used_vars;

	int   level;
	int   refcount;

	/* used for profiling */
	double       time_taken;	
	unsigned int f_calls;
} function_stack_entry;

typedef struct xdebug_tree_p {
	int n_func;
	function_stack_entry **subf;
} xdebug_tree_p;

typedef struct xdebug_tree_out {
	int nelem;
	int pos;
	struct xdebug_tree_out *parent; 
	struct function_stack_entry *fse;
	struct xdebug_tree_out **children;
} xdebug_tree_out;

ZEND_BEGIN_MODULE_GLOBALS(xdebug)
	int           level;
	xdebug_llist *stack;
	xdebug_llist *trace;
	int           max_nesting_level;
	zend_bool     default_enable;
	zend_bool     collect_params;
	zend_bool     auto_trace;
	zend_bool     do_trace;
	char         *manual_url;
	FILE         *trace_file;

	/* used for profiling */
	double 	      total_execution_time;
	double 	      total_compiling_time;
	zend_bool     do_profile;
	zend_bool     profiler_trace;
	FILE         *profile_file;
	zend_bool     auto_profile;
	char         *output_dir;
	int           auto_profile_mode;

	/* remote settings */
	zend_bool     remote_enable;  /* 0 */
	int           remote_port;    /* 17869 */
	char         *remote_host;    /* localhost */
	int           remote_mode;    /* XDEBUG_NONE, XDEBUG_JIT, XDEBUG_REQ */
	char         *remote_handler; /* php3, gdb */

	/* remote debugging globals */
	zend_bool                 remote_enabled;
	xdebug_con                context;
ZEND_END_MODULE_GLOBALS(xdebug)

#ifdef ZTS
#define XG(v) TSRMG(xdebug_globals_id, zend_xdebug_globals *, v)
#else
#define XG(v) (xdebug_globals.v)
#endif
	
#endif


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
