# JSONCraftor

**JSONCraftor** is a simple JSON parser and builder written in C. It allows you to parse JSON strings and access the values in a structured format (objects, arrays, strings, numbers, etc.). This project includes the header file `jsoncraftor.h` which contains both the JSON parser and related utility functions, and a main example in the `examples` directory.

## Features

- **Parse JSON**: Supports parsing of JSON strings containing objects, arrays, strings, numbers, booleans, and null values.
- **Create JSON objects and arrays**: You can programmatically create JSON objects and arrays.
- **Access JSON values**: Retrieve values by key from JSON objects or by index from JSON arrays.
- **Cross-platform**: This project is written in C and should work on most platforms with a C compiler.

## Installation

1. **Clone the repository**:
   ```sh
   git clone https://github.com/mysticastra/JsonCraftor.git
   cd jsoncraftor
   ```

2. **Build the project**:
   The project uses `Make` for building. Run the following command to compile and generate the executable:

   ```sh
   make
   ```

   This will compile the `main.c` example code and link it into an executable named `jsoncraftor_example`.

3. **Clean the project**:
   To remove object files and the compiled executable, run:

   ```sh
   make clean
   ```

4. **Run the example**:
   To execute the example program:

   ```sh
   make run
   ```

   This will run the compiled executable and show how the JSON parser can be used.

## Usage

### Parsing JSON

Basic example of using JsonCraftor is given below:

```c
#include <stdio.h>

#define JSONCRAFTOR_IMPLEMENTATION

#include "jsoncraftor.h"

int main() {
    const char *json_str = "{\"name\": \"John\", \"age\": 30, \"isStudent\": false}";
    
    // Parse the JSON string
    json_value *parsed_json = parse_json(json_str);

    // Get values from the JSON object
    char type[20];
    char value[100];
    
    get_type(parsed_json, "name", type);
    get_value(parsed_json, "name", value);
    printf("Key: name, Type: %s, Value: %s\n", type, value);
    
    get_type(parsed_json, "age", type);
    get_value(parsed_json, "age", value);
    printf("Key: age, Type: %s, Value: %s\n", type, value);

    return 0;
}
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

If you'd like to contribute to the project, please fork it and submit a pull request. Make sure to follow these guidelines:
- Write clear commit messages.
- Provide tests for any new features.
- Ensure code style is consistent with the project.