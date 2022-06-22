#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  size_t used;
  size_t size;
  char *array;
} CharVector;

CharVector *new_charvector(size_t initialSize) {
  CharVector *a = (CharVector *)malloc(sizeof(*a));
  a->array = (char *)malloc(initialSize * sizeof(char));
  a->used = 0;
  a->size = initialSize;

  return a;
}

void insert_charvector(CharVector *a, char element) {
  if (a->used == a->size) {
    a->size *= 2;
    a->array = (char *)realloc(a->array, a->size * sizeof(char));
  }
  a->array[a->used++] = element;
}

void free_charvector(CharVector *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

char *string_repr(char *chars) {
  CharVector *repr = new_charvector(5);

  insert_charvector(repr, '"');

  for (int i = 0; chars[i] != '\0'; i++) {
    if (chars[i] == '\n') {
      insert_charvector(repr, '\\');
      insert_charvector(repr, 'n');
    } else if (chars[i] == '\t') {
      insert_charvector(repr, '\\');
      insert_charvector(repr, 't');
    } else if (chars[i] == '"') {
      insert_charvector(repr, '\\');
      insert_charvector(repr, '"');
    } else
      insert_charvector(repr, chars[i]);
  }

  insert_charvector(repr, '"');
  insert_charvector(repr, '\0');

  char *array = repr->array;

  free(repr);

  return array;
}

#endif