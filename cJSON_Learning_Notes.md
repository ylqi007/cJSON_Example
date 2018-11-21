
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

* How to use header files? [header](https://gcc.gnu.org/onlinedocs/cpp/Header-Files.html#Header-Files)
