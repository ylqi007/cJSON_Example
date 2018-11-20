/* https://blog.csdn.net/qq_27396861/article/details/54381576 */
#include <string.h>

#include "cJSON.h"

char *json_buffer;

void packet() {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "gender", cJSON_CreateNumber(1));
    cJSON_AddItemToObject(root, "age", cJSON_CreateNumber(17));
    cJSON_AddItemToObject(root, "name", cJSON_CreateNumber(18));
    printf("%s\n", root->child->string);
    printf("%d\n", root->child->valueint);
    printf("%.2f\n", root->child->valuedouble);

    printf("%s\n", root->child->next->string);
    printf("%d\n", root->child->next->valueint);
    printf("%.2f\n", root->child->next->valuedouble);

    printf("%s\n", root->child->next->next->string);
    printf("%d\n", root->child->next->next->valueint);
    printf("%.2f\n", root->child->next->next->valuedouble);
    printf("size of root %d\n", cJSON_GetArraySize(root));

    //json_buffer = cJSON_Print(root);
    //printf("%s\n", json_buffer);
    //cJSON_Delete(root);
}

void unpacket() {
    //cJSON *root = cJSON_Parse(json_buffer);

    //cJSON *name = cJSON_GetObjectItem(root, "name");
    //printf("name = %s\n", name->valuestring);

}
