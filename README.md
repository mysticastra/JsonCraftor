# JsonCraftor

JsonCraftor is a lightweight, header-only JSON parser for C that maps JSON data directly to C structs. It provides type-safe parsing with support for nested objects and arrays.

## Features

- **Header-only**: Just include `jsoncraftor.h` in your project
- **Type-safe parsing**: Automatic type checking and conversion
- **Struct mapping**: Direct mapping of JSON to C structs
- **Nested objects**: Support for complex nested structures
- **Array support**: Handle both primitive and string arrays
- **Required fields**: Mark fields as required or optional
- **Error handling**: Detailed error messages for parsing failures
- **No external dependencies**: Only uses standard C libraries

## Supported Types

- `int`: Integer values
- `double`: Floating-point values
- `bool`: Boolean values (true/false)
- `char[]`: String values
- `struct`: Nested objects
- Arrays of:
  - Integers
  - Strings

## Usage

1. Include the header:
```c
#include "jsoncraftor.h"
```

2. Define your structs:
```c
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
```

3. Create JSON mappings:
```c
// For nested objects
JsonMap address_mappings[] = {
    {"street", &addr.street, 's', sizeof(addr.street), true, NULL},
    {"number", &addr.number, 'i', 0, true, NULL},
    {"city", &addr.city, 's', sizeof(addr.city), false, NULL}
};

// For arrays
JsonMap score_item = {"item", NULL, 'i', 0, true, NULL};
JsonMap tag_item = {"item", NULL, 's', 20, false, NULL};

// Main mappings
JsonMap mappings[] = {
    {"age", &person.age, 'i', 0, true, NULL},
    {"name", &person.name, 's', sizeof(person.name), true, NULL},
    {"is_student", &person.is_student, 'b', 0, false, NULL},
    {"gpa", &person.gpa, 'd', 0, false, NULL},
    {"address", &person.address, 'o', 3, true, address_mappings},
    {"scores", person.scores, 'a', 5, true, &score_item},
    {"tags", person.tags, 'a', 3, false, &tag_item}
};
```

4. Parse JSON:
```c
char* error = NULL;
if (parse_json(json_string, mappings, mapping_count, &error)) {
    // Success: use your populated struct
} else {
    printf("Error: %s\n", error);
}
```

## JsonMap Structure

```c
typedef struct JsonMap {
    const char* json_key;     // JSON key name
    void* struct_member;      // Pointer to struct member
    char type;               // Type identifier
    size_t size;            // Size for strings/arrays
    bool required;          // Whether field is required
    struct JsonMap* nested; // For nested objects/arrays
} JsonMap;
```

### Type Identifiers

- `'i'`: Integer
- `'s'`: String
- `'b'`: Boolean
- `'d'`: Double
- `'o'`: Object
- `'a'`: Array

## Building and Testing

The project uses a Makefile with the following targets:

```bash
make all          # Build everything
make example      # Build the example
make test         # Run tests
make clean        # Clean build files
make help         # Show help
```

### Running Tests

```bash
make test
```

### Running Example

```bash
make run
```

## Example Output

```json
Person parsed successfully!
Name: John
Age: 25
Is student: yes
GPA: 3.80
Address: Main St 123, New York
Scores: [85, 92, 88, 95, 90]
Tags: ["smart", "friendly", "active"]
```

## Error Handling

The parser provides detailed error messages for various scenarios:

- Missing required fields
- Invalid value types
- Array size mismatches
- Invalid JSON syntax
- Unterminated strings
- Nested object errors

## Project Structure

```
JsonCraftor/
├── jsoncraftor.h        # Main parser header
├── examples/            # Example usage
│   └── example.c       # Example program
├── tests/              # Test suite
│   └── test_parser.c   # Parser tests
└── Makefile            # Build system
```

## Contributing

Feel free to submit issues, fork the repository, and create pull requests for any improvements.

## License

This project is licensed under the MIT License - see the LICENSE file for details.