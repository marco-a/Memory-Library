/*
 * (C) by marco-a
 */

#define UTILITIES_EXTERN

#define VOID_POINTER_TO_SIZE_T(pointer, index) (*((size_t *)pointer + index))
#define VOID_POINTER_TO_UINT_T(pointer, index) (*((uint_t *)pointer + index))

#include "utilities.h"

/*
 * Memory allocation helpers
 */

/*
 * @_mem_get_static_size
 * Returns static size of memory.
 */
size_t _mem_get_static_size(void *mem)
{
	if (mem == nil) {
		return 0;
	}

	mem -= sizeof(size_t);

	if (mem == nil) {
		return 0;
	}

	return VOID_POINTER_TO_SIZE_T(mem, 0);
}

/*
 * @_mem_get_num_elements
 * Returns number of elements.
 */
size_t _mem_get_num_elements(void *mem)
{
	if (mem == nil) {
		return 0;
	}

	mem -= UTILITIES_MEM_OFFSET;

	if (mem == nil) {
		return 0;
	}

	return VOID_POINTER_TO_SIZE_T(mem, 0);
}

/*
 * @_mem_get_element_size
 * Returns size of one element.
 */
size_t _mem_get_element_size(void *mem)
{
	if (mem == nil) {
		return 0;
	}

	mem -= UTILITIES_MEM_OFFSET;

	if (((size_t *)mem + 1) == nil) {
		return 0;
	}

	return VOID_POINTER_TO_SIZE_T(mem, 1);
}

/*
 * @_mem_get_relative_size
 * Returns size of allocated memory.
 */
size_t _mem_get_relative_size(void *mem)
{
	return _mem_get_num_elements(mem) * _mem_get_element_size(mem);
}

/*
 * @_mem_get_absolute_size
 * Returns size of allocated memory.
 */
size_t _mem_get_absolute_size(void *mem)
{
	return _mem_get_relative_size(mem) + UTILITIES_MEM_OFFSET;
}

/*
 * @_mem_get_datatype
 * Returns datatype of allocated memory.
 */
const char *_mem_get_datatype(void *mem)
{
	if (mem == nil) {
		return nil;
	}

#ifndef UTILITIES_MEM_USE_TYPE
	return "unknown";
#endif

	mem -= sizeof(void *);

	if (mem == nil) {
		return "unknown";
	}

#ifdef UTILITIES_MEM_USE_RETAIN_COUNT
	mem -= sizeof(uint_t);

	if (mem == nil) {
		return "unknown";
	}
#endif

	return *((const char **)mem);
}

/*
 * @_mem_get_retain_count
 * Returns retain count of allocated memory.
 */
uint_t _mem_get_retain_count(void *mem)
{
	if (mem == nil) {
		return 0;
	}

#ifndef UTILITIES_MEM_USE_RETAIN_COUNT
	return 1;
#endif

	mem -= sizeof(uint_t);

	if (mem == nil) {
		return 0;
	}

	return VOID_POINTER_TO_UINT_T(mem, 0);
}

/*
 * @_mem_alloc
 * Allocates num * size Bytes.
 * Zeros memory.
 */
void *_mem_alloc(size_t num, size_t size, const char *datatype, BOOL zero)
{
	void *mem;

	size_t absolute_size;
	size_t relative_size;

	if (num == 0 || size == 0) {
		return nil;
	}

	relative_size = num * size;

	absolute_size = relative_size + UTILITIES_MEM_OFFSET;

	if (zero == TRUE) {
#if UTILITIES_MEM_ZERO == 0x00
		mem = calloc(absolute_size, 1);
#else
		mem = malloc(absolute_size);
#endif
	} else {
		mem = malloc(absolute_size);
	}

	if (mem == nil) {
		return nil;
	}

	VOID_POINTER_TO_SIZE_T(mem, 0) = num;
	VOID_POINTER_TO_SIZE_T(mem, 1) = size;

	// Add offset
	mem += sizeof(size_t) * 2;

#ifdef UTILITIES_MEM_USE_TYPE
	*((void **)mem) = (void *)datatype;

	// Add offset
	mem += sizeof(void *);
#endif

#ifdef UTILITIES_MEM_USE_RETAIN_COUNT
	VOID_POINTER_TO_UINT_T(mem, 0) = 1;

	// Add offset
	mem += sizeof(uint_t);
#endif

#if UTILITIES_MEM_ZERO != 0x00
	if (zero == TRUE) {
		memset(mem, UTILITIES_MEM_ZERO, relative_size);
	}
#endif

	return mem;
}

/*
 * @_mem_alloc_static
 * Allocates static memory.
 */
void *_mem_alloc_static(size_t num, size_t size, BOOL zero)
{
	void *mem;
	size_t relative_size;
	size_t absolute_size;

	if (num == 0 || size == 0) {
		return nil;
	}

	relative_size = num * size;

	absolute_size = relative_size + sizeof(size_t);

	if (zero == TRUE) {
#if UTILITIES_MEM_ZERO == 0x00
		mem = calloc(absolute_size, 1);
#else
		mem = malloc(absolute_size);
#endif
	} else {
		mem = malloc(absolute_size);
	}

	if (mem == nil) {
		return nil;
	}

	VOID_POINTER_TO_SIZE_T(mem, 0) = relative_size;

	mem += sizeof(size_t);

	return mem;
}

/*
 * @_mem_grow
 * Grows memory.
 */
BOOL _mem_grow(void **mem_ptr, size_t num, BOOL zero)
{
	void *mem;
	void *new_mem;
	size_t element_size;
	size_t num_elements;
	size_t relative_size;
	size_t new_relative_size;
	size_t new_absolute_size;
	
	if (mem_ptr == nil) {
		return FALSE;
	} else if (*mem_ptr == nil) {
		return FALSE;
	} else if (num == 0) {
		return FALSE;
	}

	mem = *mem_ptr;

	element_size = mem_get_element_size(mem);
	num_elements = mem_get_num_elements(mem);
	relative_size = mem_get_relative_size(mem);

	new_relative_size = (element_size * num);
	new_relative_size += relative_size;

	if (new_relative_size <= relative_size) {
		return FALSE;
	}
	
	new_absolute_size = new_relative_size + UTILITIES_MEM_OFFSET;

	new_mem = realloc(mem - UTILITIES_MEM_OFFSET, new_absolute_size);

	if (new_mem == nil) {
		return FALSE;
	}
	
	VOID_POINTER_TO_SIZE_T(new_mem, 0) = num_elements + num;

	new_mem += UTILITIES_MEM_OFFSET;

	// Zero out new memory
	if (zero == TRUE) {
		void *start;

		start = ((uchar_t *)new_mem) + relative_size;

		memset(start, UTILITIES_MEM_ZERO, (new_relative_size - relative_size));
	}

	*mem_ptr = new_mem;

	return TRUE;
}

/*
 * @_mem_shrink
 * Shrinks memory.
 */
BOOL _mem_shrink(void **mem_ptr, size_t num)
{
	void *mem;
	void *new_mem;
	size_t num_elements;
	size_t new_num_elements;

	if (mem_ptr == nil) {
		return FALSE;
	} else if (*mem_ptr == nil) {
		return FALSE;
	} else if (num == 0) {
		return FALSE;
	}

	mem = *mem_ptr;

	num_elements = mem_get_num_elements(mem);

	if (num_elements <= num) {
		return FALSE;
	}

	new_num_elements = num_elements - num;

	new_mem = _mem_alloc(new_num_elements, mem_get_element_size(mem), mem_get_datatype(mem), FALSE);

	if (new_mem == nil) {
		return FALSE;
	}

	memcpy(new_mem, (const void *)mem, mem_get_relative_size(new_mem));

	*mem_ptr = new_mem;

	return TRUE;
}

/*
 * @_mem_dealloc
 * Deallocates memory.
 */
BOOL _mem_dealloc(void **mem_ptr, BOOL is_static)
{
	if (mem_ptr == nil) {
		return FALSE;
	} else if (*mem_ptr == nil) {
		return FALSE;
	}

	if (is_static) {
		*mem_ptr -= sizeof(size_t);
	} else {
		*mem_ptr -= UTILITIES_MEM_OFFSET;
	}

	free(*mem_ptr);

	*mem_ptr = nil;

	return TRUE;
}

/*
 * @_mem_release
 * Releases memory.
 */
BOOL _mem_release(void **mem_ptr)
{
#ifndef UTILITIES_MEM_USE_RETAIN_COUNT
	return _mem_free(mem_ptr);
#else
	void *mem;
	uint_t retain_count;
	
	if (mem_ptr == nil) {
		return FALSE;
	} else if (*mem_ptr == nil) {
		return FALSE;
	}

	mem = *mem_ptr;

	mem -= sizeof(uint_t);

	if (mem == nil) {
		return FALSE;
	}

	retain_count = VOID_POINTER_TO_UINT_T(mem, 0);
	
	if (retain_count >= 1) {
		retain_count -= 1;

		VOID_POINTER_TO_UINT_T(mem, 0) = retain_count;

		if (retain_count == 0) {
			return _mem_dealloc(mem_ptr, FALSE);
		}
	}

	return FALSE;
#endif
}

/*
 * @_mem_retain
 * Retains memory.
 */
void _mem_retain(void *mem)
{
#ifndef UTILITIES_MEM_USE_RETAIN_COUNT
	return;
#else
	uint_t retain_count;

	if (mem == nil) {
		return;
	}

	mem -= sizeof(uint_t);

	if (mem == nil) {
		return;
	}

	retain_count = VOID_POINTER_TO_UINT_T(mem, 0);

	if (retain_count == UINT_MAX) {
		return;
	}

	retain_count++;

	VOID_POINTER_TO_UINT_T(mem, 0) = retain_count;
#endif
}

#define UTILITIES_MEM_DUMP_COLS 8

/*
 * @_mem_dump
 * Dumps memory.
 */
void _mem_dump(void *mem, BOOL is_static)
{
	size_t relative_size;
	size_t num_elements;
	size_t element_size;
	size_t absolute_size;

	if (mem == nil) {
		return;
	}

	if (is_static == FALSE) {
		// Get relative size
		relative_size = mem_get_relative_size(mem);
		// Get number of elements
		num_elements = mem_get_num_elements(mem);
		// Get size of single element
		element_size = mem_get_element_size(mem);
		// Get absolute size
		absolute_size = mem_get_absolute_size(mem);
	} else {
		relative_size = mem_get_static_size(mem);

		num_elements = element_size = 0;

		absolute_size = relative_size + sizeof(size_t);
	}

	fprintf(stderr, "\n+-----------------[ MEMORY INFORMATION ]-----------------+\n");

	if (is_static == FALSE) {
		fprintf(stderr, "| Datatype            : %-32s |\n", mem_get_datatype(mem));
		fprintf(stderr, "| Retain count        : %-32u |\n", mem_get_retain_count(mem));
		fprintf(stderr, "| Absolute address    : %-32p |\n", mem - UTILITIES_MEM_OFFSET);
		fprintf(stderr, "| Absolute size       : %-26zu Bytes |\n", absolute_size);
		fprintf(stderr, "| Offset              : %-26zu Bytes |\n", UTILITIES_MEM_OFFSET);
	} else {
		fprintf(stderr, "| Absolute address    : %-32p |\n", mem - sizeof(size_t));
		fprintf(stderr, "| Absolute size       : %-26zu Bytes |\n", absolute_size);
		fprintf(stderr, "| Offset              : %-26zu Bytes |\n", sizeof(size_t));
	}

	fprintf(stderr, "+--------------------------------------------------------+\n");
	fprintf(stderr, "| Relative address    : %-32p |\n", mem);

	if (relative_size == 1) {
		fprintf(stderr, "| Relative size       : %-27zu Byte |\n", relative_size);
	} else {
		fprintf(stderr, "| Relative size       : %-26zu Bytes |\n", relative_size);
	}

	if (is_static == FALSE) {
		fprintf(stderr, "+--------------------------------------------------------+\n");
		
		if (num_elements == 1) {
			fprintf(stderr, "| Number of elements  : %-24zu Element |\n", num_elements);
		} else {

			fprintf(stderr, "| Number of elements  : %-23zu Elements |\n", num_elements);
		}

		if (element_size == 1) {
			fprintf(stderr, "| Size of one element : %-27zu Byte |\n", element_size);
		} else {
			fprintf(stderr, "| Size of one element : %-26zu Bytes |\n", element_size);
		}
	}

	fprintf(stderr, "+-----------------[ MEMORY DUMP        ]-----------------+\n");

	{
		size_t i, x;

		for (i = 0; i < relative_size + ((relative_size % UTILITIES_MEM_DUMP_COLS) ? (UTILITIES_MEM_DUMP_COLS - relative_size % UTILITIES_MEM_DUMP_COLS) : 0); ++i) {
			if (i % UTILITIES_MEM_DUMP_COLS == 0) {
				fprintf(stderr, "| 0x%-17zx : ", i + (size_t)mem);
			}

			if (i < relative_size) {
				fprintf(stderr, "%02X ", 0xFF & ((uchar_t *)mem)[i]);
			} else {
				fprintf(stderr, "   ");
			}

			if (i % UTILITIES_MEM_DUMP_COLS == (UTILITIES_MEM_DUMP_COLS - 1)) {
				for (x = i - (UTILITIES_MEM_DUMP_COLS - 1); x <= i; ++x) {
					if (x >= relative_size) {
						fprintf(stderr, " ");
					} else if (isprint(((uchar_t *)mem)[x])) {
						fprintf(stderr, "%c", 0xFF & ((uchar_t *)mem)[x]);
					} else {
						fprintf(stderr, ".");
					}
				}

				fprintf(stderr, " |\n");
			}
		}
	}

	fprintf(stderr, "+--------------------------------------------------------+\n\n");
}
