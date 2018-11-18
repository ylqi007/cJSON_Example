#include "cJSON.h"

char *json_buffer;

void packet() {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("tom"));

    //json_buffer = cJSON_Print(root);
    printf("%s\n", json_buffer);
    cJSON_Delete(root);
}

void unpacket() {
    cJSON *root = cJSON_Parse(json_buffer);

    cJSON *name = cJSON_GetObjectItem(root, "name");
    printf("name = %s\n", name->valuestring);

}
