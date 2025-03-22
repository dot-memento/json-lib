/**
 * @file test_basics.c
 * @brief Tests for the basic JSON value creation and modification functions.
 */

#include "test_framework.h"

static int tests_run = 0;
static int tests_failed = 0;

void test_null_creation_and_access()
{
    json_value *value = NULL;
    json_error error = json_null_create(&value);

    ASSERT_JSON_SUCCESS("Create null value", error);
    ASSERT_NOT_NULL("Null value pointer is not NULL", value);
    ASSERT_JSON_TYPE("Null value has correct type", value, JSON_NULL);
    ASSERT_JSON_GET_NOT_BOOL("Null value is not a bool", value);
    ASSERT_JSON_GET_NOT_NUMBER("Null value is not a number", value);
    ASSERT_JSON_GET_NOT_STRING("Null value is not a string", value);
    ASSERT_JSON_GET_NOT_ARRAY("Null value is not an array", value);
    ASSERT_JSON_GET_NOT_OBJECT("Null value is not an object", value);
    json_free(value);
}

void test_bool_creation_and_access()
{
    json_value *value = NULL;
    json_error error = json_bool_create(true, &value);

    ASSERT_JSON_SUCCESS("Create bool value (true)", error);
    ASSERT_NOT_NULL("Bool value pointer is not NULL", value);
    ASSERT_JSON_TYPE("Bool value has correct type", value, JSON_BOOL);
    ASSERT_JSON_GET_BOOL("Bool value is correct", value, true);
    ASSERT_JSON_GET_NOT_NUMBER("Bool value is not a number", value);
    ASSERT_JSON_GET_NOT_STRING("Bool value is not a string", value);
    ASSERT_JSON_GET_NOT_ARRAY("Bool value is not an array", value);
    ASSERT_JSON_GET_NOT_OBJECT("Bool value is not an object", value);
    json_free(value);
}

void test_number_creation_and_access()
{
    json_value *value = NULL;
    json_error error = json_number_create(42.5, &value);

    ASSERT_JSON_SUCCESS("Create number value", error);
    ASSERT_NOT_NULL("Number value pointer is not NULL", value);
    ASSERT_JSON_TYPE("Number value has correct type", value, JSON_NUMBER);
    ASSERT_JSON_GET_NOT_BOOL("Number value is not a bool", value);
    ASSERT_JSON_GET_NUMBER("Number value is correct", value, 42.5);
    ASSERT_JSON_GET_NOT_STRING("Number value is not a string", value);
    ASSERT_JSON_GET_NOT_ARRAY("Number value is not an array", value);
    ASSERT_JSON_GET_NOT_OBJECT("Number value is not an object", value);
    json_free(value);
}

void test_string_creation_and_access()
{
    const char *test_str = "Hello, JSON!";
    json_value *value = NULL;
    json_error error = json_string_create(test_str, &value);

    ASSERT_JSON_SUCCESS("Create string value", error);
    ASSERT_NOT_NULL("String value pointer is not NULL", value);
    ASSERT_JSON_TYPE("String value has correct type", value, JSON_STRING);
    ASSERT_JSON_GET_NOT_BOOL("String value is not a bool", value);
    ASSERT_JSON_GET_NOT_NUMBER("String value is not a number", value);
    ASSERT_JSON_GET_STRING("String value is correct", value, test_str);
    ASSERT_JSON_GET_NOT_ARRAY("String value is not an array", value);
    ASSERT_JSON_GET_NOT_OBJECT("String value is not an object", value);

    const char *str_val;
    error = json_get_string(value, &str_val);

    ASSERT_JSON_SUCCESS("Get string from value", error);
    ASSERT_NOT_EQUAL_PTR("String value is copied", test_str, str_val);
    json_free(value);
}

void test_string_nocopy_creation_and_access()
{
    char *nocopy_str = strdup("No Copy String");
    json_value *value = NULL;
    json_error error = json_string_create_nocopy(nocopy_str, &value);

    ASSERT_JSON_SUCCESS("Create string value (nocopy)", error);
    ASSERT_NOT_NULL("String value (nocopy) pointer is not NULL", value);
    ASSERT_JSON_TYPE("String value (nocopy) has correct type", value, JSON_STRING);
    ASSERT_JSON_GET_NOT_BOOL("String value (nocopy) is not a bool", value);
    ASSERT_JSON_GET_NOT_NUMBER("String value (nocopy) is not a number", value);
    ASSERT_JSON_GET_STRING("String value (nocopy) is correct", value, "No Copy String");

    const char *str_val;
    error = json_get_string(value, &str_val);

    ASSERT_JSON_SUCCESS("Get string from value (nocopy)", error);
    ASSERT_EQUAL_PTR("String value (nocopy) is not copied", nocopy_str, str_val);
    json_free(value);
}

void test_change_to_null()
{
    json_value *value = NULL;
    json_bool_create(true, &value);

    json_error error = json_change_to_null(value);

    ASSERT_JSON_SUCCESS("Change to null", error);
    ASSERT_JSON_TYPE("Null value has correct type", value, JSON_NULL);
    json_free(value);
}

void test_change_to_bool()
{
    json_value *value = NULL;
    json_null_create(&value);

    json_error error = json_change_to_bool(value, true);

    ASSERT_JSON_SUCCESS("Change to bool", error);
    ASSERT_JSON_TYPE("Bool value has correct type", value, JSON_BOOL);
    ASSERT_JSON_GET_BOOL("Bool value after change is correct", value, true);
    json_free(value);
}

void test_change_to_number()
{
    json_value *value = NULL;
    json_null_create(&value);

    json_error error = json_change_to_number(value, 123.45);

    ASSERT_JSON_SUCCESS("Change to number", error);
    ASSERT_JSON_TYPE("Number value has correct type", value, JSON_NUMBER);
    ASSERT_JSON_GET_NUMBER("Number value after change is correct", value, 123.45);
    json_free(value);
}

void test_change_to_string()
{
    const char *test_str = "Modified String";
    json_value *value = NULL;
    json_null_create(&value);

    json_error error = json_change_to_string(value, test_str);

    ASSERT_JSON_SUCCESS("Change to string", error);
    ASSERT_JSON_TYPE("String value has correct type", value, JSON_STRING);
    ASSERT_JSON_GET_STRING("String value after change is correct", value, test_str);
    json_free(value);
}

void test_change_to_string_nocopy()
{
    char *nocopy_str = strdup("No Copy Modified String");
    json_value *value = NULL;
    json_null_create(&value);

    json_error error = json_change_to_string_nocopy(value, nocopy_str);

    ASSERT_JSON_SUCCESS("Change to string (nocopy)", error);
    ASSERT_JSON_TYPE("String value (nocopy) has correct type", value, JSON_STRING);
    ASSERT_JSON_GET_STRING("String value (nocopy) after change is correct", value, "No Copy Modified String");
    json_free(value);
}

void test_null_clone()
{
    json_value *value = NULL;
    json_null_create(&value);

    json_value *clone = NULL;
    json_error error = json_clone(value, &clone);

    ASSERT_JSON_SUCCESS("Clone null value", error);
    ASSERT_NOT_NULL("Cloned value pointer is not NULL", clone);
    ASSERT_JSON_TYPE("Cloned value has correct type", clone, JSON_NULL);
    json_free(value);
    json_free(clone);
}

void test_bool_clone()
{
    json_value *value = NULL;
    json_bool_create(true, &value);

    json_value *clone = NULL;
    json_error error = json_clone(value, &clone);

    ASSERT_JSON_SUCCESS("Clone bool value", error);
    ASSERT_NOT_NULL("Cloned value pointer is not NULL", clone);
    ASSERT_JSON_TYPE("Cloned value has correct type", clone, JSON_BOOL);
    ASSERT_JSON_GET_BOOL("Cloned value is correct", clone, true);
    json_free(value);
    json_free(clone);
}

void test_number_clone()
{
    json_value *value = NULL;
    json_number_create(42.5, &value);

    json_value *clone = NULL;
    json_error error = json_clone(value, &clone);

    ASSERT_JSON_SUCCESS("Clone number value", error);
    ASSERT_NOT_NULL("Cloned value pointer is not NULL", clone);
    ASSERT_JSON_TYPE("Cloned value has correct type", clone, JSON_NUMBER);
    ASSERT_JSON_GET_NUMBER("Cloned value is correct", clone, 42.5);
    json_free(value);
    json_free(clone);
}

void test_string_clone()
{
    const char *test_str = "Hello, JSON!";
    json_value *value = NULL;
    json_string_create(test_str, &value);

    json_value *clone = NULL;
    json_error error = json_clone(value, &clone);

    ASSERT_JSON_SUCCESS("Clone string value", error);
    ASSERT_NOT_NULL("Cloned value pointer is not NULL", clone);
    ASSERT_JSON_TYPE("Cloned value has correct type", clone, JSON_STRING);
    ASSERT_JSON_GET_STRING("Cloned value is correct", clone, test_str);
    json_free(value);
    json_free(clone);
}

void test_null_errors_creation()
{
    json_value *value;
    json_number_create(123.45, &value);
    json_value *original_value = value;

    json_error error = json_null_create(NULL);
    ASSERT_EQUAL_INT("Null output pointer causes error", JSON_ERROR_NULL, error);

    error = json_bool_create(true, NULL);
    ASSERT_EQUAL_INT("Null output pointer causes error", JSON_ERROR_NULL, error);

    error = json_number_create(42.5, NULL);
    ASSERT_EQUAL_INT("Null output pointer causes error", JSON_ERROR_NULL, error);

    error = json_string_create("test", NULL);
    ASSERT_EQUAL_INT("Null output pointer causes error", JSON_ERROR_NULL, error);

    char *nocopy_str = strdup("No Copy String");
    error = json_string_create_nocopy(nocopy_str, NULL);
    ASSERT_EQUAL_INT("Null output pointer causes error", JSON_ERROR_NULL, error);
    free(nocopy_str);

    error = json_string_create(NULL, &value);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    ASSERT_EQUAL_PTR("Value pointer is unchanged", original_value, value);

    error = json_string_create_nocopy(NULL, &value);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    ASSERT_EQUAL_PTR("Value pointer is unchanged", original_value, value);
    free(value);
}

void test_null_errors_access()
{
    bool bool_val = true;
    json_error error = json_get_bool(NULL, &bool_val);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    ASSERT_EQUAL_INT("Bool value is unchanged", true, bool_val);

    double num_val = 420.05;
    error = json_get_number(NULL, &num_val);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    ASSERT_EQUAL_DOUBLE("Number value is unchanged", 420.05, num_val);

    const char *str_val = "Unchanged";
    error = json_get_string(NULL, &str_val);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    ASSERT_EQUAL_STRING("String value is unchanged", "Unchanged", str_val);

    json_value *value = (json_value*)0x12345678;

    error = json_get_bool(value, NULL);
    ASSERT_EQUAL_INT("Null output pointer causes error", JSON_ERROR_NULL, error);

    error = json_get_number(value, NULL);
    ASSERT_EQUAL_INT("Null output pointer causes error", JSON_ERROR_NULL, error);

    error = json_get_string(value, NULL);
    ASSERT_EQUAL_INT("Null output pointer causes error", JSON_ERROR_NULL, error);
}

void test_null_errors_modification()
{
    json_value *value;
    json_number_create(123.45, &value);

    json_error error = json_change_to_null(NULL);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);

    error = json_change_to_bool(NULL, true);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);

    error = json_change_to_number(NULL, 42.5);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);

    error = json_change_to_string(NULL, "test");
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);

    char *nocopy_str = strdup("No Copy String");
    error = json_change_to_string_nocopy(NULL, nocopy_str);
    ASSERT_EQUAL_INT("Null input pointer causes error", JSON_ERROR_NULL, error);
    free(nocopy_str);

    error = json_change_to_string(value, NULL);
    ASSERT_EQUAL_INT("Null string pointer causes error", JSON_ERROR_NULL, error);
    ASSERT_JSON_GET_NUMBER("Value is unchanged", value, 123.45);

    error = json_change_to_string_nocopy(value, NULL);
    ASSERT_EQUAL_INT("Null string pointer causes error", JSON_ERROR_NULL, error);
    ASSERT_JSON_GET_NUMBER("Value is unchanged", value, 123.45);
    free(value);
}

int main()
{
    BEGIN_TESTS();

    test_null_creation_and_access();
    test_bool_creation_and_access();
    test_number_creation_and_access();
    test_string_creation_and_access();
    test_string_nocopy_creation_and_access();
    
    test_change_to_null();
    test_change_to_bool();
    test_change_to_number();
    test_change_to_string();
    test_change_to_string_nocopy();

    test_null_clone();
    test_bool_clone();
    test_number_clone();
    test_string_clone();
    
    test_null_errors_creation();
    test_null_errors_access();
    test_null_errors_modification();

    FINISH_TESTS();
}