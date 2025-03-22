#include "../jsoncraftor.h"
#include <stdio.h>
#include <string.h>

// Define your structs normally
typedef struct {
    char street[100];
    int number;
    char city[50];
} Address;

typedef struct {
    int age;
    char name[50];
    bool is_student;
    double gpa;
    Address address;        // Nested object
    int scores[5];         // Array of integers
    char tags[3][20];      // Array of strings
} Person;

int main() {
    Person person;
    char* error = NULL;
    
    // Define nested address mappings
    JsonMap address_mappings[] = {
        {"street", &person.address.street, 's', sizeof(person.address.street), true, NULL},
        {"number", &person.address.number, 'i', 0, true, NULL},
        {"city", &person.address.city, 's', sizeof(person.address.city), false, NULL}
    };

    // Define array mappings
    JsonMap score_item = {"item", NULL, 'i', 0, true, NULL};  // Template for integer array items
    JsonMap tag_item = {"item", NULL, 's', 20, false, NULL};   // Template for string array items
    
    // Define main JSON mappings
    JsonMap mappings[] = {
        {"age", &person.age, 'i', 0, true, NULL},
        {"name", &person.name, 's', sizeof(person.name), true, NULL},
        {"is_student", &person.is_student, 'b', 0, false, NULL},
        {"gpa", &person.gpa, 'd', 0, false, NULL},
        {"address", &person.address, 'o', 3, true, address_mappings},
        {"scores", person.scores, 'a', 5, true, &score_item},
        {"tags", person.tags, 'a', 3, false, &tag_item}
    };
    
    // Example JSON with nested object and arrays
    const char* json = "{"
        "\"name\": \"John\","
        "\"age\": 25,"
        "\"is_student\": true,"
        "\"gpa\": 3.80,"
        "\"address\": {"
            "\"street\": \"Main St\","
            "\"number\": 123,"
            "\"city\": \"New York\""
        "},"
        "\"scores\": [85, 92, 88, 95, 90],"
        "\"tags\": [\"smart\", \"friendly\", \"active\"]"
    "}";
    
    if (parse_json(json, mappings, 7, &error)) {
        printf("Person parsed successfully!\n");
        printf("Name: %s\n", person.name);
        printf("Age: %d\n", person.age);
        printf("Is student: %s\n", person.is_student ? "yes" : "no");
        printf("GPA: %.2f\n", person.gpa);
        printf("Address: %s %d, %s\n", 
               person.address.street,
               person.address.number,
               person.address.city);
        printf("Scores: [");
        for (int i = 0; i < 5; i++) {
            printf("%d%s", person.scores[i], i < 4 ? ", " : "");
        }
        printf("]\n");
        printf("Tags: [");
        for (int i = 0; i < 3; i++) {
            printf("\"%s\"%s", person.tags[i], i < 2 ? ", " : "");
        }
        printf("]\n");
    } else {
        printf("Error parsing person: %s\n", error);
    }

    return 0;
} 