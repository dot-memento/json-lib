/**
 * @file test_serialization.c
 * @brief Tests for the JSON serialization functions.
 */

#include "test_framework.h"

static int tests_run = 0;
static int tests_failed = 0;

/* Test serialization */
void test_serialization() {
    json_value *value = NULL;
    json_error error;
    char *output = NULL;
    
    /* Create a complex JSON structure */
    error = json_object_create(&value);
    ASSERT_JSON_SUCCESS("Create object for serialization", error);
    
    json_value *name_value;
    error = json_string_create("Test Object", &name_value);
    ASSERT_JSON_SUCCESS("Create name string", error);
    error = json_object_set(value, "name", name_value);
    ASSERT_JSON_SUCCESS("Set name in object", error);
    
    json_value *active_value;
    error = json_bool_create(true, &active_value);
    ASSERT_JSON_SUCCESS("Create active bool", error);
    error = json_object_set(value, "active", active_value);
    ASSERT_JSON_SUCCESS("Set active in object", error);
    
    json_value *array;
    error = json_array_create(&array);
    ASSERT_JSON_SUCCESS("Create array for serialization", error);
    
    json_value *item;
    error = json_number_create(1, &item);
    ASSERT_JSON_SUCCESS("Create array item 1", error);
    error = json_array_append(array, item);
    ASSERT_JSON_SUCCESS("Append item 1 to array", error);
    
    error = json_number_create(2, &item);
    ASSERT_JSON_SUCCESS("Create array item 2", error);
    error = json_array_append(array, item);
    ASSERT_JSON_SUCCESS("Append item 2 to array", error);
    
    error = json_object_set(value, "values", array);
    ASSERT_JSON_SUCCESS("Set array in object", error);
    
    /* Serialize to string */
    error = json_serialize_to_string(value, &output, NULL);
    ASSERT_JSON_SUCCESS("Serialize to string", error);
    ASSERT_NOT_NULL("Output string is not null", output);
    
    /* Parse the serialized string back to verify */
    json_value *parsed = NULL;
    error = json_parse_string(output, &parsed, NULL);
    ASSERT_JSON_SUCCESS("Parse serialized output", error);
    
    /* Verify structure was preserved */
    json_value *check_value;
    const char *str_val;
    error = json_object_get(parsed, "name", &check_value);
    ASSERT_JSON_SUCCESS("Get name from parsed object", error);
    error = json_string_get(check_value, &str_val);
    ASSERT_JSON_SUCCESS("Get string from parsed name", error);
    ASSERT_EQUAL_STRING("Parsed name has correct value", "Test Object", str_val);
    
    bool bool_val;
    error = json_object_get(parsed, "active", &check_value);
    ASSERT_JSON_SUCCESS("Get active from parsed object", error);
    error = json_bool_get(check_value, &bool_val);
    ASSERT_JSON_SUCCESS("Get bool from parsed active", error);
    ASSERT_EQUAL_INT("Parsed active has correct value", true, bool_val);
    
    json_value *check_array;
    error = json_object_get(parsed, "values", &check_array);
    ASSERT_JSON_SUCCESS("Get values array from parsed object", error);
    
    size_t length;
    error = json_array_length(check_array, &length);
    ASSERT_JSON_SUCCESS("Get length of parsed array", error);
    ASSERT_EQUAL_INT("Parsed array has correct length", 2, length);
    
    /* Clean up */
    free(output);
    json_free(value);
    json_free(parsed);
}

int main() {
    BEGIN_TESTS();

    test_serialization();

    FINISH_TESTS();
}