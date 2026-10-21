/**
 * @file linked_list.h
 * @brief 带头结点的单链表（LinkList）接口定义。
 *
 * 单链表中的结点通过指针连接，不要求元素在内存中连续存放。
 * 本实现采用带头结点的设计：LinkList 指向头结点，头结点本身不存放有效数据，
 * 第一个有效元素位于 list->next。这样可以统一处理头部插入、删除等操作。
 *
 * 对外的位置从 1 开始：
 *   - 第 1 个有效元素是 list->next；
 *   - 插入位置 length + 1 表示尾插；
 *   - 删除和定位操作的位置范围是 1 到 length。
 *
 * 使用示例：
 * @code
 * #include "linked_list.h"
 *
 * static void PrintElement(LinkListElemType value, size_t position, void *context)
 * {
 *     (void)context;
 *     printf("第 %zu 个元素：%d\\n", position, value);
 * }
 *
 * int main(void)
 * {
 *     LinkList list;
 *     LinkListElemType values[] = {10, 20, 30};
 *
 *     if (LinkListCreate(&list, values, 3) != LINK_LIST_OK) {
 *         return 1;
 *     }
 *     LinkListTraverse(list, PrintElement, NULL);
 *     LinkListDestroy(&list);
 *     return 0;
 * }
 * @endcode
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>

/** 单链表元素类型。需要保存其他类型时，可修改此类型定义。 */
typedef int LinkListElemType;

/** 单链表操作的返回状态。 */
typedef enum {
    LINK_LIST_OK = 0,
    LINK_LIST_ERROR = -1,
    LINK_LIST_INVALID_ARGUMENT = -2,
    LINK_LIST_OUT_OF_MEMORY = -3,
    LINK_LIST_INDEX_OUT_OF_RANGE = -4,
    LINK_LIST_EMPTY = -5,
    LINK_LIST_ELEMENT_NOT_FOUND = -6
} LinkListStatus;

/** 单链表结点。 */
typedef struct LinkListNode {
    LinkListElemType data;
    struct LinkListNode *next;
} LinkListNode;

/** LinkList 指向带头结点的单链表。 */
typedef LinkListNode *LinkList;

/**
 * @brief 遍历访问函数类型。
 * @param value 当前元素值。
 * @param position 当前元素位置，从 1 开始。
 * @param context 调用者自定义的上下文指针，可以为 NULL。
 */
typedef void (*LinkListVisitFunc)(LinkListElemType value,
                                  size_t position,
                                  void *context);

/**
 * @brief 创建一个空的带头结点单链表。
 * @param list 用于接收链表头结点指针的指针。
 */
LinkListStatus LinkListInit(LinkList *list);

/**
 * @brief 根据数组中的元素创建单链表。
 * @param list 用于接收新链表的指针。
 * @param values 初始元素数组；当 count 为 0 时可以为 NULL。
 * @param count 数组中的元素个数。
 * @note 元素按照数组中的顺序写入链表，values[0] 成为第一个元素。
 */
LinkListStatus LinkListCreate(LinkList *list,
                              const LinkListElemType *values,
                              size_t count);

/**
 * @brief 销毁链表并释放所有结点。
 * @param list 指向链表指针的指针。
 */
LinkListStatus LinkListDestroy(LinkList *list);

/** 返回链表中有效元素的个数；传入 NULL 时返回 0。 */
size_t LinkListLength(const LinkList list);

/** 判断链表是否为空；NULL 链表也视为空。 */
int LinkListEmpty(const LinkList list);

/**
 * @brief 定位元素第一次出现的位置。
 * @param list 链表。
 * @param value 要查找的值。
 * @param position 用于接收位置的指针，从 1 开始。
 */
LinkListStatus LinkListLocate(const LinkList list,
                              LinkListElemType value,
                              size_t *position);

/**
 * @brief 按顺序遍历所有有效元素。
 * @param list 链表。
 * @param visit 每访问一个元素时调用的函数，不能为 NULL。
 * @param context 传给 visit 的自定义上下文指针，可以为 NULL。
 */
LinkListStatus LinkListTraverse(const LinkList list,
                                LinkListVisitFunc visit,
                                void *context);

/**
 * @brief 在指定位置之前插入一个元素。
 * @param list 链表。
 * @param position 插入位置，从 1 开始；length + 1 表示尾插。
 */
LinkListStatus LinkListInsert(LinkList list,
                              size_t position,
                              LinkListElemType value);

/**
 * @brief 删除指定位置的元素。
 * @param list 链表。
 * @param position 删除位置，从 1 开始。
 * @param value 用于接收被删除元素的指针，可以为 NULL。
 */
LinkListStatus LinkListDelete(LinkList list,
                              size_t position,
                              LinkListElemType *value);

#endif /* LINKED_LIST_H */
