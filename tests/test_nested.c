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

/* Complex nested template */
TEST(complex_nested) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "user", "%s", "admin");
    tct_add_argument(args, "is_admin", "%s", "yes");
    tct_add_argument(args, "has_items", "%s", "yes");
    tct_add_argument(args, "item_name", "%s", "Widget");
    
    char *template = 
        "User: {{ user }}\n"
        "{{#if is_admin}}"
        "Role: Admin\n"
        "{{#if has_items}}"
        "Items:\n"
        "{{#each has_items}}"
        "  - {{ item_name }}\n"
        "{{/each}}"
        "{{/if}}"
        "{{/if}}";
    
    char *result = tct_render(template, args);
    assert(strstr(result, "User: admin") != NULL);
    assert(strstr(result, "Role: Admin") != NULL);
    assert(strstr(result, "Items:") != NULL);
    assert(strstr(result, "- Widget") != NULL);
    
    tct_free_argument(args);
    free(result);
}

/* Nested if/else with substitution */
TEST(nested_if_else_substitution) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "level1", "%s", "yes");
    tct_add_argument(args, "level2", "%s", "");
    tct_add_argument(args, "name", "%s", "Test");
    
    char *result = tct_render(
        "{{#if level1}}"
        "L1: {{ name }} "
        "{{#if level2}}L2{{#else}}No L2{{/if}}"
        "{{/if}}", 
        args);
    assert(strcmp(result, "L1: Test No L2") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Multiple variables in nested blocks */
TEST(multiple_vars_nested) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "show_outer", "%s", "1");
    tct_add_argument(args, "outer_var", "%s", "Outer");
    tct_add_argument(args, "show_inner", "%s", "1");
    tct_add_argument(args, "inner_var", "%s", "Inner");
    
    char *result = tct_render(
        "{{#if show_outer}}"
        "{{ outer_var }}: "
        "{{#if show_inner}}{{ inner_var }}{{/if}}"
        "{{/if}}", 
        args);
    assert(strcmp(result, "Outer: Inner") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Three levels deep nesting */
TEST(three_levels_deep) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "l1", "%s", "yes");
    tct_add_argument(args, "l2", "%s", "yes");
    tct_add_argument(args, "l3", "%s", "yes");
    
    char *result = tct_render(
        "{{#if l1}}"
        "1"
        "{{#if l2}}"
        "2"
        "{{#if l3}}"
        "3"
        "{{/if}}"
        "{{/if}}"
        "{{/if}}", 
        args);
    assert(strcmp(result, "123") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Nested each with if */
TEST(nested_each_if_complex) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "outer_items", "%s", "yes");
    tct_add_argument(args, "show_name", "%s", "yes");
    tct_add_argument(args, "name", "%s", "Item1");
    tct_add_argument(args, "inner_items", "%s", "yes");
    
    char *result = tct_render(
        "{{#each outer_items}}"
        "{{#if show_name}}{{ name }}{{/if}}"
        "{{#each inner_items}}*{{/each}}"
        "{{/each}}", 
        args);
    assert(strcmp(result, "Item1*") == 0);
    
    tct_free_argument(args);
    free(result);
}

int main() {
    printf("=== Running Nested Template Tests ===\n\n");
    
    run_test_complex_nested();
    run_test_nested_if_else_substitution();
    run_test_multiple_vars_nested();
    run_test_three_levels_deep();
    run_test_nested_each_if_complex();
    
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
