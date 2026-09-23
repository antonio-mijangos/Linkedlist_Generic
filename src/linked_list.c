#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define MAX_ID 11

struct Node
{
    int64_t id;
    void *data;
    Printer print;
    Destroyer destroy;
    struct Node *next;
};

struct LinkedList
{
    struct Node *head;
    struct Node *tail;
};

ListStatus create_list(LinkedList **list);
struct Node *create_node(int64_t id, void *data, Printer print, Destroyer destroy);
void unlink_node(LinkedList *list, struct Node *previous, struct Node *current);

ListStatus insert_front(LinkedList *list, void *data, Copy copy, Printer print, Destroyer destroy);
ListStatus insert_back(LinkedList *list, void *data, Copy copy, Printer print, Destroyer destroy);
ListStatus insert_before_id(LinkedList *list, void *data, int64_t id_key, Copy copy, Printer print, Destroyer destroy);

ListStatus delete_front(LinkedList *list);
ListStatus delete_back(LinkedList *list);
ListStatus delete_by_id(LinkedList *list, int64_t id_key);

ListStatus destroy_list(LinkedList **list);

struct Node *search_node_by_id(LinkedList *list, int64_t id);

ListStatus print_data_by_id(LinkedList *list, int64_t id);
ListStatus print_list(LinkedList *list);

ListStatus is_list_empty(LinkedList *list);
ListStatus is_list_corrupted(LinkedList *list);

ListStatus is_id_available(LinkedList *list, int64_t id);

int64_t read_id();
int is_number(int c);

ListStatus create_list(LinkedList **list)
{
    if (list == NULL) return INVALID_ARGUMENT;
    if (*list != NULL) return LIST_EXISTING;

    *list = malloc(sizeof **list);

    if (*list == NULL) return MEMORY_ERROR;

    (*list)->head = NULL;
    (*list)->tail = NULL;
    return LIST_OK;
}

struct Node *create_node(int64_t id, void *data, Printer print, Destroyer destroy)
{

  struct Node *new_node = malloc(sizeof *new_node);
  if (new_node == NULL) return NULL;
  
  new_node->id = id;
  new_node->data = data;
  new_node->print = print;
  new_node->destroy = destroy;
  new_node->next = NULL;
  return new_node;
}

void unlink_node(LinkedList *list, struct Node *previous, struct Node *current)
{
  /*Case delete the only node of the list*/
  if (list->head == list->tail)
  {
    list->head = NULL;
    list->tail = NULL;
    return;
  }

  /*Case delete front*/
  if (previous == NULL)
  {
    list->head = list->head->next;
    return;
  }

  /*Case delete back*/
  if(current->next == NULL)
  {
    previous->next = NULL;
    list->tail = previous;
    return;
  }

  /*General case*/
  previous->next = current->next;
  return;
}

ListStatus insert_front(struct LinkedList *list, void *data, Copy copy, Printer print, Destroyer destroy)
{
  ListStatus status;
  if ((status = is_list_corrupted(list)) != LIST_OK) return status;
  if (copy == NULL) return INVALID_ARGUMENT;
  if (destroy == NULL) return INVALID_ARGUMENT;
  if (print == NULL) return INVALID_ARGUMENT;
  if (data == NULL) return INVALID_ARGUMENT;


  int64_t id = read_id();

  if (id == -1) return LIST_OK;

  while (is_id_available(list, id) != LIST_ID_AVAILABLE)
  {
    printf("The ID number is not available! Enter a different ID.\n\n");
    id = read_id();
    if (id == -1) return LIST_OK;
  }

  void *copy_data = NULL;

  status = copy(data, &copy_data);
  if (status != LIST_OK) return status;

  struct Node *new_node = create_node(id, copy_data, print, destroy);

  if (new_node == NULL)
  {
    destroy(copy_data);
    return MEMORY_ERROR;
  }

  if(is_list_empty(list) == LIST_EMPTY)
  {
    list->head = new_node;
    list->tail = new_node;
    return LIST_OK;
  }

  new_node->next = list->head;
  list->head = new_node;
  return LIST_OK;
}

ListStatus insert_back(LinkedList *list, void *data, Copy copy, Printer print, Destroyer destroy)
{
  ListStatus status;
  if ((status = is_list_corrupted(list)) != LIST_OK) return status;
  if (copy == NULL) return INVALID_ARGUMENT;
  if (destroy == NULL) return INVALID_ARGUMENT;
  if (print == NULL) return INVALID_ARGUMENT;
  if(data == NULL) return INVALID_ARGUMENT;


  int64_t id = read_id();
  if (id == -1) return LIST_OK;

  while (is_id_available(list, id) != LIST_ID_AVAILABLE)
  {
    printf("The ID number is not available! Enter a different ID.\n\n");
    id = read_id();
    if (id == -1) return LIST_OK;
  }

  void *copy_data = NULL;

  status = copy(data, &copy_data);
  if (status != LIST_OK) return status;

  struct Node *new_node = create_node(id, copy_data, print, destroy); 

  if (new_node == NULL)
  {
    destroy(copy_data);
    return MEMORY_ERROR;
  }

  if (is_list_empty(list) == LIST_EMPTY)
  {
    list->head = new_node;
    list->tail = new_node;
    return LIST_OK;
  }

  list->tail->next = new_node;
  list->tail = new_node;
  return LIST_OK;
}

ListStatus insert_before_id(LinkedList *list, void *data, int64_t id_key, Copy copy, Printer print, Destroyer destroy)
{
  ListStatus status;
  if ((status = is_list_corrupted(list)) != LIST_OK) return status;
  if (data == NULL) return INVALID_ARGUMENT;
  if (copy == NULL) return INVALID_ARGUMENT;
  if (destroy == NULL) return INVALID_ARGUMENT;
  if (print == NULL) return INVALID_ARGUMENT;

  if (is_list_empty(list) == LIST_EMPTY) return LIST_NOT_FOUND;
  
  if (is_id_available(list, id_key) != LIST_ID_NOT_AVAILABLE) return LIST_NOT_FOUND;

  int64_t id = read_id();
  if (id == -1) return LIST_OK;

  while (is_id_available(list, id) != LIST_ID_AVAILABLE)
  {
    printf("The ID number is not available! Enter a different ID.\n\n");
    id = read_id();
    if (id == -1) return LIST_OK;
  }

  void *copy_data = NULL;
  status = copy(data, &copy_data);
  if (status != LIST_OK) return status;

  struct Node *new_node = create_node(id, copy_data, print, destroy);

  if (new_node == NULL)
  {
    destroy(copy_data);
    return MEMORY_ERROR;
  }

  if (list->head->id == id_key)
  {
    new_node->next = list->head;
    list->head = new_node;
    return LIST_OK;
  }

  struct Node *previous, *current;
  previous = NULL;
  current = list->head;

  while (current != NULL)
  {
    if (current->id == id_key)
    {
      previous->next = new_node;
      new_node->next = current;
      return LIST_OK;
    }
    previous = current;
    current = current->next;
  }
  return LIST_NOT_FOUND;
}

ListStatus delete_front(LinkedList *list)
{
  ListStatus status;
  if ((status = is_list_corrupted(list)) != LIST_OK) return status;
  if (is_list_empty(list) == LIST_EMPTY) return LIST_NOT_FOUND;

  struct Node *current = list->head;

  unlink_node(list, NULL, current);
  current->destroy(current->data);
  free(current);
  return LIST_OK;
}

ListStatus delete_back(LinkedList *list)
{
  ListStatus status;
  if ((status = is_list_corrupted(list)) != LIST_OK) return status;
  if (is_list_empty(list) == LIST_EMPTY) return LIST_NOT_FOUND;

  struct Node *previous, *current;
  previous = NULL;
  current = list->head;

  while (current != NULL)
  {
    if (current == list->tail)
    {
      unlink_node(list, previous, current);
      current->destroy(current->data);
      free(current);
      return LIST_OK;
    }
    previous = current;
    current = current->next;
  }
  return LIST_NOT_FOUND;
}

ListStatus delete_by_id(struct LinkedList *list, int64_t id_key)
{
  ListStatus status;
  if ((status = is_list_corrupted(list)) != LIST_OK) return status;
  if (is_list_empty(list) == LIST_EMPTY) return LIST_NOT_FOUND;

  struct Node *previous, *current;
  previous = NULL;
  current = list->head;
  
  while (current != NULL)
  {
    if(current->id == id_key)
    {
      unlink_node(list, previous, current);
      current->destroy(current->data);
      free(current);
      return LIST_OK;
    }
    previous = current;
    current = current->next;
  }
  return LIST_NOT_FOUND;
}

ListStatus destroy_list(LinkedList **list)
{
  ListStatus status;
  if (list == NULL) return INVALID_ARGUMENT;
  if ((status = is_list_corrupted(*list)) != LIST_OK) return status;

  struct Node *current, *next;
  current = (*list)->head;

  while (current != NULL)
  {
    next = current->next;
    current->destroy(current->data);
    free(current);
    current = next;
  }

  free(*list);
  *list = NULL;
  return LIST_OK;
}

struct Node *search_node_by_id(LinkedList *list, int64_t id)
{
  struct Node *current = list->head;

  while (current != NULL)
  {
    if (current->id == id) return current;
    current = current->next;
  }
  return NULL;
}

ListStatus print_data_by_id(LinkedList *list, int64_t id)
{
  ListStatus status;
  if ((status = is_list_corrupted(list)) != LIST_OK) return status;
  if (is_list_empty(list) == LIST_EMPTY) return LIST_NOT_FOUND;

  struct Node *result = search_node_by_id(list, id);
  if (result == NULL) return LIST_NOT_FOUND;

  result->print(result->data);
  return LIST_OK;
}

ListStatus print_list(LinkedList *list)
{
  ListStatus status;
  if ((status = is_list_corrupted(list)) != LIST_OK) return status;
  if (is_list_empty(list) == LIST_EMPTY) return LIST_EMPTY;

  struct Node *current = list->head;

  while (current != NULL)
  {
    current->print(current->data);
    current = current->next;
  }
  return LIST_OK;
}

ListStatus is_list_empty(LinkedList *list)
{
  if(list == NULL) return INVALID_ARGUMENT;
  if (list->head == NULL && list->tail == NULL) return LIST_EMPTY;

  return LIST_OK;
}


ListStatus is_list_corrupted(struct LinkedList *list)
{
  /*If the list doesn{t exist then returns and error*/
  if (list == NULL) return INVALID_ARGUMENT;

  /*If the list is empty then the structure is consistent*/
  if (is_list_empty(list) == LIST_EMPTY) return LIST_OK;

  /*If one of the pointers is null while the other points to something then the list is corrupted*/
  if((list->head == NULL && list->tail != NULL) || (list->head!= NULL && list->tail == NULL)) return LIST_CORRUPTED;

  /*If the tail´s next points to something other than null then the list is corrupted*/
  if (list->tail->next != NULL) return LIST_CORRUPTED;

  /*Verifies if the list is not cycled using Floyd´s method*/
  struct Node *slow = list->head;
  struct Node *fast = list->head;

  while(fast != NULL && fast->next != NULL)
  {
    slow = slow->next;
    fast = fast->next->next;
    if (slow == fast) return LIST_CORRUPTED;
  }

  /*Starting from the list´s head, verifies if the list contains the list´s tail. List corrupted if not*/
  
   struct Node *current = list->head;
   while (current != NULL)
   {
    if(current == list->tail) return LIST_OK;
    current = current->next;
   }

  return LIST_CORRUPTED;
}

ListStatus is_id_available(LinkedList *list, int64_t id)
{
  if (is_list_empty(list) == LIST_EMPTY) return LIST_ID_AVAILABLE;

  struct Node *current;
  current = list->head;
  
  while (current != NULL)
  {
    if (current->id == id) return LIST_ID_NOT_AVAILABLE;
    current = current->next;
  }
  return LIST_ID_AVAILABLE;
}

int64_t read_id()
{
  int c, i, pass;
  char num[MAX_ID];
  long long value = 0;
  do
  {
    i = 0;
    pass = 1;

    printf("Enter an ID for the node (Only integers accepted, 10 digits max.): ");

    while ((c = getchar()) != EOF && c != '\n' && i < MAX_ID - 1)
    {
      num[i++] = (char)c;
    }

    num[i] = '\0';

    if (i == 0)
    {
      printf("No entry! Enter an integer id for the node.\n\n");
      pass = 0;
      continue;
    }

    if (i == MAX_ID - 1 && c != EOF && c != '\n')
    {
      while ((c = getchar()) != EOF && c != '\n'){}
      printf("The number entered is too long. %d digits max.\n\n", MAX_ID - 1);
      continue;
    }

    for(int j = 0; j < i; j++)
    {
      if (!is_number(num[j]))
      {
        printf("Only integers accepted!\n\n");
        pass = 0;
        break;
      }
    }


    if (pass == 1)
    {
      pass = 0;
      for(int j = 0; j < i; j++)
      {
        if (num[j] != '0')
        {
          pass = 1;
          break;
        }
      }
    }

    if (pass == 1)
    {
      errno = 0;
      value = strtoll(num, NULL, 10);

      if (errno == ERANGE)
      {
        printf("Overflow/Underflow!\n\n");
        pass = 0;
      }
    }
  } while (i == 0 || pass == 0);
  
  return (int64_t)value;
}

int is_number(int c)
{
  if (c >= '0' && c <= '9') return 1;

  return 0;
}