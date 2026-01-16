libtct
======

Tiny C Template engine (TCT) is a micro template engine for C Language with advanced template support.

## Features

- **Simple API**: Easy-to-use C API with minimal dependencies
- **Variable Substitution**: Replace placeholders with values
- **Conditional Blocks**: Show/hide content based on conditions
- **Loop Support**: Iterate over content blocks
- **Nested Templates**: Support for deeply nested template structures
- **Zero Dependencies**: Only requires standard C library
- **Header/Source Split**: Clean library structure for easy integration

## Template Syntax

### Variable Substitution
```c
"Hello {{ name }}!"
```

### Conditional Blocks
```c
"{{#if condition}}This is shown when condition is truthy{{/if}}"
```

### Conditional with Else
```c
"{{#if admin}}Admin Panel{{#else}}User Panel{{/if}}"
```

### Loop Blocks
```c
"{{#each items}}Item: {{ items }}{{/each}}"
```

Loop blocks support full array iteration. To create an array, add multiple arguments with the same name:

```c
tct_arguments *args = NULL;
tct_add_argument(args, "items", "%s", "Apple");
tct_add_argument(args, "items", "%s", "Banana");
tct_add_argument(args, "items", "%s", "Cherry");

char *result = tct_render("{{#each items}}{{ items }}, {{/each}}", args);
// Result: "Apple, Banana, Cherry, "
```

**Note**: Items are iterated in the order they were added. Inside a loop block, the loop variable (e.g., `{{ items }}`) refers to the current iteration value.

### Nested Templates
All template features can be nested arbitrarily deep:
```c
"{{#if user}}Welcome {{ username }}!{{#if is_admin}}{{#each items}}Item{{/each}}{{/if}}{{/if}}"
```

## Building

### Using Make
```bash
# Build library and example
make all

# Build and run tests
make test

# Run the example
make run

# Clean build artifacts
make clean

# Install library (requires sudo)
sudo make install
```

### Manual Compilation
```bash
# Compile library
gcc -c -Wall -O2 libtct.c -o libtct.o
ar rcs libtct.a libtct.o

# Compile example
gcc -Wall -O2 example.c -L. -ltct -o example

# Run example
./example
```

## Usage Example

```c
#include "libtct.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    char *template = "Welcome to {{ project_name }}!\n"
        "{{ project_name }}({{ project_abbreviation }}) is a micro template "
        "engine for {{ language }}. The {{ project_name }} project is "
        "released under the terms of the {{ license }} license.\n";
    
    tct_arguments *args = NULL;
    tct_add_argument(args, "project_abbreviation", "%s", "TCT");
    tct_add_argument(args, "project_name", "%s", "Tiny C Template engine");
    tct_add_argument(args, "language", "%s", "C Language");
    tct_add_argument(args, "license", "%s", "MIT");
    
    char *result = tct_render(template, args);
    fputs(result, stdout);
    
    tct_free_argument(args);
    free(result);
    
    return 0;
}
```

## API Reference

### Types

- `tct_arguments` - Linked list structure for template arguments

### Functions

- `tct_add_argument(args, name, format, ...)` - Add a variable to the arguments list
- `tct_get_value(args, name)` - Get a variable value by name
- `tct_find_argument(args, name)` - Find an argument structure by name
- `tct_render(template, args)` - Render a template with arguments
- `tct_free_argument(args)` - Free all arguments in the list

### Adding Arguments

The `tct_add_argument` macro accepts printf-style format strings:

```c
tct_arguments *args = NULL;
tct_add_argument(args, "name", "%s", "John");
tct_add_argument(args, "age", "%d", 25);
tct_add_argument(args, "price", "%.2f", 19.99);
```

### Creating Arrays for Loops

To create an array for loop iteration, add multiple arguments with the same name:

```c
tct_arguments *args = NULL;
tct_add_argument(args, "fruits", "%s", "Apple");
tct_add_argument(args, "fruits", "%s", "Banana");
tct_add_argument(args, "fruits", "%s", "Orange");

char *template = "{{#each fruits}}- {{ fruits }}\n{{/each}}";
char *result = tct_render(template, args);
// Output:
// - Apple
// - Banana
// - Orange
```

**Note**: Array items are iterated in the order they were added to the arguments list.

### Truthy Values

In conditional and loop blocks, the following values are considered "falsy":
- Empty string `""`
- String `"0"`
- String `"false"`

All other values are considered "truthy".

## Testing

The project includes comprehensive test suites:

- `tests/test_basic.c` - Basic variable substitution tests
- `tests/test_conditionals.c` - Conditional block tests
- `tests/test_loops.c` - Loop functionality tests
- `tests/test_nested.c` - Nested template tests

Run all tests with:
```bash
make test
```

## CI/CD

The project includes GitHub Actions workflows for continuous integration:

- Builds and tests on Linux (Ubuntu)
- Builds and tests on macOS
- Code quality checks with cppcheck
- Memory leak detection with valgrind

## Installation

### System-wide Installation
```bash
sudo make install
```

This installs:
- `/usr/local/lib/libtct.a` - Static library
- `/usr/local/include/libtct.h` - Header file

### Uninstallation
```bash
sudo make uninstall
```

## License

Released under the terms of the MIT license.
