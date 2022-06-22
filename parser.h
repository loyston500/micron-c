#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  String,
  Int,
  None,
} ValueType;

typedef struct {
  long int i;
  char *c;
} ValueData;

ValueData *str_valuedata(char *c) {
  ValueData *data = (ValueData *)malloc(sizeof(*data));
  data->c = c;
  // data->i = NULL;
  return data;
}

ValueData *int_valuedata(long int i) {
  ValueData *data = (ValueData *)malloc(sizeof(*data));
  data->i = i;
  data->c = NULL;
  return data;
}

typedef struct {
  ValueType type;
  ValueData *data;
} Value;

Value *new_value(ValueType type, ValueData *data) {
  Value *value = (Value *)malloc(sizeof(*value));

  value->type = type;
  value->data = data;

  return value;
}

typedef enum {
  Value_,
  FunCall_,
} ExprType;

typedef struct {
  ExprType type;
  ExprData *data;
} Expr;

typedef struct {
  Value *value;
  Fun *fun;
} ExprData;

ExprData *value_exprdata(Value *value) {
  ExprData *data = (ExprData *)malloc(sizeof(*data));
  data->value = value;
  return data;
}

ExprData *funcall_exprdata(Fun *fun) {
  ExprData *data = (ExprData *)malloc(sizeof(*data));
  data->fun = fun;
  return data;
}

Expr *new_expr(ExprType type, ExprData *data) {
  Expr *expr = (Expr *)malloc(sizeof(*expr));

  expr->type = type;
  expr->data = data;

  return expr;
}

typedef enum {
  Set,
  Get,
  Write,
  Print,
  Add,
  Jump,
  Equal,
  Convert,
  Extract,
  If,
  Input,
  KeyChar,
  Text,
  Number,
  CatchError,
  ThrowError,
  Return,
  FunJump,
  EmptySlot,
  Exit,
} FunType;

typedef struct {
  FunType type;
  Expr **expr;
} Fun;

typedef enum {
  SetLabel,
  LabelPlaceHolder,
  FunCall,
} InstrType;

typedef struct {
  InstrType type;
  InstrData *data;
} Instr;

typedef struct {
  char *c;
  Fun *fun;
} InstrData;

InstrData *str_instrdata(char *value) {
  InstrData *data = (InstrData *)malloc(sizeof(*data));
  data->c = value;
  return data;
}

ExprData *funcall_instrdata(Fun *fun) {
  ExprData *data = (ExprData *)malloc(sizeof(*data));
  data->fun = fun;
  return data;
}

typedef struct {
  Instr *instr;
  int start;
  int end;
} InstrInfo;

InstrInfo *new_instrinfo(Instr *instr, int start, int end) {
  InstrInfo *instr_info = (InstrInfo *)malloc(sizeof(*instr_info));

  instr_info->instr = instr;
  instr_info->start = start;
  instr_info->end = end;

  return instr_info;
}

typedef enum {
  LabelAlreadySet,
  UnexpectedToken,
  InvalidSyntax,
  NotEnoughArgument,
  UnknownFunctionName,
} ParserErrorType;

typedef struct {
  ParserErrorType type;
  char *label;
  TokenInfoVector **line;
  TokenInfoVector *token;
  int got;
  int expected;
} ParserError;

#endif