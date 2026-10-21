/**
 * @file sequential_list.c
 * @brief 顺序表接口的实现。
 */

#include "sequential_list.h"

#include <limits.h>
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
#include <string.h>

static Status EnsureCapacity(SqList *list)
{
    size_t new_capacity;
    ElemType *new_data;

    if (list == NULL) {
        return INVALID_ARGUMENT;
    }

    if (list->length < list->capacity) {
        return OK;
    }

    if (list->capacity == 0 || list->capacity > SIZE_MAX / 2U) {
        return OUT_OF_MEMORY;
    }

    new_capacity = list->capacity * 2U;
    if (new_capacity > SIZE_MAX / sizeof(ElemType)) {
        return OUT_OF_MEMORY;
    }

    new_data = (ElemType *)realloc(list->data, new_capacity * sizeof(ElemType));
    if (new_data == NULL) {
        return OUT_OF_MEMORY;
    }

    list->data = new_data;
    list->capacity = new_capacity;
    return OK;
}

Status InitList(SqList *list)
{
    return InitListWithCapacity(list, SEQUENTIAL_LIST_DEFAULT_CAPACITY);
}

Status InitListWithCapacity(SqList *list, size_t capacity)
{
    if (list == NULL || capacity == 0U || capacity > SIZE_MAX / sizeof(ElemType)) {
        return INVALID_ARGUMENT;
    }

    list->data = (ElemType *)malloc(capacity * sizeof(ElemType));
    if (list->data == NULL) {
        list->length = 0U;
        list->capacity = 0U;
        return OUT_OF_MEMORY;
    }

    list->length = 0U;
    list->capacity = capacity;
    return OK;
}

Status DestroyList(SqList *list)
{
    if (list == NULL) {
        return INVALID_ARGUMENT;
    }

    free(list->data);
    list->data = NULL;
    list->length = 0U;
    list->capacity = 0U;
    return OK;
}

Status ClearList(SqList *list)
{
    if (list == NULL) {
        return INVALID_ARGUMENT;
    }

    list->length = 0U;
    return OK;
}

size_t ListLength(const SqList *list)
{
    return list == NULL ? 0U : list->length;
}

int ListEmpty(const SqList *list)
{
    return list == NULL || list->length == 0U;
}

Status GetElem(const SqList *list, size_t position, ElemType *value)
{
    if (list == NULL || value == NULL) {
        return INVALID_ARGUMENT;
    }
    if (position == 0U || position > list->length) {
        return INDEX_OUT_OF_RANGE;
    }

    *value = list->data[position - 1U];
    return OK;
}

Status LocateElem(const SqList *list, ElemType value, size_t *position)
{
    size_t index;

    if (list == NULL || position == NULL) {
        return INVALID_ARGUMENT;
    }

    for (index = 0U; index < list->length; ++index) {
        if (list->data[index] == value) {
            *position = index + 1U;
            return OK;
        }
    }

    return ELEMENT_NOT_FOUND;
}

Status ListInsert(SqList *list, size_t position, ElemType value)
{
    Status status;

    if (list == NULL) {
        return INVALID_ARGUMENT;
    }
    if (position == 0U || position > list->length + 1U) {
        return INDEX_OUT_OF_RANGE;
    }

    status = EnsureCapacity(list);
    if (status != OK) {
        return status;
    }

    memmove(&list->data[position],
            &list->data[position - 1U],
            (list->length - position + 1U) * sizeof(ElemType));
    list->data[position - 1U] = value;
    ++list->length;
    return OK;
}

Status ListDelete(SqList *list, size_t position, ElemType *value)
{
    if (list == NULL) {
        return INVALID_ARGUMENT;
    }
    if (position == 0U || position > list->length) {
        return INDEX_OUT_OF_RANGE;
    }

    if (value != NULL) {
        *value = list->data[position - 1U];
    }

    memmove(&list->data[position - 1U],
            &list->data[position],
            (list->length - position) * sizeof(ElemType));
    --list->length;
    return OK;
}
