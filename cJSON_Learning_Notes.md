
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

