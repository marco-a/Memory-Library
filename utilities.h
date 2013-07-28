/*
 * (C) by marco-a
 */

#ifndef UTILITIES_H

#define UTILITIES_H

#ifndef UTILITIES_EXTERN
#define UTILITIES_EXTERN extern
#endif

/*
 * Imports
 */

#ifndef UTILITIES_NO_IMPORTS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <ctype.h>
#include <limits.h>

#endif

/*
 * Shortcuts
 */

// Char
typedef signed char schar_t;
typedef unsigned char uchar_t;
// Short int
typedef short signed int s_sint_t;
typedef short unsigned int s_uint_t;
// Int
typedef signed int sint_t;
typedef unsigned int uint_t;
// Long int
typedef long signed int l_sint_t;
typedef long unsigned int l_uint_t;
// Long long int
typedef long long signed int ll_sint_t;
typedef long long unsigned int ll_uint_t;

/*
 * Boolean type
 */

#ifndef TRUE

typedef uchar_t BOOL;

#define TRUE ((BOOL)1)
#define FALSE ((BOOL)0)
#define YES TRUE
#define NO FALSE

#endif

/*
 * Memory allocation helpers
 */

#ifndef nil
#define nil NULL
#endif

/* Use datatype for debug */
#define UTILITIES_MEM_USE_TYPE
/* Use retain count */
#define UTILITIES_MEM_USE_RETAIN_COUNT
/* Zero byte */
#define UTILITIES_MEM_ZERO 0x00

/*
 * Define offset
 */

#ifdef UTILITIES_MEM_USE_TYPE

#ifdef UTILITIES_MEM_USE_RETAIN_COUNT

#define UTILITIES_MEM_OFFSET ((sizeof(size_t) * 2) + sizeof(void *) + sizeof(uint_t))

#else

#define UTILITIES_MEM_OFFSET ((sizeof(size_t) * 2) + sizeof(void *))

#endif

#else

#ifdef UTILITIES_MEM_USE_RETAIN_COUNT

#define UTILITIES_MEM_OFFSET (sizeof(size_t) * 2 + sizeof(uint_t))

#else

#define UTILITIES_MEM_OFFSET (sizeof(size_t) * 2)

#endif

#endif

/*
 * @_mem_get_static_size
 * Returns static size of memory.
 */
UTILITIES_EXTERN size_t _mem_get_static_size(void *);
#define mem_get_static_size(mem) _mem_get_static_size((void *)mem)

/*
 * @_mem_get_num_elements
 * Returns number of elements.
 */
UTILITIES_EXTERN size_t _mem_get_num_elements(void *);
#define mem_get_num_elements(mem) _mem_get_num_elements((void *)mem)

/*
 * @_mem_get_element_size
 * Returns size of one element.
 */
UTILITIES_EXTERN size_t _mem_get_element_size(void *);
#define mem_get_element_size(mem) _mem_get_element_size((void *)mem)

/*
 * @_mem_get_relative_size
 * Returns size of allocated memory.
 */
UTILITIES_EXTERN size_t _mem_get_relative_size(void *);
#define mem_get_relative_size(mem) _mem_get_relative_size((void *)mem)

/*
 * @_mem_get_absolute_size
 * Returns size of allocated memory.
 */
UTILITIES_EXTERN size_t _mem_get_absolute_size(void *);
#define mem_get_absolute_size(mem) _mem_get_absolute_size((void *)mem)

/*
 * @_mem_get_datatype
 * Returns datatype of allocated memory.
 */
UTILITIES_EXTERN const char *_mem_get_datatype(void *);
#define mem_get_datatype(ptr) _mem_get_datatype((void *)ptr)

/*
 * @_mem_get_retain_count
 * Returns retain count of allocated memory.
 */
UTILITIES_EXTERN uint_t _mem_get_retain_count(void *);
#define mem_get_retain_count(ptr) _mem_get_retain_count((void *)ptr)

/*
 * @_mem_alloc
 * Allocates num * size Bytes.
 * Zeros memory.
 */
UTILITIES_EXTERN void *_mem_alloc(size_t, size_t, const char *, BOOL);
#define mem_alloc(num, type) (type *)_mem_alloc(num, sizeof(type), #type, TRUE)
#define mem_alloc_fast(num, type) (type *)_mem_alloc(num, sizeof(type), #type, FALSE)

/*
 * @_mem_alloc_static
 * Allocates static memory.
 */
UTILITIES_EXTERN void *_mem_alloc_static(size_t, size_t, BOOL);
#define mem_alloc_static(num, type) (type *)_mem_alloc_static(num, sizeof(type), TRUE)
#define mem_alloc_static_fast(num, type) (type *)_mem_alloc_static(num, sizeof(type), FALSE)

/*
 * @_mem_grow
 * Grows memory.
 */
UTILITIES_EXTERN BOOL _mem_grow(void **, size_t, BOOL);
#define mem_grow(ptr, num) _mem_grow((void **)&ptr, num, TRUE)
#define mem_grow_fast(ptr, num) _mem_grow((void **)&ptr, num, FALSE)

/*
 * @_mem_shrink
 * Shrinks memory.
 */
UTILITIES_EXTERN BOOL _mem_shrink(void **, size_t);
#define mem_shrink(ptr, num) _mem_shrink((void **)&ptr, num)

/*
 * @_mem_dealloc
 * Deallocates memory.
 */
UTILITIES_EXTERN BOOL _mem_dealloc(void **, BOOL);
#define mem_dealloc(ptr) _mem_dealloc((void **)&ptr, FALSE)
#define mem_free(ptr) _mem_dealloc((void **)&ptr, TRUE)

/*
 * @_mem_release
 * Releases memory.
 */
UTILITIES_EXTERN BOOL _mem_release(void **);
#define mem_release(ptr) _mem_release((void **)&ptr)

/*
 * @_mem_retain
 * Retains memory.
 */
UTILITIES_EXTERN void _mem_retain(void *);
#define mem_retain(ptr) _mem_retain((void *)ptr)

/*
 * @mem_dump
 * Dumps memory.
 */
UTILITIES_EXTERN void _mem_dump(void *, BOOL);
#define mem_dump(ptr) _mem_dump((void *)ptr, FALSE)
#define mem_dump_static(ptr) _mem_dump((void *)ptr, TRUE)

#endif
