#ifndef JSONCRAFTOR_H
#define JSONCRAFTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct json_value json_value;
typedef struct json_object json_object;
typedef struct json_array json_array;

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

json_value *create_json_value(json_type type);
json_object *create_json_object(const char *key, json_value *value);
json_array *create_json_array(json_value *value);
char *parse_json_string(const char **str);
double parse_json_number(const char **str);
json_object *parse_json_object(const char **str);
json_array *parse_json_array(const char **str);
json_value *parse_json_value(const char **str);
json_value *parse_json(const char *str);
char *json_object_to_string(json_object *obj);
char *json_array_to_string(json_array *arr);
char *to_string(json_value *value);

#ifdef JSONCRAFTOR_IMPLEMENTATION

json_value *create_json_value(json_type type)
{
    json_value *value = (json_value *)malloc(sizeof(json_value));
    if (value == NULL)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    value->type = type;
    return value;
}

json_object *create_json_object(const char *key, json_value *value)
{
    json_object *obj = (json_object *)malloc(sizeof(json_object));
    if (obj == NULL)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    obj->key = strdup(key);
    obj->value = value;
    obj->next = NULL;
    return obj;
}

json_array *create_json_array(json_value *value)
{
    json_array *arr = (json_array *)malloc(sizeof(json_array));
    if (arr == NULL)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    arr->value = value;
    arr->next = NULL;
    return arr;
}

char *parse_json_string(const char **str)
{
    if (**str == '"')
    {
        (*str)++;
    }

    const char *start = *str;

    while (**str != '"' && **str != '\0')
    {
        if (**str == '\\')
        {
            (*str)++;
            if (**str == '"' || **str == '\\')
            {
                (*str)++;
            }
            else
            {
                (*str)++;
            }
        }
        else
        {
            (*str)++;
        }
    }

    size_t length = *str - start;

    char *result = (char *)malloc(length + 1);
    if (!result)
    {
        perror("Memory allocation failed");
        return NULL;
    }

    strncpy(result, start, length);
    result[length] = '\0';

    if (**str == '"')
    {
        (*str)++;
    }
    return result;
}

double parse_json_number(const char **str)
{
    char *end;
    double number = strtod(*str, &end);
    *str = end;
    return number;
}

json_object *parse_json_object(const char **str)
{
    json_object *head = NULL, *tail = NULL;

    while (**str != '}')
    {
        while (isspace(**str))
            (*str)++;

        if (**str == '"')
        {
            (*str)++;
            char *key = parse_json_string(str);

            while (isspace(**str))
                (*str)++;
            if (**str == ':')
                (*str)++;

            json_value *value = parse_json_value(str);

            json_object *new_obj = create_json_object(key, value);
            if (head == NULL)
            {
                head = new_obj;
                tail = head;
            }
            else
            {
                tail->next = new_obj;
                tail = tail->next;
            }

            free(key);
        }

        while (isspace(**str))
            (*str)++;
        if (**str == ',')
            (*str)++;
    }

    (*str)++;
    return head;
}

json_array *parse_json_array(const char **str)
{
    json_array *head = NULL, *tail = NULL;

    while (**str != ']')
    {
        while (isspace(**str))
            (*str)++;

        json_value *value = parse_json_value(str);

        json_array *new_elem = create_json_array(value);
        if (head == NULL)
        {
            head = new_elem;
            tail = head;
        }
        else
        {
            tail->next = new_elem;
            tail = tail->next;
        }

        while (isspace(**str))
            (*str)++;
        if (**str == ',')
            (*str)++;
    }

    (*str)++;
    return head;
}

json_value *parse_json_value(const char **str)
{
    json_value *value = NULL;

    while (isspace(**str))
        (*str)++;

    if (**str == '"')
    {
        value = create_json_value(JSON_STRING);
        value->value.string = parse_json_string(str);
    }
    else if (isdigit(**str) || **str == '-')
    {
        value = create_json_value(JSON_NUMBER);
        value->value.number = parse_json_number(str);
    }
    else if (**str == '{')
    {
        (*str)++;
        value = create_json_value(JSON_OBJECT);
        value->value.object = parse_json_object(str);
    }
    else if (**str == '[')
    {
        (*str)++;
        value = create_json_value(JSON_ARRAY);
        value->value.array = parse_json_array(str);
    }
    else if (**str == 't' || **str == 'f')
    {
        value = create_json_value(JSON_BOOLEAN);
        if (strncmp(*str, "true", 4) == 0)
        {
            value->value.boolean = 1;
            *str += 4;
        }
        else if (strncmp(*str, "false", 5) == 0)
        {
            value->value.boolean = 0;
            *str += 5;
        }
    }
    else if (**str == 'n')
    {
        value = create_json_value(JSON_NULL);
        *str += 4;
    }

    return value;
}

json_value *parse_json(const char *str)
{
    return parse_json_value(&str);
}

void get_type(json_value *root, const char *key, char *type)
{
    if (root == NULL || root->type != JSON_OBJECT)
    {
        printf("Invalid root object.\n");
        return;
    }

    json_object *obj = root->value.object;
    while (obj != NULL)
    {
        if (strcmp(obj->key, key) == 0)
        {
            switch (obj->value->type)
            {
            case JSON_STRING:
                strcpy(type, "string");
                return;

            case JSON_NUMBER:
                strcpy(type, "number");
                return;

            case JSON_BOOLEAN:
                strcpy(type, "boolean");
                return;

            case JSON_OBJECT:
                strcpy(type, "object");
                return;

            case JSON_ARRAY:
                strcpy(type, "array");
                return;

            case JSON_NULL:
                strcpy(type, "null");
                return;

            default:
                strcpy(type, "Unknown type");
                return;
            }
        }

        obj = obj->next;
    }

    perror("Key not found");
}

json_value* get_value(json_value *root, const char *key, char *value)
{
    if (root == NULL)
    {
        printf("Invalid root object.\n");
        return NULL;
    }

    if (root->type == JSON_OBJECT)
    {
        json_object *obj = root->value.object;
        while (obj != NULL)
        {
            if (strcmp(obj->key, key) == 0)
            {
                switch (obj->value->type)
                {
                case JSON_STRING:
                    strcpy(value, obj->value->value.string);
                    return NULL;

                case JSON_NUMBER:
                    sprintf(value, "%f", obj->value->value.number);
                    return NULL;

                case JSON_BOOLEAN:
                    sprintf(value, "%s", obj->value->value.boolean ? "true" : "false");
                    return NULL;

                case JSON_OBJECT:
                    return obj->value;

                case JSON_ARRAY:
                    return obj->value;

                case JSON_NULL:
                    strcpy(value, "null");
                    return NULL;

                default:
                    strcpy(value, "Unknown type");
                    return NULL;
                }
            }

            obj = obj->next;
        }

        perror("Key not found in object");
        return NULL;
    }
    else if (root->type == JSON_ARRAY)
    {
        int index = atoi(key);
        json_array *arr = root->value.array;
        int i = 0;

        while (arr != NULL)
        {
            if (i == index)
            {
                switch (arr->value->type)
                {
                case JSON_STRING:
                    strcpy(value, arr->value->value.string);
                    return NULL;

                case JSON_NUMBER:
                    sprintf(value, "%f", arr->value->value.number);
                    return NULL;

                case JSON_BOOLEAN:
                    sprintf(value, "%s", arr->value->value.boolean ? "true" : "false");
                    return NULL;

                case JSON_OBJECT:
                    return arr->value;

                case JSON_ARRAY:
                    return arr->value;

                case JSON_NULL:
                    strcpy(value, "null");
                    return NULL;

                default:
                    strcpy(value, "Unknown type");
                    return NULL;
                }
            }

            i++;
            arr = arr->next;
        }

        perror("Index out of bounds in array");
        return NULL;
    }

    perror("Root is neither an object nor an array");
    return NULL;
}

char *to_string(json_value *value) {
    if (value == NULL) {
        return strdup("null");
    }

    char *result = NULL;

    switch (value->type) {
        case JSON_STRING:
            {
                // Escape the string properly
                size_t length = strlen(value->value.string);
                result = (char *)malloc(length * 2 + 3);  // Escaping for quotes and backslashes
                size_t j = 0;
                result[j++] = '"';  // Opening quote
                for (size_t i = 0; i < length; i++) {
                    if (value->value.string[i] == '\\' || value->value.string[i] == '"') {
                        result[j++] = '\\';
                    }
                    result[j++] = value->value.string[i];
                }
                result[j++] = '"';  // Closing quote
                result[j] = '\0';
            }
            break;

        case JSON_NUMBER:
            result = (char *)malloc(50);  // Buffer size for number
            snprintf(result, 50, "%f", value->value.number);
            break;

        case JSON_BOOLEAN:
            result = strdup(value->value.boolean ? "true" : "false");
            break;

        case JSON_NULL:
            result = strdup("null");
            break;

        case JSON_OBJECT:
            result = json_object_to_string(value->value.object);
            break;

        case JSON_ARRAY:
            result = json_array_to_string(value->value.array);
            break;

        default:
            result = strdup("Unknown type");
    }

    return result;
}

char *json_object_to_string(json_object *obj) {
    if (obj == NULL) {
        return strdup("{}");
    }

    size_t buffer_size = 1024;
    char *result = (char *)malloc(buffer_size);
    result[0] = '{';

    size_t pos = 1;
    json_object *current = obj;
    while (current != NULL) {
        size_t key_len = strlen(current->key);
        char *value_str = to_string(current->value);
        size_t value_len = strlen(value_str);
        size_t needed_size = pos + key_len + value_len + 4;

        if (needed_size > buffer_size) {
            buffer_size = needed_size * 2;
            result = (char *)realloc(result, buffer_size);
        }

        pos += snprintf(result + pos, buffer_size - pos, "\"%s\":%s", current->key, value_str);

        current = current->next;
        if (current != NULL) {
            result[pos++] = ',';
        }

        free(value_str);
    }

    result[pos++] = '}';
    result[pos] = '\0';

    return result;
}

char *json_array_to_string(json_array *arr) {
    if (arr == NULL) {
        return strdup("[]");
    }

    size_t buffer_size = 1024;
    char *result = (char *)malloc(buffer_size);
    result[0] = '[';

    size_t pos = 1;
    json_array *current = arr;
    while (current != NULL) {
        char *value_str = to_string(current->value);
        size_t value_len = strlen(value_str);
        size_t needed_size = pos + value_len + 2;

        if (needed_size > buffer_size) {
            buffer_size = needed_size * 2;
            result = (char *)realloc(result, buffer_size);
        }

        pos += snprintf(result + pos, buffer_size - pos, "%s", value_str);

        current = current->next;
        if (current != NULL) {
            result[pos++] = ',';
        }

        free(value_str);
    }

    result[pos++] = ']';
    result[pos] = '\0';

    return result;
}

#endif
#endif