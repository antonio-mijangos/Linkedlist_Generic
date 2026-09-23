#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>


int assert_status(ListStatus actual, ListStatus expected, const char *function);
int assert_true(int condition, const char *function);

const char *status_to_string(ListStatus actual);

void test_create_list(void);
void test_create_list_existing(void);
void test_create_list_invalid_argument(void);

void test_insert_front(void);

ListStatus int_copy_function(const void *data, void **result);
void int_printer_function(const void *data);
void int_destroy_function(void *data);

int main(void)
{
    
}

int assert_status(ListStatus actual, ListStatus expected, const char *function)
{
    const char *result;
    result = "[FAIL]";
    if (actual == expected)
    {
        result = "[PASS]";
    }
    printf("%s %s : %s\n\nActual: %s\nExpected: %s", 
        result, function, status_to_string(actual), status_to_string(actual), status_to_string(expected));
    return actual == expected;
}

const char *status_to_string(ListStatus actual)
{
    switch (actual)
    {
        case LIST_OK:
            return "LIST OK";

        case LIST_CORRUPTED:
            return "LIST CORRUPTED";

        case LIST_EMPTY:
            return "LIST EMPTY";

        case LIST_EXISTING:
            return "LIST EXISTING";

        case LIST_NOT_FOUND:
            return "LIST NOT FOUND";

        case LIST_ID_AVAILABLE:
            return "LIST ID AVAILABLE";

        case LIST_ID_NOT_AVAILABLE:
            return "LIST ID NOT AVAILABLE";

         case MEMORY_ERROR:
            return "MEMORY ERROR";

         case INVALID_ARGUMENT:
            return "INVALID ARGUMENT";

        default:
            return "UNKNOWN STATUS";
    }
}

void test_create_list(void)
{
    LinkedList *list = NULL;
    ListStatus status;

    status = create_list(&list);

    assert_true(list != NULL, __func__);

    assert_status(status, LIST_OK, __func__);

    status = destroy_list(&list);

    assert_status(status, LIST_OK, __func__);

    assert_true(list == NULL, __func__);
}

void test_create_list_existing(void)
{
    LinkedList *list_original = NULL;
    ListStatus status;

    status = create_list(&list_original);

    assert_true(list_original != NULL, __func__);

    assert_status(status, LIST_OK, __func__);

    LinkedList *copy = list_original;

    status = create_list(&copy);

    assert_true(copy != NULL, __func__);

    assert_true(copy == list_original, __func__);

    assert_status(status, LIST_EXISTING, __func__);

    status = destroy_list(&list_original);

    assert_true(list_original == NULL, __func__);

    assert_status(status, LIST_OK, __func__);
}

void test_create_list_invalid_argument(void)
{
    ListStatus status;

    status = create_list(NULL);

    assert_status(status, INVALID_ARGUMENT, __func__);
}

/*void test_insert_front(void)
{
    LinkedList *list = NULL;
    ListStatus status;

    status = create_list(&list);

    assert_status(status, LIST_OK, __func__);

    assert_true(list != NULL, __func__);

    int data = 10;
    void *copy = NULL;
    status = int_copy_function(&data, &copy);

    assert_status(status, LIST_OK, __func__);

    assert_true(copy != NULL, __func__);

    status = insert_front(list, &copy, int_copy_function, int_printer_function, int_destroy_function);

    assert_true(list->head != NULL, __func__);

    assert_true(list->tail != NULL, __func__);

    assert_true(list->head == list->tail, __func__);

    assert_true(list->head->data != NULL, __func__);

    assert_true(list->)

    assert_true(list->head->id > 0, __func__);

    assert_true(list->head->print != NULL, __func__);

    assert_true(list->head->destroy != NULL, __func__);

    assert_true(list->head->next == NULL, __func__);

    assert_true(list->tail->next == NULL, __func__);

    assert_status(status, LIST_OK, __func__);

    status = destroy_list(&list);

    assert_status(status, LIST_OK, __func__);

    assert_true(list == NULL, __func__);
}*/

ListStatus int_copy_function(const void *data, void **result)
{
    if (data == NULL) return INVALID_ARGUMENT;
    if (result == NULL) return INVALID_ARGUMENT;

    *result = malloc(sizeof(int));

    if(*result == NULL)
    {
        return MEMORY_ERROR;
    }

    *(int *)*result = *(int *)data;
    return LIST_OK;
}

void int_printer_function(const void *data)
{
    if (data == NULL) return NULL;

    printf("%d", *(const int *)data);
}

void int_destroy_function(void *data)
{
    free(data);
}

int assert_true(int condition, const char *function)
{
    const char *result;
    result = "[FAIL]";
    if(condition)
    {
        result = "[PASS]";
    }

    printf("%s: %s", function, result);
    return condition;
}
