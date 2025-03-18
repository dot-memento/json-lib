# JSON Library for C

A lightweight JSON parser and manipulation library written in C.

## Features

- Parse JSON from files and strings
- Access and modify JSON objects and arrays
- Pretty-print JSON entries
- Support for all JSON types: null, number, string, boolean, array, and object

## Installation

Just drop ```json.h``` and ```json.c``` in your project and you're ready to go.

## Usage

Include the header in your project:

```c
#include "json.h"
```

Parse JSON from a file (or stdin):

```c
// ...
// Get a FILE pointer: stdin, fopen, etc.
// ...

json_entry *root = json_parse_file(file);
if (root == NULL) {
    // Handle error...
}

// ...
// Use the API: json_object_get, json_array_get, json_set_number, etc.
// ...

json_free(root);
```

Parse JSON from a string:

```c
char *json_string = "{\"key\": \"value\"}";
json_entry *root = json_parse_string(json_string);
if (root == NULL) {
    // Handle error...
}

// ...
// Use the API: json_object_get, json_array_get, json_set_number, etc.
// ...

json_free(root);
```

## Documentation

Refer to [json.h](src/json.h) for full API documentation.

## License

[MIT License](LICENSE)

