#define JSONCRAFTOR_IMPLEMENTATION

#include "../jsoncraftor.h"

int main()
{
    const char *str = "{\"coord\":{\"lon\":139.01,\"lat\":35.02},\"weather\":[{\"id\":800,\"main\":\"Clear\",\"description\":\"clear sky\",\"icon\":\"01n\"}],\"base\":\"stations\",\"main\":{\"temp\":285.514,\"pressure\":1013.75,\"humidity\":100,\"temp_min\":285.514,\"temp_max\":285.514,\"sea_level\":1023.22,\"grnd_level\":1013.75},\"wind\":{\"speed\":5.52,\"deg\":311},\"clouds\":{\"all\":0},\"dt\":1485792967,\"sys\":{\"message\":0.0025,\"country\":\"JP\",\"sunrise\":1485726240,\"sunset\":1485763863},\"id\":1907296,\"name\":\"Tawarano\",\"cod\":200, \"status\": true}";

    char value[100] = {0};

    json_value *root = parse_json(str);

    json_value *object = get_value(root, "weather", value);

    if (object && object->type == JSON_ARRAY)
    {
        json_value *inner_array = get_value(object, "0", value);
        if (inner_array && inner_array->type == JSON_OBJECT)
        {
            get_value(inner_array, "description", value);
            printf("Description: %s\n", value);
        }
    }

    object = get_value(root, "main", value);
    if (object && object->type == JSON_OBJECT)
    {
        get_value(object, "temp", value);
        printf("Value: %s\n", value);
    }

    object = get_value(root, "name", value);
    printf("Value: %s\n", value);

    object = get_value(root, "status", value);
    printf("Value: %s\n", value);
}
