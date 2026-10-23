/**
 * @file sequential_stack.h
 * @brief 顺序栈（SqStack）的结构定义和操作接口。
 *
 * 顺序栈使用一段连续的动态内存保存元素，并遵循“后进先出”（LIFO）原则：
 * 最后入栈的元素最先出栈。top 表示当前元素个数，同时也表示下一个元素
 * 应该存放的数组下标，因此栈顶元素位于 data[top - 1]。
 *
 * 使用示例：
 * @code
 * #include "sequential_stack.h"
 *
 * int main(void)
 * {
 *     SqStack stack;
 *     StackElemType value;
 *
 *     if (SqStackCreate(&stack) != SQ_STACK_OK) {
 *         return 1;
 *     }
 *
 *     SqStackPush(&stack, 10);
 *     SqStackGetTop(&stack, &value);
 *     SqStackPop(&stack, &value);
 *     SqStackDestroy(&stack);
 *     return 0;
 * }
 * @endcode
 */

#ifndef SEQUENTIAL_STACK_H
#define SEQUENTIAL_STACK_H

#include <stddef.h>

/** 顺序栈元素类型。需要保存其他类型时，可修改此类型定义。 */
typedef int StackElemType;

/** 顺序栈操作的返回状态。 */
typedef enum {
    SQ_STACK_OK = 0,
    SQ_STACK_ERROR = -1,
    SQ_STACK_INVALID_ARGUMENT = -2,
    SQ_STACK_OUT_OF_MEMORY = -3,
    SQ_STACK_EMPTY = -4
} SqStackStatus;

/**
 * @brief 顺序栈结构。
 *
 * top 是当前元素个数，不是最后一个元素的数组下标；
 * capacity 是 data 当前可以容纳的元素个数。
 */
typedef struct {
    StackElemType *data;
    size_t top;
    size_t capacity;
} SqStack;

/** 默认初始容量。 */
#define SQ_STACK_DEFAULT_CAPACITY 10U

/**
 * @brief 创建一个使用默认容量的空顺序栈。
 * @param stack 待创建的顺序栈指针，不能为 NULL。
 */
SqStackStatus SqStackCreate(SqStack *stack);

/**
 * @brief 按指定容量创建一个空顺序栈。
 * @param stack 待创建的顺序栈指针，不能为 NULL。
 * @param capacity 初始容量，不能为 0。
 */
SqStackStatus SqStackCreateWithCapacity(SqStack *stack, size_t capacity);

/**
 * @brief 销毁顺序栈并释放动态内存。
 * @note 销毁后可以再次调用 SqStackCreate 重新使用 stack。
 */
SqStackStatus SqStackDestroy(SqStack *stack);

/** 判断栈是否为空；NULL 指针也视为空栈。 */
int SqStackEmpty(const SqStack *stack);

/** 返回当前栈中元素个数；传入 NULL 时返回 0。 */
size_t SqStackSize(const SqStack *stack);

/**
 * @brief 获取栈顶元素但不删除它。
 * @param stack 顺序栈指针。
 * @param value 用于接收栈顶元素的指针。
 * @return 栈为空时返回 SQ_STACK_EMPTY。
 */
SqStackStatus SqStackGetTop(const SqStack *stack, StackElemType *value);

/**
 * @brief 将一个元素压入栈顶。
 * @note 当容量不足时会自动扩容为原来的 2 倍。
 */
SqStackStatus SqStackPush(SqStack *stack, StackElemType value);

/**
 * @brief 删除栈顶元素。
 * @param value 用于接收被删除元素的指针，可以为 NULL。
 * @return 栈为空时返回 SQ_STACK_EMPTY。
 */
SqStackStatus SqStackPop(SqStack *stack, StackElemType *value);

#endif /* SEQUENTIAL_STACK_H */
