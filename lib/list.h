#include <stdlib.h>
#include <assert.h>
#pragma once

//
// IMPLEMENTATION
//

#ifdef BCPTK_ARRAYS 
//{

  #define foreach(ITEM,ARR,ACTION) \
    for(size_t i = 0; (ARR)[i] != 0; i++) \
      { ITEM = *((ARR)[i]); do { ACTION }  while(0); }
      
  #define array_init(ITYPE, SIZE) \
    ((ITYPE*) calloc((SIZE), sizeof(ITYPE)));

  #define array_print(FMT,ARR,SIZE)                          \
    for(size_t i = 0; i < (SIZE);i++) {                     \
      switch (i) {                                          \
        case 0       :printf("{ " FMT " ",(ARR)[i]); break; \
        case (SIZE)-1:printf(FMT " }\n"  ,(ARR)[i]); break; \
        default      :printf(FMT " "     ,(ARR)[i]);        \
      }                                                     \
    } 

  #define array_copy(DEST,SRC,SIZE)            \
    for(size_t i = 0; i < (SIZE); i++)        \
      (DEST)[i] = (SRC)[i];
  

  #endif



// 
// MAPS
//

typedef struct {
  char   *name ;
  char   *value;
} HMItem;

typedef struct  {
  HMItem *items;
  size_t (*hashf) (char*); 
  size_t map_size;
  size_t max_depth;
} HashMap;


unsigned long
djb2_hash(char *string)
{
  unsigned char *str = (unsigned char*)string;
  unsigned long hash = 5381;
  int c;

  while((c = *(str++))) 
    hash = ( (hash << 5) + hash ) + c;
  return hash;
}



HashMap mapinit(size_t size) {                             //
  return (HashMap) {                                       //
    .items = (HMItem*) calloc(size,sizeof(HMItem)),   //
      .hashf    = djb2_hash,                          //
      .map_size = (size)   ,                          //
      .max_depth= 1                                   //
  };     
}                          


// size_t map_tokenvalue()

// void map_append(HashMap *map, char* field, void* value) {
//     for(uint i = 0; i < map->max_depth; i++)  
//       if(map->values[i]) 
//       map->hashf(field)
//         ;
// }

// #define map_append(MAP,FIELD,VALUE) \
//   do {  \
//     for(uint i = 0; i < (MAP)->max_depth; i++)  \
//       if(!(MAP)->values[(MAP)->djb2_hash( (FIELD) )][i]) \
//         { (MAP)->values[(MAP)->djb2_hash( (FIELD) )][i] = calloc() }\
//       \
//   } while(0);


#ifdef BCPTK_LIST

#define LIST_NULL_ERROR "List isn't supposed to be null."

typedef struct Node {
  void* data;
  enum { Head, Body, Tail } node_role;
  char* node_type;
  struct Node *next;
  size_t  index ;
} Node;

typedef struct {
  Node    *first ;
  size_t   length;
} List;

List list_init() {
  return (List) {
    .first  = 0,
    .length = 0
  };
}

void free_node(Node* n) {
  if (!n)
    return;

  free_node(n->next);
  free(n);
}

void free_node_data(Node* n) {
  if (!n)
    return;

  free_node(n->next);
  free(n->data);
  free(n);
}

void list_destroy(List* l) 
{
  if (!l->first)
    return;
  free_node(l->first);
}

void list_free(List *l) {
  if (!l->first)
    return;
  free_node_data(l->first);
}


#define list_print(LIST,FMT,CAST_TYPE) \
do { \
  Node* item = (LIST)->first; \
  printf("{ "); \
  while (item) { \
    printf(FMT" ",(CAST_TYPE) item->data); \
    item = item->next; \
  } \
  printf("}\n"); \
} while(0)


void list_enum(List *l) {
  Node* item = l->first;
  size_t i = 0;
  while (item) {
    item->index = i;
    // printf("%p ",item);
    item = item->next;
    i++;
  }
}

Node* list_get_node(List *l,size_t i) {
  Node* item = l->first;
  while (item) 
    if (item->index == i)
      return item;
    else
      item = item->next;
  return 0;
}

void *list_get(List *l,size_t i) {
  Node* item = l->first;
  while (item) 
    if (item->index == i)
      return item->data;
    else
      item = item->next;
  return 0;
}


Node *node_init(void* data) {
    Node *temp = calloc(1,sizeof(Node));
    temp->data = data;
    temp->next = NULL;
    return temp;
}

void list_append(List *l, void *data){
  Node *node = l->first;

  if (!l->first)
    l->first = node_init(data);
  else {
    while(node->next)
      node = node->next;

    node->next = node_init(data);
  }
  l->length++;
  list_enum(l);
}

void list_pop_first(List *l) {
	if (l->length > 1) {
		Node *old_head = l->first;
		Node *new_head = l->first->next;
		
		assert(old_head != NULL);
		assert(new_head != NULL);

		new_head->node_role = Head;
		l->first = new_head;
		l->length--;

		free(old_head);
	}
	else if (l->length == 1) {
		free_node(l->first);
		l->first = NULL;
		l->length = 0;
	}
}

void list_pop_last(List *l) {
	assert(l!=NULL && LIST_NULL_ERROR);
	if (l->length > 1) {
		Node* last = list_get_node(l,l->length);
		Node* before = list_get_node(l,l->length-1); 
		
		free(last);
		before->next = NULL;
		l->length--;
	}
	
	else if (l->length == 1) {
		free(l->first);
		l->first = NULL;
		l->length--;
	}
}

void list_insert_after(List *l,size_t i,void *data) {
	assert(l!=NULL && LIST_NULL_ERROR);

	if (l->length < i)
		return;

	if (i == 0) {
		Node *old_root = l->first;
		l->first = node_init(data);
		l->first->next = old_root; 
	}
	else if (l->length == i) {
		Node *last = list_get_node(l,l->length);
		Node *new = node_init(data);
		new->next = NULL;
		last->next = new;
	}
	else {
		Node *right = list_get_node(l,i);
		Node *left  = list_get_node(l,i+1);

		Node *new = node_init(data);
		right->next = new;
		new->next = left;
	}
}

#endif
