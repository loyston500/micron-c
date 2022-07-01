#ifndef DISPLAY_H
#define DISPLAY_H

#include "scanner.h"
#include "tokenizer.h"

CharVector *format_error(CharVector *source, TokenizerErrorInfo *error) {
    int error_start = error->start;
    int error_end = error->end;
    int line_no = 0;
    int line_start = 0;
    int line_end = 0;
    int i;

    for (i = 0; i < error_start; i++)
        if (source->array[i] == '\n')
            line_no++;

    for (i = error_start; i >= 0; i--)
        if (source->array[i] == '\n')
            break;

    line_start = i;

    int start = error_start - line_start;
    int end = error_end - line_start;

    char *array = (char *)malloc(100);
    sprintf(array, "Line:%i:%i:%i\n", line_no + 1, start, end);
    CharVector *string = (CharVector *)malloc(sizeof(*string));
    string->array = array;
    string->size = 100;
    for (i = 0; array[i] != '\0'; i++)
        ;
    string->used = i;

    // prints the line
    for (i = line_start + 1; i < source->used; i++) {
        if (source->array[i] == '\n')
            break;
        insert_charvector(string, source->array[i]);
    }

    line_end = i;

    insert_charvector(string, '\n');

    // prints the arrow
    for (i = line_start + 1; i < source->used; i++) {
        if (source->array[i] == '\n')
            break;
        if (i >= error_start && i <= error_end)
            insert_charvector(string, '^');
        else
            insert_charvector(string, ' ');
    }

    insert_charvector(string, '\n');

    return string;
}

void display_error(CharVector *source, TokenizerErrorInfo *error) {
    CharVector *string = format_error(source, error);
    printf("%s", string->array);
}

#endif