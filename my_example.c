/* https://blog.csdn.net/qq_27396861/article/details/54381576 */
#include <string.h>

#include <float.h>
#include <limits.h>

#include "cJSON.h"

char *json_buffer;

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

    //cJSON *unpacket_root;
    //char *pp;
    //pp = parse_number(unpacket_root, json_buffer);
    //printf("%s\n", pp);
    //printf("%f\n", unpacket_root->valuedouble);
    //printf("%f", unpacket_root->valuedouble);

    cJSON *strItem = cJSON_CreateString("Tom");
    printf("%s", strItem->valuestring);



//    char *s = 0;
//    printf("%ld\n", s);
//    if(s)
//        printf("\t%s\n", "This is True.");
//    else
//        printf("\t%s\n", "This is False.");
    //json_buffer = cJSON_Print(root);
    //printf("%s\n", json_buffer);
    //cJSON_Delete(root);
}

void str_packet() {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("Tomy\'s"));
    cJSON_AddItemToObject(root, "age", cJSON_CreateNumber(27));
    printf("The object of root: %d\n", root->type);
    printf("The name of the first child: %s -- %s\n", root->child->string, root->child->valuestring);
    //printf("The name of the second child: %s -- %d\n", root->child->next->string, root->child->next->valueint);

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


