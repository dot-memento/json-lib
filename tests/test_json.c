/**
 * @file test_json.c
 * @brief Tests for a lightweight JSON parser and manipulation library for C
 *
 * @author Michael Teixeira
 * @copyright MIT License
 * @see https://github.com/dot-memento/json-lib
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "json.h"

// Testing framework macros
#define TEST(name) static void test_##name(void)
#define RUN_TEST(name) do { \
    printf("Running test: %s\n", #name); \
    test_##name(); \
    printf("PASSED: %s\n\n", #name); \
} while(0)

// Tests that condition is true
#define ASSERT(condition) do { \
    if (!(condition)) { \
        printf("ASSERT FAILED: %s at line %d\n", #condition, __LINE__); \
        exit(1); \
    } \
} while(0)

// Tests that strings are equal
#define ASSERT_STR_EQ(a, b) do { \
    if (strcmp((a), (b)) != 0) { \
        printf("ASSERT FAILED: \"%s\" != \"%s\" at line %d\n", (a), (b), __LINE__); \
        exit(1); \
    } \
} while(0)

// Tests that two values are approximately equal (for doubles)
#define ASSERT_DOUBLE_EQ(a, b) do { \
    double diff = (a) - (b); \
    if (diff < 0) diff = -diff; \
    if (diff > 0.0001) { \
        printf("ASSERT FAILED: %f != %f at line %d\n", (a), (b), __LINE__); \
        exit(1); \
    } \
} while(0)

// ==================== Parse Tests ====================

TEST(parse_simple_object) {
    const char *json_str = "{ \"name\": \"John\", \"age\": 30 }";
    json_entry *obj = json_parse_string((char *)json_str);
    
    ASSERT(obj != NULL);
    ASSERT(json_get_type(obj) == JSON_OBJECT);
    
    json_entry *name = json_object_get(obj, "name");
    ASSERT(name != NULL);
    ASSERT(json_get_type(name) == JSON_STRING);
    ASSERT_STR_EQ(json_get_string(name), "John");
    
    json_entry *age = json_object_get(obj, "age");
    ASSERT(age != NULL);
    ASSERT(json_get_type(age) == JSON_NUMBER);
    ASSERT_DOUBLE_EQ(json_get_number(age), 30.0);
    
    json_free(obj);
}

TEST(parse_simple_array) {
    const char *json_str = "[1, 2, 3, 4, 5]";
    json_entry *arr = json_parse_string((char *)json_str);
    
    ASSERT(arr != NULL);
    ASSERT(json_get_type(arr) == JSON_ARRAY);
    ASSERT(json_array_count(arr) == 5);
    
    for (int i = 0; i < 5; i++) {
        json_entry *item = json_array_get(arr, i);
        ASSERT(item != NULL);
        ASSERT(json_get_type(item) == JSON_NUMBER);
        ASSERT_DOUBLE_EQ(json_get_number(item), (double)(i + 1));
    }
    
    json_free(arr);
}

TEST(parse_nested_structures) {
    const char *json_str = 
        "{"
        "  \"name\": \"John\","
        "  \"age\": 30,"
        "  \"address\": {"
        "    \"street\": \"123 Main St\","
        "    \"city\": \"Anytown\","
        "    \"zip\": 12345"
        "  },"
        "  \"phones\": ["
        "    \"111-222-3333\","
        "    \"444-555-6666\""
        "  ]"
        "}";
    
    json_entry *obj = json_parse_string((char *)json_str);
    
    ASSERT(obj != NULL);
    ASSERT(json_get_type(obj) == JSON_OBJECT);

    // Test nested object
    json_entry *address = json_object_get(obj, "address");
    ASSERT(address != NULL);
    ASSERT(json_get_type(address) == JSON_OBJECT);
    
    json_entry *street = json_object_get(address, "street");
    ASSERT(street != NULL);
    ASSERT_STR_EQ(json_get_string(street), "123 Main St");
    
    json_entry *zip = json_object_get(address, "zip");
    ASSERT(zip != NULL);
    ASSERT_DOUBLE_EQ(json_get_number(zip), 12345.0);

    // Test nested array
    json_entry *phones = json_object_get(obj, "phones");
    ASSERT(phones != NULL);
    ASSERT(json_get_type(phones) == JSON_ARRAY);
    ASSERT(json_array_count(phones) == 2);
    
    json_entry *phone1 = json_array_get(phones, 0);
    ASSERT(phone1 != NULL);
    ASSERT_STR_EQ(json_get_string(phone1), "111-222-3333");
    
    json_free(obj);
}

TEST(parse_all_types) {
    const char *json_str = 
        "{"
        "  \"nullValue\": null,"
        "  \"boolTrue\": true,"
        "  \"boolFalse\": false,"
        "  \"numberInt\": 42,"
        "  \"numberFloat\": 3.14,"
        "  \"string\": \"hello world\","
        "  \"emptyObject\": {},"
        "  \"emptyArray\": []"
        "}";
    
    json_entry *obj = json_parse_string((char *)json_str);
    
    ASSERT(obj != NULL);
    ASSERT(json_get_type(obj) == JSON_OBJECT);
    
    // Test null
    json_entry *null_val = json_object_get(obj, "nullValue");
    ASSERT(null_val != NULL);
    ASSERT(json_get_type(null_val) == JSON_NULL);
    
    // Test boolean
    json_entry *bool_true = json_object_get(obj, "boolTrue");
    ASSERT(bool_true != NULL);
    ASSERT(json_get_type(bool_true) == JSON_BOOL);
    ASSERT(json_get_bool(bool_true) == true);
    
    json_entry *bool_false = json_object_get(obj, "boolFalse");
    ASSERT(bool_false != NULL);
    ASSERT(json_get_type(bool_false) == JSON_BOOL);
    ASSERT(json_get_bool(bool_false) == false);
    
    // Test numbers
    json_entry *num_int = json_object_get(obj, "numberInt");
    ASSERT(num_int != NULL);
    ASSERT(json_get_type(num_int) == JSON_NUMBER);
    ASSERT_DOUBLE_EQ(json_get_number(num_int), 42.0);
    
    json_entry *num_float = json_object_get(obj, "numberFloat");
    ASSERT(num_float != NULL);
    ASSERT(json_get_type(num_float) == JSON_NUMBER);
    ASSERT_DOUBLE_EQ(json_get_number(num_float), 3.14);
    
    // Test string
    json_entry *str = json_object_get(obj, "string");
    ASSERT(str != NULL);
    ASSERT(json_get_type(str) == JSON_STRING);
    ASSERT_STR_EQ(json_get_string(str), "hello world");
    
    // Test empty structures
    json_entry *empty_obj = json_object_get(obj, "emptyObject");
    ASSERT(empty_obj != NULL);
    ASSERT(json_get_type(empty_obj) == JSON_OBJECT);
    
    json_entry *empty_arr = json_object_get(obj, "emptyArray");
    ASSERT(empty_arr != NULL);
    ASSERT(json_get_type(empty_arr) == JSON_ARRAY);
    ASSERT(json_array_count(empty_arr) == 0);
    
    json_free(obj);
}

// ==================== Accessor Tests ====================

TEST(accessor_functions) {
    const char *json_str = 
        "{"
        "  \"number\": 42,"
        "  \"string\": \"hello\","
        "  \"bool\" : true,"
        "  \"array\": [1, 2, 3]"
        "}";
    
    json_entry *obj = json_parse_string((char *)json_str);
    ASSERT(obj != NULL);
    
    // Test try functions with correct types
    double num_val;
    ASSERT(json_get_number_safe(json_object_get(obj, "number"), &num_val));
    ASSERT_DOUBLE_EQ(num_val, 42.0);
    
    const char *str_val;
    ASSERT(json_get_string_safe(json_object_get(obj, "string"), &str_val));
    ASSERT_STR_EQ(str_val, "hello");
    
    bool bool_val;
    ASSERT(json_get_bool_safe(json_object_get(obj, "bool"), &bool_val));
    ASSERT(bool_val == true);
    
    // Test try functions with incorrect types
    ASSERT(!json_get_number_safe(json_object_get(obj, "string"), &num_val));
    ASSERT(!json_get_string_safe(json_object_get(obj, "number"), &str_val));
    ASSERT(!json_get_bool_safe(json_object_get(obj, "number"), &bool_val));
    
    // Test array accessor
    json_entry *array = json_object_get(obj, "array");
    ASSERT(json_array_count(array) == 3);
    
    // Test out of bounds array access
    ASSERT(json_array_get(array, 3) == NULL);
    
    // Test non-existent key
    ASSERT(json_object_get(obj, "nonexistent") == NULL);
    
    json_free(obj);
}

// ==================== Modification Tests ====================

TEST(create_and_modify_object) {
    // Create a new JSON entry
    json_entry *obj = json_new_null();
    ASSERT(obj != NULL);
    
    // Set it as an object
    json_set_object(obj);
    ASSERT(json_get_type(obj) == JSON_OBJECT);
    
    // Add various types of values
    json_entry *str_entry = json_new_null();
    ASSERT(json_set_string(str_entry, "test string"));
    ASSERT(json_object_set(obj, "string", str_entry));
    
    json_entry *num_entry = json_new_null();
    json_set_number(num_entry, 123.45);
    ASSERT(json_object_set(obj, "number", num_entry));
    
    json_entry *bool_entry = json_new_null();
    json_set_bool(bool_entry, true);
    ASSERT(json_object_set(obj, "boolean", bool_entry));
    
    json_entry *null_entry = json_new_null();
    json_set_null(null_entry);
    ASSERT(json_object_set(obj, "null", null_entry));
    
    // Create a nested array
    json_entry *arr_entry = json_new_null();
    json_set_array(arr_entry);
    
    for (int i = 0; i < 5; i++) {
        json_entry *item = json_new_null();
        json_set_number(item, i);
        ASSERT(json_array_append(arr_entry, item));
    }
    
    ASSERT(json_object_set(obj, "array", arr_entry));
    
    // Verify values were set correctly
    json_entry *val;
    val = json_object_get(obj, "string");
    ASSERT(val != NULL);
    ASSERT(json_get_type(val) == JSON_STRING);
    ASSERT_STR_EQ(json_get_string(val), "test string");
    
    val = json_object_get(obj, "number");
    ASSERT(val != NULL);
    ASSERT(json_get_type(val) == JSON_NUMBER);
    ASSERT_DOUBLE_EQ(json_get_number(val), 123.45);
    
    val = json_object_get(obj, "boolean");
    ASSERT(val != NULL);
    ASSERT(json_get_type(val) == JSON_BOOL);
    ASSERT(json_get_bool(val) == true);
    
    val = json_object_get(obj, "null");
    ASSERT(val != NULL);
    ASSERT(json_get_type(val) == JSON_NULL);
    
    val = json_object_get(obj, "array");
    ASSERT(val != NULL);
    ASSERT(json_get_type(val) == JSON_ARRAY);
    ASSERT(json_array_count(val) == 5);
    
    // Test removing a value
    json_entry *removed = json_object_remove(obj, "number");
    ASSERT(removed != NULL);
    ASSERT(json_get_type(removed) == JSON_NUMBER);
    ASSERT_DOUBLE_EQ(json_get_number(removed), 123.45);
    json_free(removed);
    
    // Verify it's gone
    ASSERT(json_object_get(obj, "number") == NULL);
    
    json_free(obj);
}

TEST(create_and_modify_array) {
    // Create a new array
    json_entry *arr = json_new_null();
    json_set_array(arr);
    ASSERT(json_get_type(arr) == JSON_ARRAY);
    
    // Append items
    for (int i = 0; i < 3; i++) {
        json_entry *item = json_new_null();
        json_set_number(item, i);
        ASSERT(json_array_append(arr, item));
    }
    
    ASSERT(json_array_count(arr) == 3);
    
    // Insert an item
    json_entry *str_entry = json_new_null();
    json_set_string(str_entry, "inserted");
    ASSERT(json_array_insert(arr, 1, str_entry));
    
    ASSERT(json_array_count(arr) == 4);
    ASSERT(json_get_type(json_array_get(arr, 1)) == JSON_STRING);
    ASSERT_STR_EQ(json_get_string(json_array_get(arr, 1)), "inserted");
    
    // Remove an item
    json_entry *removed = json_array_remove(arr, 0);
    ASSERT(removed != NULL);
    ASSERT(json_get_type(removed) == JSON_NUMBER);
    ASSERT_DOUBLE_EQ(json_get_number(removed), 0.0);
    json_free(removed);
    
    ASSERT(json_array_count(arr) == 3);
    
    json_free(arr);
}

TEST(type_conversion) {
    json_entry *entry = json_new_null();
    
    // Start as a string
    json_set_string(entry, "test");
    ASSERT(json_get_type(entry) == JSON_STRING);
    
    // Convert to number
    json_set_number(entry, 42.0);
    ASSERT(json_get_type(entry) == JSON_NUMBER);
    ASSERT_DOUBLE_EQ(json_get_number(entry), 42.0);
    
    // Convert to bool
    json_set_bool(entry, false);
    ASSERT(json_get_type(entry) == JSON_BOOL);
    ASSERT(json_get_bool(entry) == false);
    
    // Convert to array
    json_set_array(entry);
    ASSERT(json_get_type(entry) == JSON_ARRAY);
    
    // Add items to the array
    json_entry *item = json_new_null();
    json_set_number(item, 1);
    ASSERT(json_array_append(entry, item));
    
    // Convert to object (should free array items)
    json_set_object(entry);
    ASSERT(json_get_type(entry) == JSON_OBJECT);
    
    // Convert to null
    json_set_null(entry);
    ASSERT(json_get_type(entry) == JSON_NULL);
    
    json_free(entry);
}

// ==================== Edge Case Tests ====================

TEST(empty_input) {
    // Empty string
    json_entry *result = json_parse_string((char *)"");
    ASSERT(result == NULL);
    
    // Only whitespace
    result = json_parse_string((char *)"   \n\t  ");
    ASSERT(result == NULL);
}

// ==================== Run all tests ====================

int main() {
    printf("Running JSON library tests\n\n");
    
    RUN_TEST(parse_simple_object);
    RUN_TEST(parse_simple_array);
    RUN_TEST(parse_nested_structures);
    RUN_TEST(parse_all_types);
    RUN_TEST(accessor_functions);
    RUN_TEST(create_and_modify_object);
    RUN_TEST(create_and_modify_array);
    RUN_TEST(type_conversion);
    RUN_TEST(empty_input);
    
    printf("All tests passed!\n");
    return 0;
}