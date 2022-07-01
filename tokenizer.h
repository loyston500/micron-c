#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    Str, // String Literal
    Int, // Integer
    Idn, // Identifier
    Til, // ~
    Col, // :
    Smi, // ;
    Dot, // .
    Eol, // \n
    Dol, // $
    Que, // ?
    Eql, // =
    Not, // !
    Hsh, // #
} TokenType;

static const char *const TOKEN_TYPE[] = {
    [Str] = "Str", [Int] = "Int", [Idn] = "Idn", [Til] = "Til", [Col] = "Col",
    [Smi] = "Smi", [Dot] = "Dot", [Eol] = "Eol", [Dol] = "Dol", [Que] = "Que",
    [Eql] = "Eql", [Not] = "Not", [Hsh] = "Hsh",
};

typedef struct {
    char *c;
    long int i;
} TokenData;

TokenData *str_tokendata(char *c) {
    TokenData *data = (TokenData *)malloc(sizeof(*data));
    data->c = c;
    // data->i = NULL;
    return data;
}

TokenData *int_tokendata(long int i) {
    TokenData *data = (TokenData *)malloc(sizeof(*data));
    data->i = i;
    data->c = NULL;
    return data;
}

void free_tokendata(TokenData *data) { free(data); }

typedef struct {
    TokenType type;
    TokenData *data;
} Token;

Token *new_token(TokenType type, TokenData *data) {
    Token *token = (Token *)malloc(sizeof(*token));

    token->type = type;
    token->data = data;

    return token;
}

void free_token(Token *token) {
    free_tokendata(token->data);
    free(token);
}

// NOTE: TOKEN INFO
typedef struct {
    Token *token;
    int start;
    int end;
} TokenInfo;

TokenInfo *new_tokeninfo(Token *token, int start, int end) {
    TokenInfo *token_info = (TokenInfo *)malloc(sizeof(*token_info));

    token_info->token = token;
    token_info->start = start;
    token_info->end = end;

    return token_info;
}

void free_tokeninfo(TokenInfo *token_info) {
    free_token(token_info->token);
    free(token_info);
}

void print_tokeninfo(TokenInfo *token_info) {

    if (token_info->token->data == NULL)
        printf("(type=%i name=%s start=%i end=%i)\n", token_info->token->type,
               TOKEN_TYPE[token_info->token->type], token_info->start,
               token_info->end);
    else if (token_info->token->data->c == NULL)
        printf("(type=%i name=%s int=%li start=%i end=%i)\n",
               token_info->token->type, TOKEN_TYPE[token_info->token->type],
               token_info->token->data->i, token_info->start, token_info->end);
    else {
        char *repr = string_repr(token_info->token->data->c);

        printf("(type=%i name=%s str=%s start=%i end=%i)\n",
               token_info->token->type, TOKEN_TYPE[token_info->token->type],
               repr, token_info->start, token_info->end);

        free(repr);
    }
}

typedef struct {
    size_t used;
    size_t size;
    TokenInfo **array;
} TokenInfoVector;

TokenInfoVector *new_tokeninfovector(size_t initialSize) {
    TokenInfoVector *a = (TokenInfoVector *)malloc(sizeof(*a));
    a->array = (TokenInfo **)malloc(initialSize * sizeof(TokenInfo *));
    a->used = 0;
    a->size = initialSize;

    return a;
}

void insert_tokeninfovector(TokenInfoVector *a, TokenInfo *element) {
    if (a->used == a->size) {
        a->size *= 2;
        a->array =
            (TokenInfo **)realloc(a->array, a->size * sizeof(TokenInfo *));
    }
    a->array[a->used++] = element;
}

void free_tokeninfovector(TokenInfoVector *a) {
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

typedef enum { SyntaxError } TokenizerError;

static const char *const ERROR_TYPE[] = {[SyntaxError] = "SyntaxError"};

typedef struct {
    TokenizerError type;
    int start;
    int end;
    char *msg;
} TokenizerErrorInfo;

TokenizerErrorInfo *new_tokenizererrorinfo(TokenizerError type, int start,
                                           int end, char *msg) {
    TokenizerErrorInfo *error = (TokenizerErrorInfo *)malloc(sizeof(*error));

    error->type = type;
    error->start = start;
    error->end = end;
    error->msg = msg;

    return error;
}

// Holds either TokenInfoVector or TokenizerErrorInfo
typedef struct {
    TokenInfoVector *tokens;
    TokenizerErrorInfo *error;
} TokenizerReturn;

TokenizerReturn *new_tokenizerreturn(TokenInfoVector *tokens,
                                     TokenizerErrorInfo *error) {
    TokenizerReturn *tokenizer_return =
        (TokenizerReturn *)malloc(sizeof(*tokenizer_return));

    tokenizer_return->tokens = tokens;
    tokenizer_return->error = error;

    return tokenizer_return;
}

TokenizerReturn *tokenize(CharVector *chars) {
    TokenInfoVector *tokens =
        new_tokeninfovector(5); // these tokens are actually token_infos

    int i = 0;
    char c;

    while (i < chars->used) {
        c = chars->array[i];

        if (c == '~')
            insert_tokeninfovector(tokens,
                                   new_tokeninfo(new_token(Til, NULL), i, i));
        else if (c == ':')
            insert_tokeninfovector(tokens,
                                   new_tokeninfo(new_token(Col, NULL), i, i));
        else if (c == '.')
            insert_tokeninfovector(tokens,
                                   new_tokeninfo(new_token(Dot, NULL), i, i));
        else if (c == '\n')
            insert_tokeninfovector(tokens,
                                   new_tokeninfo(new_token(Eol, NULL), i, i));
        else if (c == '$')
            insert_tokeninfovector(tokens,
                                   new_tokeninfo(new_token(Dol, NULL), i, i));
        else if (c == ';')
            insert_tokeninfovector(tokens,
                                   new_tokeninfo(new_token(Smi, NULL), i, i));
        else if (c == '?')
            insert_tokeninfovector(tokens,
                                   new_tokeninfo(new_token(Que, NULL), i, i));
        else if (c == '=')
            insert_tokeninfovector(tokens,
                                   new_tokeninfo(new_token(Eql, NULL), i, i));
        else if (c == '!')
            insert_tokeninfovector(tokens,
                                   new_tokeninfo(new_token(Not, NULL), i, i));
        else if (c == '#')
            insert_tokeninfovector(tokens,
                                   new_tokeninfo(new_token(Hsh, NULL), i, i));
        else if (c == ' ')
            ;

        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                 (c == '_')) {
            int j = i;
            CharVector *temp = new_charvector(5);

            while (i < chars->used - 1) {
                c = chars->array[i];
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                    (c >= '0' && c <= '9') || (c == '_'))
                    insert_charvector(temp, c);
                else
                    break;

                i++;
            }

            char *col = temp->array;
            insert_tokeninfovector(
                tokens,
                new_tokeninfo(new_token(Idn, str_tokendata(col)), j, i));
            i--;

            free(temp);
        }

        else if ((c >= '0' && c <= '9') || (c == '-')) {
            int j = i;
            i++;
            CharVector *temp = new_charvector(5);
            insert_charvector(temp, chars->array[j]);

            while (i < chars->used) {
                c = chars->array[i];
                if (c >= '0' && c <= '9')
                    insert_charvector(temp, c);
                else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                    return new_tokenizerreturn(
                        tokens,
                        new_tokenizererrorinfo(SyntaxError, j, i,
                                               "Invalid number literal"));
                } else
                    break;

                i++;
            }

            char *col = temp->array;
            insert_tokeninfovector(
                tokens,
                new_tokeninfo(new_token(Int, str_tokendata(col)), j, i));
            i--;

            free(temp);
        }

        else if (c == '"') {
            int j = i;
            i += 1;

            CharVector *temp = new_charvector(5);

            if (!(i < chars->used))
                return new_tokenizerreturn(
                    tokens, new_tokenizererrorinfo(
                                SyntaxError, j, i,
                                "EOL while scanning for the string literal"));

            while (chars->array[i] != '"') {
                if (chars->array[i] == '\\') {
                    if (i + 1 < chars->array[i]) {
                        c = chars->array[i + 1];
                        char _c;
                        if (c == 'n')
                            _c = '\n';
                        else if (c == 't')
                            _c = '\t';
                        else
                            _c = c;

                        insert_charvector(temp, _c);
                    } else
                        return new_tokenizerreturn(
                            tokens,
                            new_tokenizererrorinfo(
                                SyntaxError, j, i,
                                "EOL while scanning for the escape sequence"));

                    i++;
                } else
                    insert_charvector(temp, chars->array[i]);

                i++;

                if (!(i < chars->used))
                    return new_tokenizerreturn(
                        tokens,
                        new_tokenizererrorinfo(
                            SyntaxError, j, i,
                            "EOL while scanning for the string literal"));
            }

            char *col = temp->array;
            insert_tokeninfovector(
                tokens,
                new_tokeninfo(new_token(Str, str_tokendata(col)), j, i + 1));

            free(temp);
        }

        else if (c == '[') {
            int j = i;
            i++;

            if (!(i < chars->used))
                return new_tokenizerreturn(
                    tokens, new_tokenizererrorinfo(
                                SyntaxError, j, i,
                                "EOL while scanning for the comment literal"));

            while (chars->array[i] != ']') {
                i++;

                if (!(i < chars->used))
                    return new_tokenizerreturn(
                        tokens,
                        new_tokenizererrorinfo(
                            SyntaxError, j, i,
                            "EOL while scanning for the comment literal"));
            }
        }

        else
            return new_tokenizerreturn(
                tokens,
                new_tokenizererrorinfo(SyntaxError, i, i, "Invalid character"));

        i++;
    }

    TokenizerReturn *tokenizer_return = new_tokenizerreturn(tokens, NULL);

    return tokenizer_return;
}

#endif