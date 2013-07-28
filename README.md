Memory-Library
==============

A small memory library for C.

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
```
