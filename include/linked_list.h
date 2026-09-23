#ifndef PROJECT_LINKED_LIST_H
#define PROJECT_LINKED_LIST_H

#include <stdint.h>

typedef struct LinkedList LinkedList;

typedef enum
{
  LIST_OK,
  LIST_EMPTY,
  MEMORY_ERROR,
  LIST_NOT_FOUND,
  LIST_CORRUPTED,
  INVALID_ARGUMENT,
  LIST_EXISTING,
  LIST_ID_AVAILABLE,
  LIST_ID_NOT_AVAILABLE
}ListStatus;



typedef void (*Destroyer)(void *);
typedef ListStatus (*Copy)(const void *, void **result);
typedef void (*Printer)(const void *data);


ListStatus create_list(LinkedList **list);

ListStatus insert_front(LinkedList *list, void *data, Copy copy, Printer print, Destroyer destroy);
ListStatus insert_back(LinkedList *list, void *data, Copy copy, Printer print, Destroyer destroy);
ListStatus insert_before_id(LinkedList *list, void *data, int64_t id_key, Copy copy, Printer print, Destroyer destroy);

ListStatus delete_front(LinkedList *list);
ListStatus delete_back(LinkedList *list);
ListStatus delete_by_id(LinkedList *list, int64_t id_key);


ListStatus destroy_list(LinkedList **list);

ListStatus print_data_by_id(LinkedList *list, int64_t id);
ListStatus print_list(LinkedList *list);


#endif