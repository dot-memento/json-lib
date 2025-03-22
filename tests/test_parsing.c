/**
 * @file test_parsing.c
 * @brief Tests for the JSON parsing functions.
 */

#include "test_framework.h"

static int tests_run = 0;
static int tests_failed = 0;

/* Test parsing */
void test_parsing() {
    json_value *value = NULL;
    json_error error;
    
    /* Test simple values */
    error = json_parse_string("null", &value, NULL);
    ASSERT_JSON_SUCCESS("Parse null literal", error);
    
    json_type type;
    error = json_get_type(value, &type);
    ASSERT_JSON_SUCCESS("Get type of parsed null", error);
    ASSERT_EQUAL_INT("Parsed null has correct type", JSON_NULL, type);
    json_free(value);
    
    error = json_parse_string("true", &value, NULL);
    ASSERT_JSON_SUCCESS("Parse true literal", error);
    
    bool bool_val;
    error = json_get_bool(value, &bool_val);
    ASSERT_JSON_SUCCESS("Get bool from parsed true", error);
    ASSERT_EQUAL_INT("Parsed true has correct value", true, bool_val);
    json_free(value);
    
    error = json_parse_string("42.5", &value, NULL);
    ASSERT_JSON_SUCCESS("Parse number", error);
    
    double num_val;
    error = json_get_number(value, &num_val);
    ASSERT_JSON_SUCCESS("Get number from parsed value", error);
    ASSERT_EQUAL_DOUBLE("Parsed number has correct value", 42.5, num_val);
    json_free(value);
    
    error = json_parse_string("\"test string\"", &value, NULL);
    ASSERT_JSON_SUCCESS("Parse string", error);
    
    const char *str_val;
    error = json_get_string(value, &str_val);
    ASSERT_JSON_SUCCESS("Get string from parsed value", error);
    ASSERT_EQUAL_STRING("Parsed string has correct value", "test string", str_val);
    json_free(value);
    
    /* Test array */
    error = json_parse_string("[1, \"two\", true, null]", &value, NULL);
    ASSERT_JSON_SUCCESS("Parse array", error);
    
    error = json_get_type(value, &type);
    ASSERT_JSON_SUCCESS("Get type of parsed array", error);
    ASSERT_EQUAL_INT("Parsed array has correct type", JSON_ARRAY, type);
    
    size_t length;
    error = json_array_length(value, &length);
    ASSERT_JSON_SUCCESS("Get length of parsed array", error);
    ASSERT_EQUAL_INT("Parsed array has correct length", 4, length);
    json_free(value);
    
    /* Test object */
    error = json_parse_string("{\"name\": \"John\", \"age\": 30, \"is_active\": true}", &value, NULL);
    ASSERT_JSON_SUCCESS("Parse object", error);
    
    error = json_get_type(value, &type);
    ASSERT_JSON_SUCCESS("Get type of parsed object", error);
    ASSERT_EQUAL_INT("Parsed object has correct type", JSON_OBJECT, type);
    
    size_t size;
    error = json_object_size(value, &size);
    ASSERT_JSON_SUCCESS("Get size of parsed object", error);
    ASSERT_EQUAL_INT("Parsed object has correct size", 3, size);
    
    /* Check specific object value */
    json_value *obj_value;
    error = json_object_get(value, "name", &obj_value);
    ASSERT_JSON_SUCCESS("Get object property", error);
    
    error = json_get_string(obj_value, &str_val);
    ASSERT_JSON_SUCCESS("Get string from object property", error);
    ASSERT_EQUAL_STRING("Object property has correct value", "John", str_val);
    
    json_free(value);
}

#define ASSERT_PARSE_ERROR(msg, json_str, error_code) do { \
    json_value *_value; \
    json_error _error = json_parse_string(json_str, &_value, NULL); \
    ASSERT_JSON_ERROR(msg, error_code, _error); \
} while (0)

/* Test parsing errors */
void test_errors() {
    /* Test null pointer handling */
    json_error error = json_parse_string("true", NULL, NULL);
    ASSERT_EQUAL_INT("NULL output causes parse error", JSON_ERROR_NULL, error);

    ASSERT_PARSE_ERROR("Null input pointer causes error", NULL, JSON_ERROR_NULL);

    /* Test invalid JSON string */
    ASSERT_PARSE_ERROR("Missing quotes cause error", "{invalid: \"json\"}", JSON_ERROR_UNEXPECTED_CHARACTER);
    ASSERT_PARSE_ERROR("Invalid identifier causes error", "{\"invalid\": json}", JSON_ERROR_UNEXPECTED_IDENTIFIER);
    ASSERT_PARSE_ERROR("Unclosed array causes error", "[1, 2, 3", JSON_ERROR_UNEXPECTED_CHARACTER);
    ASSERT_PARSE_ERROR("Multiple root values cause error", "true 4", JSON_ERROR_UNEXPECTED_CHARACTER);

    /* Test invalid unicode escape */
    ASSERT_PARSE_ERROR("Invalid unicode escape length causes error", "\"\\u123\"", JSON_ERROR_UNICODE);
    ASSERT_PARSE_ERROR("Invalid unicode escape character causes error", "\"\\u12G4\"", JSON_ERROR_UNICODE);
    ASSERT_PARSE_ERROR("Invalid unicode high surrogate range causes error", "\"\\uDE3D\\uDE03\"", JSON_ERROR_UNICODE);
    ASSERT_PARSE_ERROR("Invalid unicode low surrogate range causes error", "\"\\uD83D\\uD803\"", JSON_ERROR_UNICODE);
    ASSERT_PARSE_ERROR("Low unicode surrogate without high surrogate causes error", "\"\\uD803\"", JSON_ERROR_UNICODE);
}

int main() {
    BEGIN_TESTS();

    test_parsing();
    test_errors();

    FINISH_TESTS();
}