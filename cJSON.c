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

static const char *ep;


static void *(*cJSON_malloc)(size_t sz) = malloc;
static void (*cJSON_free)(void *ptr) = free;

static char *cJSON_strdup(const char *str) {
    size_t len;
    char *copy;

    len = strlen(str) + 1;  /* The strlen() returns the number of characters that precede the terminating null character. */
    if(!(copy=(char*)cJSON_malloc(len)))
        return 0;
    memcpy(copy, str, len);
    return copy;
}

/* Internal constructor. */
static cJSON *cJSON_New_Item(void) {
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
        if(!(c->type & cJSON_IsReference) && c->child) cJSON_Delete(c->child);
        if(!(c->type & cJSON_IsReference) && c->valuestring) cJSON_free(c->valuestring);
        if(!(c->type & cJSON_StringIsConst) && c->string) cJSON_free(c->string);
        cJSON_free(c);
        c = next;
    }
}

/* Parse the input text to generate a number, and populate the result into item. */

static int pow2gt(int x)    {   --x;    x|=x>>1;    x|=x>>2;    x|=x>>4;    x|=x>>8;    x|=x>>16;   return x+1;}

static char *ensure(printbuffer *p, int needed) {
    char *newbuffer;
    int newsize;
    if(!p || !p->buffer)
        return 0;
    needed += p->offset;
    if(needed <= p->length)
        return p->buffer + p->offset;   // There are enough free spaces in printbuffer p
    newsize = pow2gt(needed);
    newbuffer=(char*)cJSON_malloc(newsize);
    if(!newbuffer) {
        cJSON_free(p->buffer);
        p->length=0;
        p->buffer=0;
        return 0;
    }   // ? why free printbuffer p ? ==> newsize==0?
    if(newbuffer)
        memcpy(newbuffer, p->buffer, p->length);
    cJSON_free(p->buffer);
    p->length = newsize;
    p->buffer = newbuffer;
    return newbuffer + p->offset;
}

static int update(printbuffer *p) {
    char *str;
    if(!p || !p->buffer) return 0;
    str=p->buffer + p->offset;
    return p->offset + strlen(str);
}

/* Render the number nicely from the given item into a string. */

/* Render the number nicely from the given item into a string. */
static char *print_number(cJSON *item, printbuffer *p) {
    char *str = 0;
    double d = item->valuedouble;   /* The double value of this item. */
    if(d==0) {  /* The special case for 0. */
        if(p)   str = ensure(p, 2);
        else    str = (char*)cJSON_malloc(2);
        if(str) strcpy(str, "0");
    } else if(fabs(((double)item->valueint)-d)<=DBL_EPSILON && d<=INT_MAX && d>=INT_MIN) {
        if(p)   str = ensure(p, 21);    /* 2^64+1 can be represented in 21 digits. log(10)2^64=20 */
        else    str = (char*)cJSON_malloc(21);
        if(str) sprintf(str, "%d", item->valueint);
    } else {
        if(p)   str = ensure(p, 64);    /* This is a nice tradeoff. I don't know. */
        else    str = (char*)cJSON_malloc(64);
        if(str) {
            if(fabs(floor(d)-d)<=DBL_EPSILON && fabs(d)<1.0e60)
                sprintf(str, "%.0f", d);
            else if(fabs(d)<1.0e-6 || fabs(d)>1.0e9)
                sprintf(str, "%e", d);
            else
                sprintf(str, "%f", d);
        }
    }
    return str;
}

/* Parse the input text to generate a number, and populate the result into item. */
const char *parse_number(cJSON *item, const char *num) {
    double n=0, sign=1, scale=0;
    int subscale=0, signsubscale=1;

    if(*num=='-') sign=-1, num++;   /* Has sign? */
    if(*num=='0') num++;            /* Is zero? */
    if(*num>='1' && *num<='9') {
        do {
            n = (n*10.0) + (*num++ - '0');
        } while(*num>='0' && *num<='9');    /* Number */
    }
    if(*num=='.' && num[1]>='0' && num[1]<='9') {
        num++;
        do {
            n = (n*10.0) + (*num++ - '0');
            scale--;
        } while(*num>=0 && *num<='9');  /*Fractional part. */
    }
    if(*num=='e' || *num=='E') {
        num++;
        if(*num=='+')
            num++;
        else if(*num=='-') {
            signsubscale = -1;
            num++;      /* With sign. */
        }
        while(*num>='0' && *num<='9') {
            subscale = (subscale*10) + (*num++ - '0');  /* scale number. */
        }
    }
    n = sign*n*pow(10.0, (scale+subscale*signsubscale));    /* number = +/- num.fraction ( 10^(+/- exponent) */
    item->valuedouble = n;
    printf("The number of n: %f\n", n);
    item->valueint = (int)n;
    item->type = cJSON_Number;

    return num;
}

static unsigned parse_hex4(const char *str) {
    unsigned h = 0;
    if(*str>='0' && *str<='9') h+=(*str)-'0'; else if(*str>='A' && *str<='F') h+=10+(*str)-'A'; else if(*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
    h=h<<4; str++;
    if(*str>='0' && *str<='9') h+=(*str)-'0'; else if(*str>='A' && *str<='F') h+=10+(*str)-'A'; else if(*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
    h=h<<4; str++;
    if(*str>='0' && *str<='9') h+=(*str)-'0'; else if(*str>='A' && *str<='F') h+=10+(*str)-'A'; else if(*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
    h=h<<4; str++;
    if(*str>='0' && *str<='9') h+=(*str)-'0'; else if(*str>='A' && *str<='F') h+=10+(*str)-'A'; else if(*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
    return h;
}

/* Parse the input text into an unescaped cstring, and populate item. */
static const unsigned char firstByteMark[7] = {0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC};
const char *parse_string(cJSON *item, const char *str) {
    const char *ptr = str + 1;
    char *ptr2, *out;
    int len=0;
    unsigned uc, uc2;
    if(*str != '\"') {ep = str; return 0;}  /* Not a string. */

    while(*ptr!='\"' && *ptr && ++len) if(*ptr++ == '\\') ptr++;    /* Skip escaped quotes. */

    out = (char*)cJSON_malloc(len+1);   /* This is how long we need for he string, roughly. */
    if(!out) return 0;

    ptr=str+1; ptr2=out;
    while(*ptr!='\"' && *ptr) {
        if(*ptr!='\\')
            *ptr2++=*ptr++;
        else {
            ptr++;
            switch(*ptr) {
                case 'b':   *ptr2++='\b';   break;
                case 'f':   *ptr2++='\f';   break;
                case 'n':   *ptr2++='\n';   break;
                case 'r':   *ptr2++='\r';	break;
				case 't':   *ptr2++='\t';	break;
				case 'u':           /* transcode utf16 to utf8. */
				    uc=parse_hex4(ptr+1); ptr+=4;   /* Get the unicode. */

				    if((uc>=0xDC00 && uc<=0xDFFF) || uc==0) break;  /* check for invalid. */

				    if(uc>=0xD800 && uc<=0xDBFF) {  /* UTF16 surrogate pairs. */
                        if(ptr[1]!='\\' || ptr[2]!='u') break;  /* Missing second-half of surrogate. */
                        uc2 = parse_hex4(ptr+3); ptr+=6;
                        if(uc2<0xDC00 || uc2>0xDFFF)    break;  /* invalid second-half of surrogate. */
                        uc=0x10000 + (((uc & 0x3FF)<<10) | (uc2&0x3FF));
				    }

				    len = 4;
				    if(uc<0x80)
                        len = 1;
                    else if(uc<0x800)
                        len = 8;
                    else if(uc<0x10000)
                        len = 3;
                    ptr2+=len;

                    switch(len) {
                        case 4: *--ptr2 =((uc | 0x80) & 0xBF); uc >>= 6;
                        case 3: *--ptr2 =((uc | 0x80) & 0xBF); uc >>= 6;
						case 2: *--ptr2 =((uc | 0x80) & 0xBF); uc >>= 6;
						case 1: *--ptr2 =(uc | firstByteMark[len]);
                    }
                    ptr2+=len;
                    break;
                default: *ptr2++=*ptr;  break;
            }
            ptr++;
        }
    }
    *ptr2=0;
    if(*ptr=='\"') ptr++;
    item->valuestring = out;
    item->type=cJSON_String;
    return ptr;
}

/* Render the cstring provided to an escaped version that can be printed. */
static char *print_string_ptr(const char *str, printbuffer *p) {
    const char *ptr;    // This pointer won't change the content.
    char *ptr2, *out;
    int len=0, flag=0;
    unsigned char token;

    for(ptr=str; *ptr; ptr++) {flag|=((*ptr>0 && *ptr<32) || (*ptr=='\"') || (*ptr=='\\')) ? 1 : 0;}
    if(!flag) {
        len = ptr - str;
        if(p)   out = ensure(p, len+3);   // Two for "" and one for \0
        else    out = (char*)cJSON_malloc(len+3);
        if(!out) return 0;
        ptr2=out; *ptr2++='\"';
        strcpy(ptr2, str);
        ptr2[len]='\"';
        ptr2[len+1]=0;
        return out;
    }
    if(!str) {  // empty string
        if(p)   out=ensure(p, 3);
        else    out=cJSON_malloc(3);
        if(!out) return 0;
        strcpy(out, "\"\"");
        return out;
    }
    ptr=str; while((token=*ptr) && ++len) {if(strchr("\"\\\b\f\n\r\t", token)) len++; else if(token<32) len+=5; ptr++;}

    if(p)   out=ensure(p, len+3);
    else    out=(char*)cJSON_malloc(len+3);
    if(!out) return 0;

    ptr2=out; ptr=str;
    *ptr2++='\"';
    while(*ptr) {
        if((unsigned char)*ptr>31 && *ptr!='\"' && *ptr!='\\') *ptr2++=*ptr++;
        else {
            *ptr2++='\\';
            switch(token=*ptr++) {
				case '\\':	*ptr2++='\\';	break;
				case '\"':	*ptr2++='\"';	break;
				case '\b':	*ptr2++='b';	break;
				case '\f':	*ptr2++='f';	break;
				case '\n':	*ptr2++='n';	break;
				case '\r':	*ptr2++='r';	break;
				case '\t':	*ptr2++='t';	break;
				default: sprintf(ptr2,"u%04x",token);ptr2+=5;	break;	/* escape and print */
            }
        }
    }
    *ptr2++='\"'; *ptr2++=0;
    return out;
}
/* Call print_string_ptr (which is useful) on an item. */
static char *print_string(cJSON *item, printbuffer *p) { return print_string_ptr(item->valuestring, p);}

/* Predeclare these prototypes. */
static const char *parse_value(cJSON *item, const char *value);
static char *print_value(cJSON *item,int depth,int fmt,printbuffer *p);

static char *print_object(cJSON *item, int depth, int fmt, printbuffer *p);

/* Utility to jump whitespace and cr/lf */
static const char *skip(const char *in) {while (in && *in && (unsigned char)*in<=32) in++; return in;}

/* Parse an object - Create a new root and populate. */
cJSON *cJSON_ParseWithOpts(const char *value, const char **return_parse_end, int require_null_terminated) {
    const char *end = 0;
    cJSON *c = cJSON_New_Item();
    ep = 0;
    if(!c) return 0;    /* Memory fail. */

    end = parse_value(c, skip(value));
    if(!end) {cJSON_Delete(c); return 0;}   /* Parse failure. ep is set. */

    /* If we require null-terminated JSON without appended garbage, skip and then check for a null terminator. */
    if(require_null_terminated) {end=skip(end); if(*end) {cJSON_Delete(c); ep=end; return 0;}}
    if(return_parse_end) *return_parse_end=end;
    return c;
}
/* Default options for cJSON_Parse. */
cJSON *cJSON_Parse(const char *value) {return cJSON_ParseWithOpts(value, 0, 0);}

/* Render a cJSON item/entity/structure to text. */
char *cJSON_Print(cJSON *item)              {return print_value(item, 0, 1, 0);}
char *cJSON_PrintUnformatted(cJSON *item)	{return print_value(item,0,0,0);}

/* Parse core - When encountering text, process appropriately. */
static const char *parse_value(cJSON *item, const char *value) {
    if(!value)  return 0;   /* Fail on null. */
    if(*value=='\"')    {return parse_string(item, value);}

    ep=value; return 0; /* Failure. */
}

/* Render a value to text. */
static char *print_value(cJSON *item, int depth, int fmt, printbuffer *p) {
    char *out = 0;
    if(!item) return 0;
    if(p) {
        switch((item->type)&255) {
            case cJSON_Number:  out = print_number(item, p); break;
            case cJSON_String:  out = print_string(item, p); break;
            case cJSON_Object:  out = print_object(item, depth, fmt, p); break;
        }
    } else {
        switch((item->type)&255) {
            case cJSON_Number:  out = print_number(item, 0); break;
            case cJSON_String:  out = print_string(item, 0); break;
            case cJSON_Object:  out = print_object(item, depth, fmt, 0); break;
        }
    }
    return out;
}

/* Build an object from the text. */

/* Render an object to text. */
static char *print_object(cJSON *item, int depth, int fmt, printbuffer *p) {
    char **entries=0, **names=0;
    char *out=0, *ptr, *ret, *str;
    int len=7, i=0, j;
    cJSON *child = item->child;
    int numentries=0, fail=0;
    size_t tmplen=0;
    /* Count the number of entries. */
    while(child) {numentries++; child=child->next;}
    /* Explicitly handle empty object case. */
    if(!numentries) {   // No child.
        if(p)   out = ensure(p, fmt?depth+4:3);
        else    out=(char*)cJSON_malloc(fmt?depth+4:3);
        if(!out) return 0;
        ptr=out;
        *ptr++='{';
        if(fmt) {*ptr++=' '; for(i=0; i<depth-1; i++) *ptr++='\t';}
        *ptr++='}';
        *ptr++=0;
        return out;
    }
    /* numentries!=0 */
    if(p) {
        /* Compose the output. */
        i=p->offset;
        len=fmt?2:1;    ptr=ensure(p, len+1);   if(!ptr) return 0;
        *ptr++='{'; if(fmt) *ptr++='\n';    *ptr=0; p->offset+=len;
        child=item->child; depth++;
        while(child) {
            if(fmt) {
                ptr=ensure(p,depth);	if (!ptr) return 0;
				for (j=0;j<depth;j++) *ptr++='\t';
				p->offset+=depth;
            }
            print_string_ptr(child->string,p);
			p->offset=update(p);

			len=fmt?2:1;
			ptr=ensure(p,len);	if (!ptr) return 0;
			*ptr++=':';if (fmt) *ptr++='\t';
			p->offset+=len;

			print_value(child,depth,fmt,p);
			p->offset=update(p);

			len=(fmt?1:0)+(child->next?1:0);
			ptr=ensure(p,len+1); if (!ptr) return 0;
			if (child->next) *ptr++=',';
			if (fmt) *ptr++='\n';*ptr=0;
			p->offset+=len;
			child=child->next;
        }
        ptr=ensure(p,fmt?(depth+1):2);	 if (!ptr) return 0;
		if (fmt)	for (i=0;i<depth-1;i++) *ptr++='\t';
		*ptr++='}';*ptr=0;
		out=(p->buffer)+i;
    } else {
        /* Allocate space for the names and the objects. */
        entries=(char**)cJSON_malloc(numentries*sizeof(char*));
        if(!entries) return 0;
        names=(char**)cJSON_malloc(numentries*sizeof(char*));
        if(!names) {cJSON_free(entries); return 0;}
        memset(entries, 0, numentries*sizeof(char*));
        memset(names, 0, numentries*sizeof(char*));

        /* Collect all the results into our arrays. */
        child=item->child; depth++; if(fmt) len+=depth;
        while(child) {
            names[i]=str=print_string_ptr(child->string, 0);
//            printf("* %s\t", names[i]);
            entries[i++]=ret=print_value(child, depth, fmt, 0);
//            printf("%s\n", entries[i-1]);
            if(str && ret) len+=strlen(ret)+strlen(str)+2+(fmt?2+depth:0); else fail=1;
            child=child->next;
        }

        /* Try to allocate the output string. */
        if(!fail) out=(char*)cJSON_malloc(len);
        if(!out) fail=1;

        /* Handle failure */
        if(fail) {
            for(i=0; i<numentries; i++) {if(names[i]) cJSON_free(names[i]); if(entries[i]) cJSON_free(entries[i]);}
            cJSON_free(names); cJSON_free(entries);
            return 0;
        }

        /* Compose the output. */
        *out='{'; ptr=out+1; if(fmt) *ptr++='\n'; *ptr=0;
        for(i=0; i<numentries; i++) {
            if(fmt) for(j=0; j<depth; j++) *ptr++='\t';
            tmplen=strlen(names[i]); memcpy(ptr, names[i], tmplen); ptr+=tmplen;
            *ptr++=':'; if(fmt) *ptr++='\t';
            strcpy(ptr, entries[i]); ptr+=strlen(entries[i]);
            if(i!=numentries-1) *ptr++=',';
            if(fmt) *ptr++='\n'; *ptr=0;
            cJSON_free(names[i]); cJSON_free(entries[i]);
        }
        cJSON_free(names); cJSON_free(entries);
        if(fmt) for(i=0; i<depth-1; i++) *ptr++='\t';
        *ptr++='}';*ptr++=0;
    }
    return out;
}


/* Get Array size/item / object item. */
int cJSON_GetArraySize(cJSON *array) {
    cJSON *c = array->child;
    int i = 0;
    while(c) {
        i++;
        c = c->next;
    }
    return i;
}

/* Utility for array list handling. */
static void suffix_object(cJSON *prev, cJSON *item) {
    prev->next=item;
    item->prev=prev;
}

/* Utility for handling references. */

/* Add item to array/object. */
void cJSON_AddItemToArray(cJSON *array, cJSON *item) {
    cJSON *c = array->child;
    if(!item)
        return;
    if(!c)  // if array->child is NULL
        array->child = item;
    else {
        while(c && c->next)
            c = c->next;
        suffix_object(c, item); // for now, c refers to the last element.
    }
}

/* Add item to object. */
void cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item) { if(!item) return;if(item->string)cJSON_free(item->string);item->string=cJSON_strdup(string);cJSON_AddItemToArray(object, item);}

/* Replace array/object items with new ones. */
cJSON *cJSON_CreateNumber(double num)           {cJSON *item=cJSON_New_Item(); if(item){item->type=cJSON_Number;item->valuedouble=num;item->valueint=(int)num;} return item;}
cJSON *cJSON_CreateString(const char *string)   {cJSON *item=cJSON_New_Item(); if(item){item->type=cJSON_String;item->valuestring=cJSON_strdup(string);} return item;}
cJSON *cJSON_CreateTrue(void)                   {cJSON *item=cJSON_New_Item(); if(item)item->type=cJSON_True; return item;}
cJSON *cJSON_CreateFalse(void)                  {cJSON *item=cJSON_New_Item(); if(item)item->type=cJSON_False; return item;}
cJSON *cJSON_CreateBool(int b)                  {cJSON *item=cJSON_New_Item(); if(item)item->type=b?cJSON_True:cJSON_False; return item;}
cJSON *cJSON_CreateObject(void)                 {cJSON *item = cJSON_New_Item(); if(item)item->type = cJSON_Object;return item;}    //#define cJSON_Object 6

