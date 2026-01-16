#include "../libtct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Test counter */
static int tests_run = 0;
static int tests_passed = 0;

#define TEST(name) \
    static void test_##name(); \
    static void run_test_##name() { \
        printf("Running test: %s ... ", #name); \
        tests_run++; \
        test_##name(); \
        tests_passed++; \
        printf("PASSED\n"); \
    } \
    static void test_##name()

/* Basic variable substitution test */
TEST(simple_substitution) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "name", "%s", "World");
    
    char *result = tct_render("Hello {{ name }}!", args);
    assert(strcmp(result, "Hello World!") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Multiple variable substitution */
TEST(multiple_substitution) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "first", "%s", "John");
    tct_add_argument(args, "last", "%s", "Doe");
    
    char *result = tct_render("{{ first }} {{ last }}", args);
    assert(strcmp(result, "John Doe") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Variable with whitespace in template */
TEST(whitespace_handling) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "var", "%s", "value");
    
    char *result = tct_render("{{  var  }}", args);
    assert(strcmp(result, "value") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Missing variable (should return empty string) */
TEST(missing_variable) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "exists", "%s", "yes");
    
    char *result = tct_render("{{ missing }}", args);
    assert(strcmp(result, "") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Integer formatting */
TEST(integer_formatting) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "count", "%d", 42);
    
    char *result = tct_render("Count: {{ count }}", args);
    assert(strcmp(result, "Count: 42") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Float formatting */
TEST(float_formatting) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "pi", "%.2f", 3.14159);
    
    char *result = tct_render("Pi: {{ pi }}", args);
    assert(strcmp(result, "Pi: 3.14") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Template with no variables */
TEST(no_variables) {
    tct_arguments *args = NULL;
    
    char *result = tct_render("Just plain text", args);
    assert(strcmp(result, "Just plain text") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Empty template */
TEST(empty_template) {
    tct_arguments *args = NULL;
    
    char *result = tct_render("", args);
    assert(strcmp(result, "") == 0);
    
    tct_free_argument(args);
    free(result);
}

int main() {
    printf("=== Running Basic Template Tests ===\n\n");
    
    run_test_simple_substitution();
    run_test_multiple_substitution();
    run_test_whitespace_handling();
    run_test_missing_variable();
    run_test_integer_formatting();
    run_test_float_formatting();
    run_test_no_variables();
    run_test_empty_template();
    
    printf("\n=== Test Results ===\n");
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_run - tests_passed);
    
    if (tests_passed == tests_run) {
        printf("\nAll tests PASSED!\n");
        return 0;
    } else {
        printf("\nSome tests FAILED!\n");
        return 1;
    }
}
