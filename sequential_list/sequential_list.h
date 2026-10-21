/**
 * @file sequential_list.h
 * @brief 顺序表（动态数组）的接口定义。
 *
 * 顺序表使用一段连续的动态内存保存数据元素。与静态数组相比，
 * 本实现可以根据需要自动扩容，因此更适合在后续算法程序中复用。
 *
 * 下标约定：
 *   - 对外提供的元素位置从 1 开始；
 *   - 例如，第 1 个元素对应 data[0]，第 length 个元素对应 data[length - 1]。
 *
 * 使用示例：
 * @code
 * #include "sequential_list.h"
 *
 * int main(void)
 * {
 *     SqList list;
 *     ElemType value;
 *
 *     if (InitList(&list) != OK) {
 *         return 1;
 *     }
 *
 *     value = 10;
 *     ListInsert(&list, 1, value);
 *     GetElem(&list, 1, &value);
 *
 *     DestroyList(&list);
 *     return 0;
 * }
 * @endcode
 */

#ifndef SEQUENTIAL_LIST_H
#define SEQUENTIAL_LIST_H

#include <stddef.h>

/** 顺序表中元素的类型。需要保存其他类型时，可以修改此类型定义。 */
typedef int ElemType;

/** 顺序表操作的返回状态。 */
typedef enum {
    OK = 0,
    ERROR = -1,
    INVALID_ARGUMENT = -2,
    OUT_OF_MEMORY = -3,
    INDEX_OUT_OF_RANGE = -4,
    LIST_EMPTY = -5,
    ELEMENT_NOT_FOUND = -6
} Status;

/**
 * @brief 顺序表结构。
 *
 * data 指向容量为 capacity 的连续存储空间；
 * length 表示当前已经存储的元素个数；
 * capacity 表示 data 当前可以容纳的元素个数。
 */
typedef struct {
    ElemType *data;
    size_t length;
    size_t capacity;
} SqList;

/** 使用的默认初始容量。顺序表为空时也会分配这段空间。 */
#define SEQUENTIAL_LIST_DEFAULT_CAPACITY 10U

/**
 * @brief 初始化顺序表。
 * @param list 待初始化的顺序表指针，不能为 NULL。
 * @return OK 表示成功；INVALID_ARGUMENT 表示参数无效；
 *         OUT_OF_MEMORY 表示内存分配失败。
 */
Status InitList(SqList *list);

/**
 * @brief 按指定容量初始化顺序表。
 * @param list 待初始化的顺序表指针，不能为 NULL。
 * @param capacity 初始容量，不能为 0。
 * @return OK 表示成功；INVALID_ARGUMENT 表示参数无效；
 *         OUT_OF_MEMORY 表示内存分配失败。
 */
Status InitListWithCapacity(SqList *list, size_t capacity);

/**
 * @brief 销毁顺序表并释放其占用的动态内存。
 * @param list 待销毁的顺序表指针，可以为 NULL。
 * @return OK 表示成功；INVALID_ARGUMENT 表示参数无效。
 * @note 销毁后 list 可以再次传入 InitList 重新使用。
 */
Status DestroyList(SqList *list);

/**
 * @brief 清空顺序表中的所有元素，但保留已经申请的容量。
 */
Status ClearList(SqList *list);

/** 返回当前元素个数。传入 NULL 时返回 0。 */
size_t ListLength(const SqList *list);

/** 判断顺序表是否为空。NULL 顺序表也视为空。 */
int ListEmpty(const SqList *list);

/**
 * @brief 按位置查找元素。
 * @param list 顺序表指针。
 * @param position 元素位置，从 1 开始。
 * @param value 用于接收元素值的指针。
 */
Status GetElem(const SqList *list, size_t position, ElemType *value);

/**
 * @brief 按值查找元素第一次出现的位置。
 * @param list 顺序表指针。
 * @param value 待查找的值。
 * @param position 用于接收位置的指针，从 1 开始。
 */
Status LocateElem(const SqList *list, ElemType value, size_t *position);

/**
 * @brief 在指定位置插入元素。
 * @param list 顺序表指针。
 * @param position 插入位置，从 1 开始；position = length + 1 表示尾插。
 * @param value 待插入的元素。
 * @note 插入点及其后的元素会依次向后移动一个位置。
 */
Status ListInsert(SqList *list, size_t position, ElemType value);

/**
 * @brief 删除指定位置的元素。
 * @param list 顺序表指针。
 * @param position 删除位置，从 1 开始。
 * @param value 用于接收被删除元素的指针，可以为 NULL。
 */
Status ListDelete(SqList *list, size_t position, ElemType *value);

#endif /* SEQUENTIAL_LIST_H */
