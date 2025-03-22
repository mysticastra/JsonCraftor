#include "../jsoncraftor.h"
#include <stdio.h>
#include <string.h>

// Global test counters
static int passed_tests = 0;
static int failed_tests = 0;

#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("❌ %s (Line %d)\n", message, __LINE__); \
            failed_tests++; \
        } else { \
            printf("✓ %s\n", message); \
            passed_tests++; \
        } \
    } while (0)

// Test structures
typedef struct {
    char street[100];
    int number;
    char city[50];
} TestAddress;

typedef struct {
    int age;
    char name[50];
    bool is_student;
    double gpa;
    TestAddress address;
    int scores[5];
    char tags[3][20];
} TestPerson;

// Test cases
void test_basic_types() {
    printf("\nTesting basic types:\n");
    TestPerson person;
    char* error = NULL;

    const char* json = "{"
        "\"name\": \"John\","
        "\"age\": 25,"
        "\"is_student\": true,"
        "\"gpa\": 3.8"
    "}";

    JsonMap mappings[] = {
        {"age", &person.age, 'i', 0, true, NULL},
        {"name", &person.name, 's', sizeof(person.name), true, NULL},
        {"is_student", &person.is_student, 'b', 0, false, NULL},
        {"gpa", &person.gpa, 'd', 0, false, NULL}
    };

    bool result = parse_json(json, mappings, 4, &error);
    ASSERT(result, "Basic types parsing");
    ASSERT(person.age == 25, "Integer parsing");
    ASSERT(strcmp(person.name, "John") == 0, "String parsing");
    ASSERT(person.is_student == true, "Boolean parsing");
    ASSERT(person.gpa == 3.8, "Double parsing");
}

void test_nested_object() {
    printf("\nTesting nested object:\n");
    TestPerson person;
    char* error = NULL;

    JsonMap address_mappings[] = {
        {"street", &person.address.street, 's', sizeof(person.address.street), true, NULL},
        {"number", &person.address.number, 'i', 0, true, NULL},
        {"city", &person.address.city, 's', sizeof(person.address.city), false, NULL}
    };

    const char* json = "{"
        "\"address\": {"
            "\"street\": \"Main St\","
            "\"number\": 123,"
            "\"city\": \"New York\""
        "}"
    "}";

    JsonMap mappings[] = {
        {"address", &person.address, 'o', 3, true, address_mappings}
    };

    bool result = parse_json(json, mappings, 1, &error);
    ASSERT(result, "Nested object parsing");
    ASSERT(strcmp(person.address.street, "Main St") == 0, "Nested string parsing");
    ASSERT(person.address.number == 123, "Nested integer parsing");
    ASSERT(strcmp(person.address.city, "New York") == 0, "Nested optional string parsing");
}

void test_arrays() {
    printf("\nTesting arrays:\n");
    TestPerson person;
    char* error = NULL;

    JsonMap score_item = {"item", NULL, 'i', 0, true, NULL};
    JsonMap tag_item = {"item", NULL, 's', 20, false, NULL};

    const char* json = "{"
        "\"scores\": [85, 92, 88, 95, 90],"
        "\"tags\": [\"smart\", \"friendly\", \"active\"]"
    "}";

    JsonMap mappings[] = {
        {"scores", person.scores, 'a', 5, true, &score_item},
        {"tags", person.tags, 'a', 3, false, &tag_item}
    };

    bool result = parse_json(json, mappings, 2, &error);
    ASSERT(result, "Arrays parsing");
    ASSERT(person.scores[0] == 85, "Integer array first element");
    ASSERT(person.scores[4] == 90, "Integer array last element");
    ASSERT(strcmp(person.tags[0], "smart") == 0, "String array first element");
    ASSERT(strcmp(person.tags[2], "active") == 0, "String array last element");
}

void test_error_cases() {
    printf("\nTesting error cases:\n");
    TestPerson person;
    char* error = NULL;

    // Test missing required field
    const char* json1 = "{}";
    JsonMap mappings1[] = {
        {"name", &person.name, 's', sizeof(person.name), true, NULL}
    };
    bool result = parse_json(json1, mappings1, 1, &error);
    ASSERT(!result, "Missing required field detection");

    // Test invalid JSON syntax
    const char* json2 = "{\"name\": \"John\", }";  // Invalid trailing comma
    bool result2 = parse_json(json2, mappings1, 1, &error);
    ASSERT(!result2, "Invalid JSON syntax detection");

    const char* json3 = "{\"name\": \"John\"";  // Missing closing brace
    bool result3 = parse_json(json3, mappings1, 1, &error);
    ASSERT(!result3, "Unclosed object detection");

    // Test invalid value type
    const char* json4 = "{\"age\": \"not a number\"}";
    JsonMap mappings3[] = {
        {"age", &person.age, 'i', 0, true, NULL}
    };
    bool result4 = parse_json(json4, mappings3, 1, &error);
    ASSERT(!result4, "Invalid value type detection");
}

int main() {
    printf("Running JSON parser tests...\n");

    test_basic_types();
    test_nested_object();
    test_arrays();
    test_error_cases();

    printf("\nTest Summary:\n");
    printf("✓ Passed: %d\n", passed_tests);
    printf("❌ Failed: %d\n", failed_tests);
    printf("Total: %d\n", passed_tests + failed_tests);

    return failed_tests > 0 ? 1 : 0;
} 