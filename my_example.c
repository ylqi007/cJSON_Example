/* https://blog.csdn.net/qq_27396861/article/details/54381576 */
#include <string.h>

#include <float.h>
#include <limits.h>

#include "cJSON.h"

char *json_buffer;

void cJSON_Object_test() {
    cJSON *root = cJSON_CreateObject();
    printf("The type of root: %d\n", root->type);       /* In cJSON.h, #define cJSON_Object 6 */
    printf("%d\t%d\t%d\n", root->next, root->prev, root->child);
    printf("%d\t%d\t%f\n", root->type, root->valueint, root->valuedouble);
    printf("%d\t%d\n", root->string, root->valuestring);
    printf("The size of cJSON: %d\n", sizeof(root));    /* root is a pointer pointing to a cJSON object, and it's size is 8 */
    printf("The size of cJSON: %d\n", sizeof(cJSON));   /* cJSON is an struct, 56 */
}

void cJSON_Number_test() {
    cJSON *num = cJSON_CreateNumber(9);
    printf("The type of num: %d\n", num->type);       /* In cJSON.h, #define cJSON_Object 6 */
    printf("%d\t%d\t%d\n", num->next, num->prev, num->child);
    printf("%d\t%d\t%f\n", num->type, num->valueint, num->valuedouble);
    printf("%d\t%d\n", num->string, num->valuestring);
}

void cJSON_String_test() {
    char *test_string = "Hello, Yunlong.";
    cJSON *str = cJSON_CreateString(test_string);
    printf("The type of str: %d\n", str->type);       /* In cJSON.h, #define cJSON_Object 6 */
    printf("%d\t%d\t%d\n", str->next, str->prev, str->child);
    printf("%d\t%d\t%f\n", str->type, str->valueint, str->valuedouble);
    printf("%d\t%s\n", str->string, str->valuestring);
}

void print_number_test() {
    printf("%.20e\n", DBL_EPSILON);
    printf("The maximum value of integer: %d, the size of INT_MAX: %d\n", INT_MAX, sizeof(INT_MAX));
    printf("The minimum value of integer: %d\n", INT_MIN);

    cJSON *num = cJSON_CreateNumber(9.1);
    char *s = cJSON_Print(num);
    printf("%s\n", s);
}

void packet() {
    cJSON *root = cJSON_CreateObject();
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

    cJSON *strItem = cJSON_CreateString("Tom");
    printf("%s", strItem->valuestring);
}

void str_packet() {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("Tomy\'s"));
    cJSON_AddItemToObject(root, "age", cJSON_CreateNumber(27));
    printf("The object of root: %d\n", root->type);
    printf("The name of the first child: %s -- %s\n", root->child->string, root->child->valuestring);
    printf("The name of the second child: %s -- %d\n", root->child->next->string, root->child->next->valueint);

    //json_buffer = cJSON_Print(cJSON_CreateString("Yunlong\"Qi\"\t\\"));
    printf("####################################\n");
    cJSON *r1 = cJSON_CreateObject();
    json_buffer = cJSON_Print(root);
    printf("%s", json_buffer);
}

void unpacket() {
    //cJSON *root = cJSON_Parse(json_buffer);

    //cJSON *name = cJSON_GetObjectItem(root, "name");
    //printf("name = %s\n", name->valuestring);

}

int sprintf_test() {
    int sal;
    char name[20];
    char designation[30];
    char info[60];

    printf("Enter your name:\n");
    gets(name);

    printf("Enter your designation:\n");
    gets(designation);

    printf("Enter your salary:\n");
    scanf("%d", &sal);

    sprintf(info, "Welcome %s !\nName: %s \nDesignation: %s\nSalary: %d", name, name, designation, sal);
    printf("\n\t%s\n", info);
}

void DBL_EPSILON_test() {
    printf("The maximum value of float = %.10e\n", FLT_MAX);
    printf("The minimum value of float = %.10e\n", FLT_MIN);

    printf("The number of digits in the number = %.10e\n", FLT_MANT_DIG);
    printf("The number DBL_EPSILON = %.34e\n", DBL_EPSILON);

    printf("The value INT_MAX = %d\n", INT_MAX);
    printf("The value INT_MIN = %d\n", INT_MIN);
}


