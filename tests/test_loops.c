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

/* Simple each loop - with item */
TEST(each_with_item) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "items", "%s", "yes");
    
    char *result = tct_render("{{#each items}}Item{{/each}}", args);
    assert(strcmp(result, "Item") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Simple each loop - without item */
TEST(each_without_item) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "items", "%s", "");
    
    char *result = tct_render("{{#each items}}Item{{/each}}", args);
    assert(strcmp(result, "") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Each with variable substitution */
TEST(each_with_substitution) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "items", "%s", "yes");
    tct_add_argument(args, "item", "%s", "Apple");
    
    char *result = tct_render("{{#each items}}Item: {{ item }}{{/each}}", args);
    assert(strcmp(result, "Item: Apple") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Each with surrounding text */
TEST(each_with_surrounding_text) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "items", "%s", "1");
    
    char *result = tct_render("Before {{#each items}}middle{{/each}} after", args);
    assert(strcmp(result, "Before middle after") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Nested each loops */
TEST(nested_each) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "outer", "%s", "yes");
    tct_add_argument(args, "inner", "%s", "yes");
    
    char *result = tct_render("{{#each outer}}O{{#each inner}}I{{/each}}{{/each}}", args);
    assert(strcmp(result, "OI") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* Each with if inside */
TEST(each_with_if) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "items", "%s", "yes");
    tct_add_argument(args, "show", "%s", "yes");
    
    char *result = tct_render("{{#each items}}{{#if show}}Shown{{/if}}{{/each}}", args);
    assert(strcmp(result, "Shown") == 0);
    
    tct_free_argument(args);
    free(result);
}

/* If with each inside */
TEST(if_with_each) {
    tct_arguments *args = NULL;
    tct_add_argument(args, "show", "%s", "yes");
    tct_add_argument(args, "items", "%s", "yes");
    
    char *result = tct_render("{{#if show}}{{#each items}}Item{{/each}}{{/if}}", args);
    assert(strcmp(result, "Item") == 0);
    
    tct_free_argument(args);
    free(result);
}

int main() {
    printf("=== Running Loop Tests ===\n\n");
    
    run_test_each_with_item();
    run_test_each_without_item();
    run_test_each_with_substitution();
    run_test_each_with_surrounding_text();
    run_test_nested_each();
    run_test_each_with_if();
    run_test_if_with_each();
    
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
