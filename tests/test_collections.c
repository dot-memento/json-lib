/**
 * @file test_collections.c
 * @brief Tests for the JSON array and object collection functions.
 */

#include "test_framework.h"

static int tests_run = 0;
static int tests_failed = 0;

void test_array_creation()
{
    json_value *array;
    json_error error = json_array_create(&array);

    ASSERT_JSON_SUCCESS("Create array", error);
    ASSERT_NOT_NULL("Array value pointer is not NULL", array);
    ASSERT_JSON_TYPE("Array value has correct type", array, JSON_ARRAY);
    ASSERT_JSON_GET_NOT_BOOL("Array value is not a bool", array);
    ASSERT_JSON_GET_NOT_NUMBER("Array value is not a number", array);
    ASSERT_JSON_GET_NOT_STRING("Array value is not a string", array);
    ASSERT_JSON_GET_NOT_OBJECT("Array value is not an object", array);
    ASSERT_JSON_ARRAY_LENGTH("Initial array length is 0", array, 0);
    json_free(array);
}

void test_change_to_array()
{
    json_value *value;
    json_null_create(&value);

    json_error error = json_set_as_array(value);

    ASSERT_JSON_SUCCESS("Change to array", error);
    ASSERT_JSON_TYPE("Array value has correct type", value, JSON_ARRAY);
    ASSERT_JSON_GET_NOT_BOOL("Array value is not a bool", value);
    ASSERT_JSON_GET_NOT_NUMBER("Array value is not a number", value);
    ASSERT_JSON_GET_NOT_STRING("Array value is not a string", value);
    ASSERT_JSON_GET_NOT_OBJECT("Array value is not an object", value);
    ASSERT_JSON_ARRAY_LENGTH("Initial array length is 0", value, 0);
    json_free(value);
}

void test_array_append_and_access()
{
    json_value *first_item, *second_item, *array;
    json_number_create(42.5, &first_item);
    json_bool_create(true, &second_item);
    json_array_create(&array);
    
    json_error error = json_array_append(array, first_item);

    ASSERT_JSON_SUCCESS("Append number to array", error);
    ASSERT_JSON_ARRAY_LENGTH("Array length after appending is 1", array, 1);

    error = json_array_append(array, second_item);

    ASSERT_JSON_SUCCESS("Append bool to array", error);
    ASSERT_JSON_ARRAY_LENGTH("Array length after appending is 2", array, 2);

    ASSERT_JSON_GET_ARRAY("Get first item from array", array, 0, first_item);
    ASSERT_JSON_GET_NUMBER("First item has correct value", first_item, 42.5);
    ASSERT_JSON_GET_ARRAY("Get second item from array", array, 1, second_item);
    ASSERT_JSON_GET_BOOL("Second item has correct value", second_item, true);
    json_free(array);
}

void test_array_insert()
{
    json_value *first_insert, *second_insert, *array;
    json_number_create(42.5, &first_insert);
    json_bool_create(true, &second_insert);
    json_array_create(&array);
    
    json_error error = json_array_insert(array, 0, first_insert);

    ASSERT_JSON_SUCCESS("Insert number to array", error);
    ASSERT_JSON_ARRAY_LENGTH("Array length after inserting is 1", array, 1);

    error = json_array_insert(array, 0, second_insert);

    ASSERT_JSON_SUCCESS("Append bool to array", error);
    ASSERT_JSON_ARRAY_LENGTH("Array length after inserting is 2", array, 2);

    ASSERT_JSON_GET_ARRAY("Get first item from array", array, 1, first_insert);
    ASSERT_JSON_GET_NUMBER("First item has correct value", first_insert, 42.5);
    ASSERT_JSON_GET_ARRAY("Get second item from array", array, 0, second_insert);
    ASSERT_JSON_GET_BOOL("Second item has correct value", second_insert, true);
    json_free(array);
}

void test_array_set()
{
    json_value *first_item, *second_item, *array;
    json_number_create(42.5, &first_item);
    json_bool_create(true, &second_item);
    json_array_create(&array);
    json_array_append(array, first_item);
    json_array_append(array, second_item);

    json_value *new_item;
    json_string_create("inserted", &new_item);
    json_error error = json_array_set(array, 1, new_item);

    ASSERT_JSON_SUCCESS("Set string in array", error);
    ASSERT_JSON_GET_ARRAY("Get second item from array", array, 1, new_item);
    ASSERT_JSON_GET_STRING("Second item has correct value", new_item, "inserted");
    json_free(array);
}

void test_array_clone()
{
    json_value *array, *item;
    json_number_create(42.5, &item);
    json_array_create(&array);
    json_array_append(array, item);

    json_value *clone;
    json_error error = json_clone(array, &clone);

    ASSERT_JSON_SUCCESS("Clone array", error);
    ASSERT_JSON_TYPE("Cloned value has correct type", clone, JSON_ARRAY);
    ASSERT_JSON_ARRAY_LENGTH("Cloned array has correct length", clone, 1);

    json_value *cloned_item;
    error = json_array_get(clone, 0, &cloned_item);

    ASSERT_JSON_SUCCESS("Get item from cloned array", error);
    ASSERT_JSON_GET_NUMBER("Cloned item has correct value", cloned_item, 42.5);
    ASSERT_NOT_EQUAL_PTR("Cloned item is not the same as the original", item, cloned_item);

    json_free(array);
    json_free(clone);
}

void test_array_remove()
{
    json_value *first_item, *second_item, *array;
    json_number_create(42.5, &first_item);
    json_string_create("inserted", &second_item);
    json_array_create(&array);
    json_array_append(array, first_item);
    json_array_append(array, second_item);

    json_value *removed_item;
    json_error error = json_array_remove(array, 1, &removed_item);

    ASSERT_JSON_SUCCESS("Remove second item from array", error);
    ASSERT_JSON_ARRAY_LENGTH("Array length after removal is 1", array, 1);
    ASSERT_JSON_GET_STRING("Removed item has correct value", removed_item, "inserted");
    
    error = json_array_remove(array, 0, NULL);
    
    ASSERT_JSON_SUCCESS("Remove first item from array", error);
    ASSERT_JSON_ARRAY_LENGTH("Array length after removal is 0", array, 0);
    
    json_free(removed_item);
    json_free(array);
}

void test_array_errors()
{
    json_value *array, *item;
    json_array_create(&array);
    json_number_create(42.5, &item);

    json_error error = json_array_create(NULL);
    ASSERT_EQUAL_INT("Null output pointer causes error", JSON_ERROR_NULL, error);

    error = json_set_as_array(NULL);
    ASSERT_EQUAL_INT("Null output pointer causes error", JSON_ERROR_NULL, error);
    
    error = json_array_append(NULL, item);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    error = json_array_append(array, NULL);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);

    error = json_array_insert(NULL, 0, item);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    error = json_array_insert(array, 0, NULL);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    error = json_array_insert(array, 1, item);
    ASSERT_EQUAL_INT("Out of bounds insert returns error", JSON_ERROR_INDEX_OUT_OF_BOUNDS, error);

    error = json_array_get(NULL, 0, &item);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    error = json_array_get(array, 0, NULL);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    error = json_array_get(array, 0, &item);
    ASSERT_EQUAL_INT("Out of bounds access returns error", JSON_ERROR_INDEX_OUT_OF_BOUNDS, error);

    size_t length;
    error = json_array_length(NULL, &length);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    error = json_array_length(array, NULL);
    ASSERT_EQUAL_INT("Null output pointer causes error", JSON_ERROR_NULL, error);

    error = json_array_set(NULL, 0, item);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    error = json_array_set(array, 0, NULL);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    error = json_array_set(array, 0, item);
    ASSERT_EQUAL_INT("Out of bounds modification returns error", JSON_ERROR_INDEX_OUT_OF_BOUNDS, error);

    error = json_array_remove(NULL, 0, &item);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    error = json_array_remove(array, 0, &item);
    ASSERT_EQUAL_INT("Out of bounds removal returns error", JSON_ERROR_INDEX_OUT_OF_BOUNDS, error);
    
    json_free(array);
    json_free(item);
}

void test_object_creation()
{
    json_value *object;
    json_error error = json_object_create(&object);

    ASSERT_JSON_SUCCESS("Create object", error);
    ASSERT_NOT_NULL("Object value pointer is not NULL", object);
    ASSERT_JSON_TYPE("Object value has correct type", object, JSON_OBJECT);
    ASSERT_JSON_GET_NOT_BOOL("Object value is not a bool", object);
    ASSERT_JSON_GET_NOT_NUMBER("Object value is not a number", object);
    ASSERT_JSON_GET_NOT_STRING("Object value is not a string", object);
    ASSERT_JSON_GET_NOT_ARRAY("Object value is not an array", object);
    ASSERT_JSON_OBJECT_SIZE("Initial object size is 0", object, 0);
    json_free(object);
}

void test_change_to_object()
{
    json_value *value;
    json_null_create(&value);

    json_error error = json_set_as_object(value);

    ASSERT_JSON_SUCCESS("Change to object", error);
    ASSERT_JSON_TYPE("Object value has correct type", value, JSON_OBJECT);
    ASSERT_JSON_GET_NOT_BOOL("Object value is not a bool", value);
    ASSERT_JSON_GET_NOT_NUMBER("Object value is not a number", value);
    ASSERT_JSON_GET_NOT_STRING("Object value is not a string", value);
    ASSERT_JSON_GET_NOT_ARRAY("Object value is not an array", value);
    ASSERT_JSON_OBJECT_SIZE("Initial object size is 0", value, 0);
    json_free(value);
}

void test_object_set()
{
    json_value *first_item, *second_item, *object;
    json_number_create(42.5, &first_item);
    json_bool_create(true, &second_item);
    json_object_create(&object);
    
    json_error error = json_object_set(object, "first", first_item);

    ASSERT_JSON_SUCCESS("Set number in object", error);
    ASSERT_JSON_OBJECT_SIZE("Object size after setting is 1", object, 1);

    error = json_object_set(object, "second", second_item);

    ASSERT_JSON_SUCCESS("Set bool in object", error);
    ASSERT_JSON_OBJECT_SIZE("Object size after setting is 2", object, 2);

    ASSERT_JSON_GET_OBJECT("Get first item from object", object, "first", first_item);
    ASSERT_JSON_GET_NUMBER("First item has correct value", first_item, 42.5);
    ASSERT_JSON_GET_OBJECT("Get second item from object", object, "second", second_item);
    ASSERT_JSON_GET_BOOL("Second item has correct value", second_item, true);
    json_free(object);
}

void test_object_has_key()
{
    json_value *first_item, *second_item, *object;
    json_number_create(42.5, &first_item);
    json_bool_create(true, &second_item);
    json_object_create(&object);
    json_object_set(object, "first", first_item);
    json_object_set(object, "second", second_item);

    bool has_key;
    json_error error = json_object_has_key(object, "first", &has_key);

    ASSERT_JSON_SUCCESS("Check has_key for existing key", error);
    ASSERT_EQUAL_INT("has_key returns true for existing key", true, has_key);

    error = json_object_has_key(object, "missing", &has_key);

    ASSERT_JSON_SUCCESS("Check has_key for non-existing key", error);
    ASSERT_EQUAL_INT("has_key returns false for non-existing key", false, has_key);
    json_free(object);
}

void test_object_clone()
{
    json_value *object, *item;
    json_number_create(42.5, &item);
    json_object_create(&object);
    json_object_set(object, "first", item);

    json_value *clone;
    json_error error = json_clone(object, &clone);

    ASSERT_JSON_SUCCESS("Clone object", error);
    ASSERT_JSON_TYPE("Cloned value has correct type", clone, JSON_OBJECT);
    ASSERT_JSON_OBJECT_SIZE("Cloned object has correct size", clone, 1);

    json_value *cloned_item;
    error = json_object_get(clone, "first", &cloned_item);

    ASSERT_JSON_SUCCESS("Get item from cloned object", error);
    ASSERT_JSON_GET_NUMBER("Cloned item has correct value", cloned_item, 42.5);
    ASSERT_NOT_EQUAL_PTR("Cloned item is not the same as the original", item, cloned_item);

    json_free(object);
    json_free(clone);
}

void test_object_remove()
{
    json_value *first_item, *second_item, *object;
    json_number_create(42.5, &first_item);
    json_string_create("inserted", &second_item);
    json_object_create(&object);
    json_object_set(object, "first", first_item);
    json_object_set(object, "second", second_item);

    json_value *removed_item;
    json_error error = json_object_remove(object, "second", &removed_item);

    ASSERT_JSON_SUCCESS("Remove second item from object", error);
    ASSERT_JSON_OBJECT_SIZE("Object size after removal is 1", object, 1);
    ASSERT_JSON_GET_STRING("Removed item has correct value", removed_item, "inserted");

    error = json_object_remove(object, "first", NULL);

    ASSERT_JSON_SUCCESS("Remove first item from object", error);
    ASSERT_JSON_OBJECT_SIZE("Object size after removal is 0", object, 0);

    json_free(removed_item);
    json_free(object);
}

void test_object_errors()
{
    json_value *object, *item;
    json_object_create(&object);
    json_number_create(42.5, &item);

    json_error error = json_object_create(NULL);
    ASSERT_EQUAL_INT("Null output pointer causes error", JSON_ERROR_NULL, error);

    error = json_set_as_object(NULL);
    ASSERT_EQUAL_INT("Null output pointer causes error", JSON_ERROR_NULL, error);
    
    error = json_object_set(NULL, "key", item);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    error = json_object_set(object, NULL, item);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    error = json_object_set(object, "key", NULL);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);

    bool has_key;
    error = json_object_has_key(NULL, "key", &has_key);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    error = json_object_has_key(object, NULL, &has_key);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    error = json_object_has_key(object, "key", NULL);
    ASSERT_EQUAL_INT("Null output pointer causes error", JSON_ERROR_NULL, error);

    error = json_object_get(NULL, "key", &item);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    error = json_object_get(object, NULL, &item);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    error = json_object_get(object, "key", NULL);
    ASSERT_EQUAL_INT("Null output pointer causes error", JSON_ERROR_NULL, error);
    error = json_object_get(object, "missing", &item);
    ASSERT_EQUAL_INT("Key not found returns error", JSON_ERROR_KEY_NOT_FOUND, error);

    error = json_object_remove(NULL, "key", &item);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    error = json_object_remove(object, NULL, &item);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    error = json_object_remove(object, "missing", &item);
    ASSERT_EQUAL_INT("Key not found returns error", JSON_ERROR_KEY_NOT_FOUND, error);
    
    json_free(object);
    json_free(item);
}

int main() {
    BEGIN_TESTS();

    test_array_creation();
    test_change_to_array();
    test_array_append_and_access();
    test_array_insert();
    test_array_set();
    test_array_clone();
    test_array_remove();
    test_array_errors();

    test_object_creation();
    test_change_to_object();
    test_object_set();
    test_object_has_key();
    test_object_clone();
    test_object_remove();
    test_object_errors();

    FINISH_TESTS();
}