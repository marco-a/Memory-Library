# Simplifying dynamic allocated memory in C #
Dynamic memory management in C is very annoying, this library aims to provide an easy and nevertheless powerful way to use dynamic allocated memory in C.

==============
## Allocate memory
```c
void *mem_alloc(size_t num, type)
```
Allocates `num * sizeof(type)` Bytes and zeros out memory with `UTILITIES_MEM_ZERO`.

```c
void *mem_alloc_fast(size_t num, type)
```
Same as `mem_alloc` but does not zero out new memory.

```c
void *mem_alloc_static(size_t num, type)
````
Allocates `num * sizeof(type)` Bytes and zeros out memory with `UTILITIES_MEM_ZERO`. Static memory is not grow or shrinkable. (Which uses less memory).

```c
void *mem_alloc_static_fast(size_t num, type)
````
Same as `mem_alloc_static` but does not zero out new memory.

## Grow & shrink memory
```c
BOOL mem_grow(void *mem, size_t num)
```
Grows memory to `old_size + (num * sizeof(pointer_type))` and zeros out new memory with `UTILITIES_MEM_ZERO`. *This function works only with non static memory*. Returns `TRUE` on success, otherwise `FALSE`.

```c
BOOL mem_grow_fast(void *mem, size_t num)
```
Same as `mem_grow` but does not zero out new memory.

```c
BOOL mem_shrink(void *mem, size_t num)
```
Shrinks memory to `old_size - (num * sizeof(pointer_type)`. *This function works only with non static memory*. Returns `TRUE` on success, otherwise `FALSE`.

## Free memory
```c
BOOL mem_dealloc(void *mem)
```
Deallocates memory. *This function works only with non static memory*. Returns `TRUE` on success, otherwise `FALSE`.

```c
BOOL mem_free(void *mem)
```
Deallocates memory. This function works only with static memory. Returns `TRUE` on success, otherwise `FALSE`.

## Helper functions

```c
size_t mem_get_num_elements(void *mem)
```
Returns number of elements. *This function works only with non static memory*.

```c
size_t mem_get_relative_size(void *mem)
```
Returns relative size of memory. *This function works only with non static memory*.

```c
size_t mem_get_absolute_size(void *mem)
```
Returns absolute size of memory (memory size + offset). *This function works only with non static memory*.

```c
size_t mem_get_element_size(void *mem)
```
Returns size of one single element. *This function works only with non static memory*.

```c
const char *mem_get_datatype(void *mem)
```
Returns datatype of memory. *This function works only with non static memory*. Returns `unknown` when `UTILITIES_MEM_USE_TYPE` is not defined.

```c
unsigned int mem_get_retain_count(void *mem)
```
Returns retain count of memory. *This function works only with non static memory*. Returns `1` when `UTILITIES_MEM_USE_RETAIN_COUNT` is not defined.

```c
size_t mem_get_static_size(void *mem)
```
Returns static size of memory. This function works only with static memory.

## Debug functions
```c
void mem_dump(void *mem)
```
Dumps memory information. *This function works only with non static memory*.

```c
void mem_dump_static(void *mem)
```
Dumps memory information. This function works only with static memory.

## Example

Output for main.c:
```
Sucessfully allocated 10 ints
Successfully shrank to 5 ints
Successfully grew to 10 ints
Number is: 1337

+-----------------[ MEMORY INFORMATION ]-----------------+
| Datatype            : uint_t                           |
| Retain count        : 1                                |
| Absolute address    : 0x100103b70                      |
| Absolute size       : 68                         Bytes |
| Offset              : 28                         Bytes |
+--------------------------------------------------------+
| Relative address    : 0x100103b8c                      |
| Relative size       : 40                         Bytes |
+--------------------------------------------------------+
| Number of elements  : 10                      Elements |
| Size of one element : 4                          Bytes |
+-----------------[ MEMORY DUMP        ]-----------------+
| 0x100103b8c         : 01 00 00 00 03 00 00 00 ........ |
| 0x100103b94         : 03 00 00 00 07 00 00 00 ........ |
| 0x100103b9c         : 00 00 00 00 00 00 00 00 ........ |
| 0x100103ba4         : 00 00 00 00 00 00 00 00 ........ |
| 0x100103bac         : 00 00 00 00 00 00 00 00 ........ |
+--------------------------------------------------------+

Memory deallocated successfully!

Successfully allocated 40 Bytes

+-----------------[ MEMORY INFORMATION ]-----------------+
| Absolute address    : 0x100103b40                      |
| Absolute size       : 48                         Bytes |
| Offset              : 8                          Bytes |
+--------------------------------------------------------+
| Relative address    : 0x100103b48                      |
| Relative size       : 40                         Bytes |
+-----------------[ MEMORY DUMP        ]-----------------+
| 0x100103b48         : 04 00 00 00 02 00 00 00 ........ |
| 0x100103b50         : 00 00 00 00 00 00 00 00 ........ |
| 0x100103b58         : 00 00 00 00 00 00 00 00 ........ |
| 0x100103b60         : 00 00 00 00 00 00 00 00 ........ |
| 0x100103b68         : 00 00 00 00 00 00 00 00 ........ |
+--------------------------------------------------------+

Number is: 42
Static memory deallocated succesfully!
```
