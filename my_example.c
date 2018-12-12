/* https://blog.csdn.net/qq_27396861/article/details/54381576 */
#include <string.h>

#include <float.h>
#include <limits.h>
#include <stdio.h>

#include "cJSON.h"

char *json_buffer;

/* String test. */
void cJSON_String_test() {
    char *test_string = "Hello, Yunlong\\'s girl friend!";
    cJSON *str = cJSON_CreateString(test_string);
    printf("The type of str:\t\t%d\n", str->type);       /* In cJSON.h, #define cJSON_String 4 */
    printf("The next element of str:\t%p\n", (void*)str->next);
    printf("The prev element of str:\t%p\n", (void*)str->next);
    printf("The child element of str:\t%lu\n", (unsigned long int)str->child);
    printf("The type:\t\t\t%d\n", str->type);
    printf("The value of valutint:\t\t%d\n", str->valueint);
    printf("The value of valutdouble:\t%f\n", str->valuedouble);
    printf("The value of string:\t\t%s\n", str->string);
    printf("The value of valuestring:\t%s\n", str->valuestring);

    /* Test print_string(). */
    char* res = cJSON_Print(str);
    printf("\n%s\n\n", res);

    /* Test parse_string(). */
    cJSON *test = cJSON_CreateObject();
    parse_string(test, res);
    printf("The type of test:\t\t%d\n", test->type);       /* In cJSON.h, #define cJSON_String 4 */
    printf("The next element of test:\t%p\n", (void*)test->next);
    printf("The prev element of test:\t%p\n", (void*)test->next);
    printf("The child element of test:\t%lu\n", (unsigned long int)test->child);
    printf("The type:\t\t\t%d\n", test->type);
    printf("The value of valutint:\t\t%d\n", test->valueint);
    printf("The value of valutdouble:\t%f\n", test->valuedouble);
    printf("The value of string:\t\t%s\n", test->string);
    printf("The value of valuestring:\t%s\n", test->valuestring);

    printf("\nTest for cJSON_Parse():\n");
    cJSON *test1 = cJSON_Parse(res);
    printf("The type of test1:\t\t%d\n", test1->type);       /* In cJSON.h, #define cJSON_String 4 */
    printf("The next element of test1:\t%p\n", (void*)test1->next);
    printf("The prev element of test1:\t%p\n", (void*)test1->next);
    printf("The child element of test1:\t%lu\n", (unsigned long int)test1->child);
    printf("The type:\t\t\t%d\n", test1->type);
    printf("The value of valutint:\t\t%d\n", test1->valueint);
    printf("The value of valutdouble:\t%f\n", test1->valuedouble);
    printf("The value of string:\t\t%s\n", test1->string);
    printf("The value of valuestring:\t%s\n", test1->valuestring);
}


/* Boolean test. */
void cJSON_Boolean_test() {
    cJSON *b = cJSON_CreateBool(0);
    printf("%d\n", b->type);
    cJSON *bt = cJSON_CreateTrue();
    printf("%d\n", bt->type);
    cJSON *bf = cJSON_CreateFalse();
    printf("%d\n", bf->type);
    printf("The type of bf:\t\t%d\n", bf->type);       /* In cJSON.h, #define cJSON_String 4 */
    printf("The next element of bf:\t%p\n", (void*)bf->next);
    printf("The prev element of bf:\t%p\n", (void*)bf->next);
    printf("The child element of bf:\t%lu\n", (unsigned long int)bf->child);
    printf("The type:\t\t\t%d\n", bf->type);
    printf("The value of valutint:\t\t%d\n", bf->valueint);
    printf("The value of valutdouble:\t%f\n", bf->valuedouble);
    printf("The value of string:\t\t%s\n", bf->string);
    printf("The value of valuestring:\t%s\n", bf->valuestring);
}


/* Object test. */
void cJSON_Object_test() {
    cJSON *root = cJSON_CreateObject();
    printf("Type: %d\n", root->type);
    cJSON_AddItemToObject(root, "Name", cJSON_CreateString("Yunlong Qi"));
    printf("%s: %s\n", root->child->string, root->child->valuestring);
    cJSON_AddItemToObject(root, "New Name", cJSON_CreateString("Alex"));
    printf("%s: %s\n", root->child->next->string, root->child->next->valuestring);
    cJSON_AddItemToObject(root, "Her Name", cJSON_CreateString("Zhu"));
    printf("%s: %s\n", root->child->next->next->string, root->child->next->next->valuestring);
}





//void cJSON_Object_test() {
//    cJSON *root = cJSON_CreateObject();
//    printf("The type of root: %d\n", root->type);       /* In cJSON.h, #define cJSON_Object 6 */
//    printf("%p\t%p\t%p\n", root->next, root->prev, root->child);
//    printf("%d\t%d\t%f\n", root->type, root->valueint, root->valuedouble);
//    printf("%p\t%s\n", root->string, root->valuestring);
//    printf("The size of cJSON: %zu\n", sizeof(root));    /* root is a pointer pointing to a cJSON object, and it's size is 8 */
//    printf("The size of cJSON: %zu\n", sizeof(cJSON));   /* cJSON is an struct, 56 */
//}

void cJSON_Number_test() {
    cJSON *num = cJSON_CreateNumber(9);
    printf("The type of num: %d\n", num->type);       /* In cJSON.h, #define cJSON_Object 6 */
    printf("%p\t%p\t%p\n", num->next, num->prev, num->child);
    printf("%d\t%d\t%f\n", num->type, num->valueint, num->valuedouble);
    printf("%s\t%s\n", num->string, num->valuestring);
}

void print_number_test() {
    printf("%.20e\n", DBL_EPSILON);
    printf("The maximum value of integer: %u, the size of INT_MAX: %zu\n", INT_MAX, sizeof(INT_MIN));
    printf("The minimum value of integer: %u\n", INT_MIN);

    cJSON *num = cJSON_CreateNumber(9000000000.1);
    char *s = cJSON_Print(num);
    printf("%s\n", s);
}

//void packet() {
//    cJSON *root = cJSON_CreateObject();
//    cJSON_AddItemToObject(root, "gender", cJSON_CreateNumber(1));
//    cJSON_AddItemToObject(root, "age", cJSON_CreateNumber(17));
//    cJSON_AddItemToObject(root, "name", cJSON_CreateNumber(18));
//    printf("%s\n", root->child->string);
//    printf("%d\n", root->child->valueint);
//    printf("%.2f\n", root->child->valuedouble);
//
//    printf("%s\n", root->child->next->string);
//    printf("%d\n", root->child->next->valueint);
//    printf("%.2f\n", root->child->next->valuedouble);
//
//    printf("%s\n", root->child->next->next->string);
//    printf("%d\n", root->child->next->next->valueint);
//    printf("%.2f\n", root->child->next->next->valuedouble);
//    printf("size of root %d\n", cJSON_GetArraySize(root));
//
//    cJSON *num = cJSON_CreateNumber(9.01);
//    printbuffer *p = 0;
    //json_buffer = print_number(num, p);
//    json_buffer = cJSON_Print(num);
//    printf("The value of jsonbuffer: %s\n", json_buffer);

//    cJSON *strItem = cJSON_CreateString("Tom");
//    printf("%s", strItem->valuestring);
//}

//void str_packet() {
//    cJSON *root = cJSON_CreateObject();
//    cJSON_AddItemToObject(root, "name", cJSON_CreateString("Tomy\'s"));
//    cJSON_AddItemToObject(root, "age", cJSON_CreateNumber(27));
//    printf("The object of root: %d\n", root->type);
//    printf("The name of the first child: %s -- %s\n", root->child->string, root->child->valuestring);
//    printf("The name of the second child: %s -- %d\n", root->child->next->string, root->child->next->valueint);
//
//    //json_buffer = cJSON_Print(cJSON_CreateString("Yunlong\"Qi\"\t\\"));
//    printf("####################################\n");
//    cJSON *r1 = cJSON_CreateObject();
//    json_buffer = cJSON_Print(root);
//    printf("%s", json_buffer);
//}

//int sprintf_test() {
//    int sal;
//    char name[20];
//    char designation[30];
//    char info[60];
//
//    printf("Enter your name:\n");
//    gets(name);
//
//    printf("Enter your designation:\n");
//    gets(designation);
//
//    printf("Enter your salary:\n");
//    scanf("%d", &sal);
//
//    sprintf(info, "Welcome %s !\nName: %s \nDesignation: %s\nSalary: %d", name, name, designation, sal);
//    printf("\n\t%s\n", info);
//}

//void DBL_EPSILON_test() {
//    printf("The maximum value of float = %.10e\n", FLT_MAX);
//    printf("The minimum value of float = %.10e\n", FLT_MIN);
//
//    printf("The number of digits in the number = %.10e\n", FLT_MANT_DIG);
//    printf("The number DBL_EPSILON = %.34e\n", DBL_EPSILON);
//
//    printf("The value INT_MAX = %d\n", INT_MAX);
//    printf("The value INT_MIN = %d\n", INT_MIN);
//}


