/**
 * @file linked_list.c
 * @brief 带头结点单链表接口的实现。
 */

#include "linked_list.h"

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

LinkListStatus LinkListInit(LinkList *list)
{
    if (list == NULL) {
        return LINK_LIST_INVALID_ARGUMENT;
    }

    *list = (LinkList)malloc(sizeof(LinkListNode));
    if (*list == NULL) {
        return LINK_LIST_OUT_OF_MEMORY;
    }

    (*list)->next = NULL;
    return LINK_LIST_OK;
}

LinkListStatus LinkListCreate(LinkList *list,
                              const LinkListElemType *values,
                              size_t count)
{
    LinkListStatus status;
    LinkListNode *tail;
    size_t index;

    if (list == NULL || (values == NULL && count != 0U)) {
        return LINK_LIST_INVALID_ARGUMENT;
    }

    status = LinkListInit(list);
    if (status != LINK_LIST_OK) {
        return status;
    }

    tail = *list;
    for (index = 0U; index < count; ++index) {
        tail->next = (LinkListNode *)malloc(sizeof(LinkListNode));
        if (tail->next == NULL) {
            LinkListDestroy(list);
            return LINK_LIST_OUT_OF_MEMORY;
        }

        tail = tail->next;
        tail->data = values[index];
        tail->next = NULL;
    }

    return LINK_LIST_OK;
}

LinkListStatus LinkListDestroy(LinkList *list)
{
    LinkListNode *current;
    LinkListNode *next;

    if (list == NULL) {
        return LINK_LIST_INVALID_ARGUMENT;
    }

    current = *list;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *list = NULL;
    return LINK_LIST_OK;
}

size_t LinkListLength(const LinkList list)
{
    const LinkListNode *current;
    size_t length = 0U;

    if (list == NULL) {
        return 0U;
    }

    current = list->next;
    while (current != NULL) {
        ++length;
        current = current->next;
    }

    return length;
}

int LinkListEmpty(const LinkList list)
{
    return list == NULL || list->next == NULL;
}

LinkListStatus LinkListLocate(const LinkList list,
                              LinkListElemType value,
                              size_t *position)
{
    const LinkListNode *current;
    size_t current_position = 1U;

    if (list == NULL || position == NULL) {
        return LINK_LIST_INVALID_ARGUMENT;
    }

    current = list->next;
    while (current != NULL) {
        if (current->data == value) {
            *position = current_position;
            return LINK_LIST_OK;
        }

        current = current->next;
        ++current_position;
    }

    return LINK_LIST_ELEMENT_NOT_FOUND;
}

LinkListStatus LinkListTraverse(const LinkList list,
                                LinkListVisitFunc visit,
                                void *context)
{
    const LinkListNode *current;
    size_t position = 1U;

    if (list == NULL || visit == NULL) {
        return LINK_LIST_INVALID_ARGUMENT;
    }

    current = list->next;
    while (current != NULL) {
        visit(current->data, position, context);
        current = current->next;
        ++position;
    }

    return LINK_LIST_OK;
}

LinkListStatus LinkListInsert(LinkList list,
                              size_t position,
                              LinkListElemType value)
{
    LinkListNode *previous;
    LinkListNode *new_node;
    size_t current_position;

    if (list == NULL) {
        return LINK_LIST_INVALID_ARGUMENT;
    }
    if (position == 0U) {
        return LINK_LIST_INDEX_OUT_OF_RANGE;
    }

    previous = list;
    current_position = 1U;
    while (current_position < position && previous->next != NULL) {
        previous = previous->next;
        ++current_position;
    }

    if (current_position != position) {
        return LINK_LIST_INDEX_OUT_OF_RANGE;
    }

    new_node = (LinkListNode *)malloc(sizeof(LinkListNode));
    if (new_node == NULL) {
        return LINK_LIST_OUT_OF_MEMORY;
    }

    new_node->data = value;
    new_node->next = previous->next;
    previous->next = new_node;
    return LINK_LIST_OK;
}

LinkListStatus LinkListDelete(LinkList list,
                              size_t position,
                              LinkListElemType *value)
{
    LinkListNode *previous;
    LinkListNode *deleted_node;
    size_t current_position;

    if (list == NULL) {
        return LINK_LIST_INVALID_ARGUMENT;
    }
    if (position == 0U) {
        return LINK_LIST_INDEX_OUT_OF_RANGE;
    }

    previous = list;
    current_position = 1U;
    while (current_position < position && previous->next != NULL) {
        previous = previous->next;
        ++current_position;
    }

    if (current_position != position || previous->next == NULL) {
        return LINK_LIST_INDEX_OUT_OF_RANGE;
    }

    deleted_node = previous->next;
    previous->next = deleted_node->next;
    if (value != NULL) {
        *value = deleted_node->data;
    }
    free(deleted_node);
    return LINK_LIST_OK;
}
