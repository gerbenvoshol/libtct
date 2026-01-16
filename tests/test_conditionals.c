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

/* Simple if condition - true */
TEST(if_true) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "show", "%s", "yes");
    
    char *result = tct_render("{{#if show}}visible{{/if}}", args);
    assert(strcmp(result, "visible") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Simple if condition - false (empty string) */
TEST(if_false_empty) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "show", "%s", "");
    
    char *result = tct_render("{{#if show}}visible{{/if}}", args);
    assert(strcmp(result, "") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Simple if condition - false (zero) */
TEST(if_false_zero) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "show", "%s", "0");
    
    char *result = tct_render("{{#if show}}visible{{/if}}", args);
    assert(strcmp(result, "") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Simple if condition - false (false string) */
TEST(if_false_string) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "show", "%s", "false");
    
    char *result = tct_render("{{#if show}}visible{{/if}}", args);
    assert(strcmp(result, "") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* If with else - true branch */
TEST(if_else_true) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "admin", "%s", "true");
    
    char *result = tct_render("{{#if admin}}Admin{{#else}}User{{/if}}", args);
    assert(strcmp(result, "Admin") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* If with else - false branch */
TEST(if_else_false) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "admin", "%s", "");
    
    char *result = tct_render("{{#if admin}}Admin{{#else}}User{{/if}}", args);
    assert(strcmp(result, "User") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* If with variable substitution inside */
TEST(if_with_substitution) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "show", "%s", "yes");
    tct_add_argument(args, "name", "%s", "John");
    
    char *result = tct_render("{{#if show}}Hello {{ name }}!{{/if}}", args);
    assert(strcmp(result, "Hello John!") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* If with text around it */
TEST(if_with_surrounding_text) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "show", "%s", "1");
    
    char *result = tct_render("Before {{#if show}}middle{{/if}} after", args);
    assert(strcmp(result, "Before middle after") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Nested if conditions */
TEST(nested_if) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "outer", "%s", "yes");
    tct_add_argument(args, "inner", "%s", "yes");
    
    char *result = tct_render("{{#if outer}}Outer {{#if inner}}Inner{{/if}}{{/if}}", args);
    assert(strcmp(result, "Outer Inner") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Nested if - outer true, inner false */
TEST(nested_if_partial) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "outer", "%s", "yes");
    tct_add_argument(args, "inner", "%s", "");
    
    char *result = tct_render("{{#if outer}}Outer {{#if inner}}Inner{{/if}}{{/if}}", args);
    assert(strcmp(result, "Outer ") == 0);
    
    tct_free_argument(args);
    free(result);
}

int main() {
    printf("=== Running Conditional Tests ===\n\n");
    
    run_test_if_true();
    run_test_if_false_empty();
    run_test_if_false_zero();
    run_test_if_false_string();
    run_test_if_else_true();
    run_test_if_else_false();
    run_test_if_with_substitution();
    run_test_if_with_surrounding_text();
    run_test_nested_if();
    run_test_nested_if_partial();
    
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
