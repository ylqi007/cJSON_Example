
# 2018-11-19
Implement the following functions:
* cJSON_CreateObject()
* cJSON_AddItemToObject()
* cJSON_CreateNumber

# 2018-11-20
Implement the following functions:
* char *print_number(cJSON *item, printbuffer *p);
* static char *ensure(printbuffer *p, int needed);	--> This `ensure()` function allocates the needed memory.

1. Moving `printbuffer` from `cJSON.c` to `cJSON.h`;
2. Moving `static char *ensure(printbuffer *p, int needed)` from `cJSON.c` to `char *ensure(printbuffer *p, int needed)` in `cJSON.h`.

* How to use header files? [header](https://gcc.gnu.org/onlinedocs/cpp/Header-Files.html#Header-Files) [header](http://www.gnu.org/software/libc/manual/html_node/Header-Files.html)

# 2018-11-21
* char *print_value(cJSON *item, printbuffer *p);
* const char *parse_number(cJSON *item, const char *num); // The returned `num` is different with the input argument `num`.
eg. input "9.01000" and return `item->valuedouble=9.009995`.

# 2018-11-22
1. *C guarantees that zero is never a valid address for data, so a return value of zero can be used to signal an abnormal event, in this case no space.*
> Pointers and integers are not interchangeable. Zero is the sole exception: the constant zero may be assigned to a pointer, and a pointer may be compared with the constant zero. The symbolic constant NULL is often used in place of zero, as a mnemonic to indicate more clearly that this is a special value for a pointer. NULL is defined in <stdio.h>. 
2. `static char *print_string_ptr(const char *str, printbuffer *p);`  // Render the cstring provided to an escaped version that can be printed.

`print_string(cJSON *item, printbuffer *p)` will call subroutine `print_string_ptr(const char *str, printbuffer *p)`.

3. `print_object()` and `parse_object()`

# 2018-11-27
1. `sizeof(cJSON)`
	In the original code, the size of `cJSON` is 64 because of some padding bytes. I change the order of the elements in the definition of cJSON and the size changed to 56. [reference](https://stackoverflow.com/questions/119123/why-isnt-sizeof-for-a-struct-equal-to-the-sum-of-sizeof-of-each-member)
2. `cJSON_CreateObject()` ==> `cJSON_Object_test()`;
3. `cJSON_CreateNumber()` ==> `cJSON_Number_test()`;
4. `cJSON_CreateString()` ==> `cJSON_String_test()`;
* The `char *strcpy(char *s1, const char *s2)` function copies the string pointed to by *s2* **(including the terminating null character)** into the array pointed to by *s1*. If copying takes place between objects that overlap, the behavior is undefined.
* The `void *memcpy(void *s1, const void *s2, size_t n)` function copies n characters from the object pointed to by *s2* into the object pointed to by *s1*. If copying takes place between objects that overlap, the behavior is undefined.
* The `size_t strlen(const char *s)` function returns the number of characters *that precede the terminating null character*.
* The `void *malloc(size_t size)` function allocates space for an object whose size is specified by size and whose value is indeterminate. The `malloc` function returns either a null pointer or a pointer to the allocated space.
5. `print_number()` 
* [Why 21 chars?](http://mathcentral.uregina.ca/QQ/database/QQ.02.06/trang1.html)

















