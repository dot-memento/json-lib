/**
 * @file test_framework.c
 * @brief Small test framework for the JSON library.
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#if !defined(_POSIX_C_SOURCE) && !defined(_DEFAULT_SOURCE) && \
    !defined(_BSD_SOURCE) && !defined(_SVID_SOURCE) && \
    (__STDC_VERSION__ < 202311L)

// Duplicates a string using dynamic allocation.
// Only define strdup if not available in the standard library.
char *strdup(const char *string)
{
    size_t size = strlen(string);
    char *string_copy = malloc(size + 1);
    return string_copy ? strcpy(string_copy, string) : NULL;
}

#endif

#define ASSERT(message, test) do { \
    tests_run++; \
    if (!(test)) { \
        fprintf(stderr, "FAIL: %s:%d: %s\n", __FILE__, __LINE__, message); \
        tests_failed++; \
    } \
} while (0)

#define ASSERT_NOT_EQUAL_INT(message, expected, actual) do { \
    tests_run++; \
    if ((expected) == (actual)) { \
        fprintf(stderr, "FAIL: %s:%d: %s (expected not %lli)\n", __FILE__, __LINE__, message, (long long)(expected)); \
        tests_failed++; \
    } \
} while (0)

#define ASSERT_EQUAL_INT(message, expected, actual) do { \
    tests_run++; \
    if ((expected) != (actual)) { \
        fprintf(stderr, "FAIL: %s:%d: %s (expected %lli, got %lli)\n", __FILE__, __LINE__, message, (long long)(expected), (long long)(actual)); \
        tests_failed++; \
    } \
} while (0)

#define ASSERT_EQUAL_DOUBLE(message, expected, actual) do { \
    tests_run++; \
    if (fabs((expected) - (actual)) > 0.000001) { \
        fprintf(stderr, "FAIL: %s:%d: %s (expected %f, got %f)\n", __FILE__, __LINE__, message, (expected), (actual)); \
        tests_failed++; \
    } \
} while (0)

#define ASSERT_EQUAL_STRING(message, expected, actual) do { \
    tests_run++; \
    if (!(actual) || strcmp((expected), (actual))) { \
        fprintf(stderr, "FAIL: %s:%d: %s (expected '%s', got '%s')\n", __FILE__, __LINE__, message, (expected), (actual)); \
        tests_failed++; \
    } \
} while (0)

#define ASSERT_NOT_EQUAL_PTR(message, expected, actual) do { \
    tests_run++; \
    if ((expected) == (actual)) { \
        fprintf(stderr, "FAIL: %s:%d: %s (expected not %p)\n", __FILE__, __LINE__, message, (expected)); \
        tests_failed++; \
    } \
} while (0)

#define ASSERT_EQUAL_PTR(message, expected, actual) do { \
    tests_run++; \
    if ((expected) != (actual)) { \
        fprintf(stderr, "FAIL: %s:%d: %s (expected %p, got %p)\n", __FILE__, __LINE__, message, (const void*)(expected), (const void*)(actual)); \
        tests_failed++; \
    } \
} while (0)

#define ASSERT_NULL(message, ptr) ASSERT(message, (ptr) == NULL)
#define ASSERT_NOT_NULL(message, ptr) ASSERT(message, (ptr) != NULL)

#define BEGIN_TESTS() do { \
    tests_run = 0; \
    tests_failed = 0; \
    clock_t _start_time = clock();

#define FINISH_TESTS() \
    clock_t _end_time = clock(); \
    double _elapsed = ((double)(_end_time - _start_time)) / CLOCKS_PER_SEC; \
    printf("\n--- Test Results ---\n"); \
    printf("Tests run: %d\n", tests_run); \
    if (tests_failed == 0) { \
        printf("All tests passed! (%0.3f seconds)\n", _elapsed); \
    } else { \
        printf("Tests failed: %d/%d (%0.3f seconds)\n", tests_failed, tests_run, _elapsed); \
    } \
    return tests_failed > 0 ? 1 : 0; \
} while (0)

// JSON specific assertions

#include "../src/json.h"

#define ASSERT_JSON_SUCCESS(message, result) ASSERT_EQUAL_INT(message, JSON_SUCCESS, (result))
#define ASSERT_JSON_ERROR(message, result, expected) ASSERT_EQUAL_INT(message, (expected), (result))

#define ASSERT_JSON_TYPE(message, value, expected) do { \
    json_type _type; \
    json_error _error = json_get_type(value, &_type); \
    ASSERT_JSON_SUCCESS(message" (error)", _error); \
    ASSERT_EQUAL_INT(message" (type)", expected, _type); \
} while (0)

#define ASSERT_JSON_GET_BOOL(message, value, expected) do { \
    bool _bool_val; \
    json_error _error = json_get_bool(value, &_bool_val); \
    ASSERT_JSON_SUCCESS(message" (error)", _error); \
    ASSERT_EQUAL_INT(message" (value)", expected, _bool_val); \
} while (0)

#define ASSERT_JSON_GET_NOT_BOOL(message, value) do { \
    bool _bool_val; \
    json_error _error = json_get_bool(value, &_bool_val); \
    ASSERT_EQUAL_INT(message, JSON_ERROR_WRONG_TYPE, _error); \
} while (0)

#define ASSERT_JSON_GET_NUMBER(message, value, expected) do { \
    double _num_val; \
    json_error _error = json_get_number(value, &_num_val); \
    ASSERT_JSON_SUCCESS(message" (error)", _error); \
    ASSERT_EQUAL_DOUBLE(message" (value)", expected, _num_val); \
} while (0)

#define ASSERT_JSON_GET_NOT_NUMBER(message, value) do { \
    double _num_val; \
    json_error _error = json_get_number(value, &_num_val); \
    ASSERT_EQUAL_INT(message, JSON_ERROR_WRONG_TYPE, _error); \
} while (0)

#define ASSERT_JSON_GET_STRING(message, value, expected) do { \
    const char *_str_val; \
    json_error _error = json_get_string(value, &_str_val); \
    ASSERT_JSON_SUCCESS(message" (error)", _error); \
    ASSERT_EQUAL_STRING(message" (value)", expected, _str_val); \
} while (0)

#define ASSERT_JSON_GET_NOT_STRING(message, value) do { \
    const char *_str_val; \
    json_error _error = json_get_string(value, &_str_val); \
    ASSERT_EQUAL_INT(message, JSON_ERROR_WRONG_TYPE, _error); \
} while (0)

#define ASSERT_JSON_GET_ARRAY(message, value, index, expected) do { \
    json_value *_val; \
    json_error _error = json_array_get(value, index, &_val); \
    ASSERT_JSON_SUCCESS(message" (error)", _error); \
    ASSERT_EQUAL_PTR(message" (value)", expected, _val); \
} while (0)

#define ASSERT_JSON_GET_NOT_ARRAY(message, value) do { \
    json_value *_val; \
    json_error _error = json_array_get(value, 0, &_val); \
    ASSERT_EQUAL_INT(message, JSON_ERROR_WRONG_TYPE, _error); \
} while (0)

#define ASSERT_JSON_ARRAY_LENGTH(message, value, expected) do { \
    size_t _length; \
    json_error _error = json_array_length(value, &_length); \
    ASSERT_JSON_SUCCESS(message, _error); \
    ASSERT_EQUAL_INT(message, expected, _length); \
} while (0)

#define ASSERT_JSON_ARRAY_OUT_OF_BOUNDS(message, value, index) do { \
    json_value *_val; \
    json_error _error = json_array_get(value, index, &_val); \
    ASSERT_EQUAL_INT(message, JSON_ERROR_INDEX_OUT_OF_BOUNDS, _error); \
} while (0)

#define ASSERT_JSON_GET_OBJECT(message, value, key, expected) do { \
    json_value *_val; \
    json_error _error = json_object_get(value, key, &_val); \
    ASSERT_JSON_SUCCESS(message" (error)", _error); \
    ASSERT_EQUAL_PTR(message" (value)", expected, _val); \
} while (0)

#define ASSERT_JSON_GET_NOT_OBJECT(message, value) do { \
    json_value *_val; \
    json_error _error = json_object_get(value, "key", &_val); \
    ASSERT_EQUAL_INT(message, JSON_ERROR_WRONG_TYPE, _error); \
} while (0)

#define ASSERT_JSON_OBJECT_SIZE(message, value, expected) do { \
    size_t _size; \
    json_error _error = json_object_size(value, &_size); \
    ASSERT_JSON_SUCCESS(message, _error); \
    ASSERT_EQUAL_INT(message, expected, _size); \
} while (0)

#define ASSERT_JSON_GET_OBJECT_NOT_FOUND(message, value, key) do { \
    json_value *_val; \
    json_error _error = json_object_get(value, key, &_val); \
    ASSERT_EQUAL_INT(message, JSON_ERROR_KEY_NOT_FOUND, _error); \
} while (0)

#endif // TEST_FRAMEWORK_H