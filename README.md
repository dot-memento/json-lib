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

For parsing, you can use an optional option structure (you can use NULL instead if you don't care):

```c
json_error_info error_info;
json_parse_options options = {
    .error_info = &error_info; // Optional: for additionnal parsing error info
    .max_depth = 50 // Default is 1000
}
```

Parse JSON from a file (or stdin):

```c
// ...
// FILE *file;
// ...

json_value *root;
json_error error = json_parse_file(file, &root, &options);
if (error) {
    // Handle error...
}

// ...
// Use the API: json_object_get, json_get_bool, json_change_to_number, etc...
// ...

json_free(root);
```

Parse JSON from a string:

```c
json_value *root;
char *json_string = "{\"key\": \"value\"}";
json_error error = json_parse_string(json_string, &root, &options);
if (error) {
    // Handle error...
}

// ...
// Use the API: json_object_get, json_get_bool, json_change_to_number, etc...
// ...

json_free(root);
```

## Documentation

Refer to [json.h](src/json.h) for full API documentation.

## License

[MIT License](LICENSE)
