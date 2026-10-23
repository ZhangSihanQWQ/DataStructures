/**
 * @file sequential_stack.c
 * @brief 顺序栈接口的实现。
 */

#include "sequential_stack.h"

#include <stdint.h>
#if defined(__has_include)
#  if __has_include(<malloc.h>)
#    include <malloc.h>
#  else
#    include <stdlib.h>
#  endif
#else
#  include <malloc.h>
#endif

/** 确保顺序栈至少还有一个可用存储位置。 */
static SqStackStatus SqStackEnsureCapacity(SqStack *stack)
{
    size_t new_capacity;
    StackElemType *new_data;

    if (stack == NULL) {
        return SQ_STACK_INVALID_ARGUMENT;
    }
    if (stack->top < stack->capacity) {
        return SQ_STACK_OK;
    }
    if (stack->capacity == 0U || stack->capacity > SIZE_MAX / 2U) {
        return SQ_STACK_OUT_OF_MEMORY;
    }

    new_capacity = stack->capacity * 2U;
    if (new_capacity > SIZE_MAX / sizeof(StackElemType)) {
        return SQ_STACK_OUT_OF_MEMORY;
    }

    new_data = (StackElemType *)realloc(stack->data,
                                        new_capacity * sizeof(StackElemType));
    if (new_data == NULL) {
        return SQ_STACK_OUT_OF_MEMORY;
    }

    stack->data = new_data;
    stack->capacity = new_capacity;
    return SQ_STACK_OK;
}

SqStackStatus SqStackCreate(SqStack *stack)
{
    return SqStackCreateWithCapacity(stack, SQ_STACK_DEFAULT_CAPACITY);
}

SqStackStatus SqStackCreateWithCapacity(SqStack *stack, size_t capacity)
{
    if (stack == NULL || capacity == 0U
        || capacity > SIZE_MAX / sizeof(StackElemType)) {
        return SQ_STACK_INVALID_ARGUMENT;
    }

    stack->data = (StackElemType *)malloc(capacity * sizeof(StackElemType));
    if (stack->data == NULL) {
        stack->top = 0U;
        stack->capacity = 0U;
        return SQ_STACK_OUT_OF_MEMORY;
    }

    stack->top = 0U;
    stack->capacity = capacity;
    return SQ_STACK_OK;
}

SqStackStatus SqStackDestroy(SqStack *stack)
{
    if (stack == NULL) {
        return SQ_STACK_INVALID_ARGUMENT;
    }

    free(stack->data);
    stack->data = NULL;
    stack->top = 0U;
    stack->capacity = 0U;
    return SQ_STACK_OK;
}

int SqStackEmpty(const SqStack *stack)
{
    return stack == NULL || stack->top == 0U;
}

size_t SqStackSize(const SqStack *stack)
{
    return stack == NULL ? 0U : stack->top;
}

SqStackStatus SqStackGetTop(const SqStack *stack, StackElemType *value)
{
    if (stack == NULL || value == NULL) {
        return SQ_STACK_INVALID_ARGUMENT;
    }
    if (stack->top == 0U) {
        return SQ_STACK_EMPTY;
    }

    *value = stack->data[stack->top - 1U];
    return SQ_STACK_OK;
}

SqStackStatus SqStackPush(SqStack *stack, StackElemType value)
{
    SqStackStatus status;

    if (stack == NULL) {
        return SQ_STACK_INVALID_ARGUMENT;
    }

    status = SqStackEnsureCapacity(stack);
    if (status != SQ_STACK_OK) {
        return status;
    }

    stack->data[stack->top] = value;
    ++stack->top;
    return SQ_STACK_OK;
}

SqStackStatus SqStackPop(SqStack *stack, StackElemType *value)
{
    if (stack == NULL) {
        return SQ_STACK_INVALID_ARGUMENT;
    }
    if (stack->top == 0U) {
        return SQ_STACK_EMPTY;
    }

    --stack->top;
    if (value != NULL) {
        *value = stack->data[stack->top];
    }
    return SQ_STACK_OK;
}
