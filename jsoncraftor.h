#ifdef JSONCRAFTOR_H
#define JSONCRAFTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef enum
{
    JSON_NULL,
    JSON_BOOLEAN,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
} json_type;

typedef struct json_object
{
    char *key;
    json_value *value;
    struct json_object *next;
} json_object;

typedef struct json_array
{
    json_value *value;
    struct json_array *next;
} json_array;

typedef struct json_value
{
    json_type type;
    union
    {
        bool boolean;
        double number;
        char *string;
        struct json_object *object;
        struct json_array *array;
    } value;

} json_value;

#ifdef JSONCRAFTOR_IMPLEMENTATION
#endif // JSONCRAFTOR_IMPLEMENTATION

#endif // JSONCRAFTOR_H