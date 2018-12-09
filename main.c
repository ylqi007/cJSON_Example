#include <stdio.h>
#include <stdlib.h>

#include "cJSON.h"
#include "my_example.h"

int main()
{
    printf("##### Hello World! #####\n\n");
    /* Create cJSON objects with different types. */
    cJSON_String_test();



    //cJSON_Object_test();
    //cJSON_Number_test();

    //print_number_test();

    /* Test for parse. */
    //str_packet();
    //unpacket();
    //sprintf_test();
    //DBL_EPSILON_test();
    return 0;
}
