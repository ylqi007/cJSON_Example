/*
  Copyright (c) 2009 Dave Gamble

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

/* cJSON */
/* JSON parser in C. */

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>
#include <ctype.h>
#include "cJSON.h"

static void *(*cJSON_malloc)(size_t sz) = malloc;
static void (*cJSON_free)(void *ptr) = free;

static char* cJSON_strdup(const char *str) {
    size_t len;
    char *copy;
    len = strlen(str) + 1;
    if(!(copy=cJSON_malloc(len))) return 0;
    memcpy(copy, str, len);
    return copy;
}

/* Internal constructor. */
cJSON *cJSON_New_Item(void) {
    cJSON *node = (cJSON*)cJSON_malloc(sizeof(cJSON));
    if(node)
        memset(node, 0, sizeof(cJSON));
    return node;
}

/* Delete a cJSON structure. */
void cJSON_Delete(cJSON *c) {
    cJSON *next;
    while(c) {
        next = c->next;
        cJSON_free(c);
        c = next;
    }
}


typedef struct {
    char *buffer;
    int length;
    int offset;
} printbuffer;

/* Parse the input text to generate a number, and populate the result into item. */



/* Render the number nicely from the given item into a string. */



/* Parse the input text into an unescaped cstring, and populate item. */



/* Render the cstring provided to an escaped version that can be printed. */

/* Invote print_string_ptr (which is useful) on an item. */

/* Predeclare these prototypes. */

/* Default options for cJSON_Parse */

/* Render a cJSON item/entity/structure to text. */
//char *cJSON_Print(cJSON *item) {return print_value(item, 0, 1, 0);}


/* Parser core - when encountering text, process appropriately. */


/* Render a value to text. */


/* Build an array from input text. */


/* Render an array to text */


/* Build an object from the text. */


/* Render an object to text. */


/* Get Array size/item / object item. */

/* Utility for array list handling. */
static void suffix_object(cJSON *prev, cJSON *item) {
    prev->next = item;
    item->prev = prev;
}
/* Utility for handling references. */

/* Add item to array/object. */
void cJSON_AddItemToArray(cJSON *array, cJSON *item) {
    cJSON *c = array->child;
    if(!item) return;
    if(!c)  // if c is null
        array->child=item;
    else {
        while(c && c->next)
            c=c->next;
        suffix_object(c, item);
    }
}

void cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item) {
    if(!item) return;
    if(item->string)
        cJSON_free(item->string);
    item->string = cJSON_strdup(string);
    cJSON_AddItemToArray(object, item);
}

/* Replace array/object items with new ones. */

/* Create basic types: */
cJSON *cJSON_CreateString(const char *string) {
    cJSON *item=cJSON_New_Item();
    if(item) {
        item->type=cJSON_String;
        item->valuestring=cJSON_strdup(string);
    }
    return item;
}
cJSON *cJSON_CreateObject(void) {
    cJSON *item = cJSON_New_Item();
    if(item)
        item->type = cJSON_Object;
    return item;
}


/* Create Arrays: */

/* Duplication */

