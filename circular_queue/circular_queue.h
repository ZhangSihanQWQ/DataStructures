/**
 * @file circular_queue.h
 * @brief 顺序循环队列（Sqqueue）的结构定义和操作接口。
 *
 * 顺序循环队列使用连续的动态数组保存元素，并通过 front 和 rear
 * 在数组首尾循环移动来实现“先进先出”（FIFO）：
 *   - front 指向当前队头元素；
 *   - rear 指向下一个入队元素应该存放的位置；
 *   - size 表示当前元素个数。
 *
 * 使用 size 区分队空和队满，因此不需要额外浪费一个数组位置。
 * 当队列容量不足时，本实现会自动扩容，并保持原有元素的出队顺序。
 *
 * 使用示例：
 * @code
 * #include "circular_queue.h"
 *
 * int main(void)
 * {
 *     Sqqueue queue;
 *     QueueElemType value;
 *
 *     if (SqqueueCreate(&queue) != SQ_QUEUE_OK) {
 *         return 1;
 *     }
 *     SqqueueEnqueue(&queue, 10);
 *     SqqueueGetFront(&queue, &value);
 *     SqqueueDequeue(&queue, &value);
 *     SqqueueDestroy(&queue);
 *     return 0;
 * }
 * @endcode
 */

#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include <stddef.h>

/** 顺序循环队列元素类型。需要保存其他类型时，可修改此类型定义。 */
typedef int QueueElemType;

/** 顺序循环队列操作的返回状态。 */
typedef enum {
    SQ_QUEUE_OK = 0,
    SQ_QUEUE_ERROR = -1,
    SQ_QUEUE_INVALID_ARGUMENT = -2,
    SQ_QUEUE_OUT_OF_MEMORY = -3,
    SQ_QUEUE_EMPTY = -4
} SqqueueStatus;

/**
 * @brief 顺序循环队列结构。
 *
 * data 是连续存储空间；
 * front 是队头元素所在的下标；
 * rear 是下一个入队位置的下标；
 * size 是当前元素个数；
 * capacity 是 data 可以容纳的元素个数。
 */
typedef struct {
    QueueElemType *data;
    size_t front;
    size_t rear;
    size_t size;
    size_t capacity;
} Sqqueue;

/** 默认初始容量。 */
#define SQ_QUEUE_DEFAULT_CAPACITY 10U

/** 使用默认容量创建一个空队列。 */
SqqueueStatus SqqueueCreate(Sqqueue *queue);

/** 使用指定容量创建一个空队列，capacity 不能为 0。 */
SqqueueStatus SqqueueCreateWithCapacity(Sqqueue *queue, size_t capacity);

/** 销毁队列并释放其占用的动态内存。 */
SqqueueStatus SqqueueDestroy(Sqqueue *queue);

/** 判断队列是否为空；NULL 队列也视为空队列。 */
int SqqueueEmpty(const Sqqueue *queue);

/** 判断队列是否已满。动态扩容队列通常不会返回满，但该接口便于教学和判断状态。 */
int SqqueueFull(const Sqqueue *queue);

/** 返回队列当前元素个数；传入 NULL 时返回 0。 */
size_t SqqueueSize(const Sqqueue *queue);

/**
 * @brief 获取队头元素但不删除它。
 * @return 队列为空时返回 SQ_QUEUE_EMPTY。
 */
SqqueueStatus SqqueueGetFront(const Sqqueue *queue, QueueElemType *value);

/**
 * @brief 向队尾插入一个元素。
 * @note 队列容量不足时会自动扩容为原来的 2 倍。
 */
SqqueueStatus SqqueueEnqueue(Sqqueue *queue, QueueElemType value);

/**
 * @brief 删除队头元素。
 * @param value 用于接收被删除元素的指针，可以为 NULL。
 * @return 队列为空时返回 SQ_QUEUE_EMPTY。
 */
SqqueueStatus SqqueueDequeue(Sqqueue *queue, QueueElemType *value);

#endif /* CIRCULAR_QUEUE_H */
