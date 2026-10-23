/**
 * @file circular_queue.c
 * @brief 顺序循环队列接口的实现。
 */

#include "circular_queue.h"

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

/**
 * @brief 扩大队列容量，并将元素按逻辑顺序重新排列。
 *
 * 扩容前队列中的元素可能已经跨越数组末尾，因此不能只直接 realloc；
 * 需要从 front 开始逐个复制到新的连续空间，扩容后 front 归零。
 */
static SqqueueStatus SqqueueEnsureCapacity(Sqqueue *queue)
{
    QueueElemType *new_data;
    size_t new_capacity;
    size_t index;

    if (queue == NULL) {
        return SQ_QUEUE_INVALID_ARGUMENT;
    }
    if (queue->size < queue->capacity) {
        return SQ_QUEUE_OK;
    }
    if (queue->capacity == 0U || queue->capacity > SIZE_MAX / 2U) {
        return SQ_QUEUE_OUT_OF_MEMORY;
    }

    new_capacity = queue->capacity * 2U;
    if (new_capacity > SIZE_MAX / sizeof(QueueElemType)) {
        return SQ_QUEUE_OUT_OF_MEMORY;
    }

    new_data = (QueueElemType *)malloc(new_capacity * sizeof(QueueElemType));
    if (new_data == NULL) {
        return SQ_QUEUE_OUT_OF_MEMORY;
    }

    for (index = 0U; index < queue->size; ++index) {
        new_data[index] = queue->data[(queue->front + index) % queue->capacity];
    }

    free(queue->data);
    queue->data = new_data;
    queue->capacity = new_capacity;
    queue->front = 0U;
    queue->rear = queue->size;
    return SQ_QUEUE_OK;
}

SqqueueStatus SqqueueCreate(Sqqueue *queue)
{
    return SqqueueCreateWithCapacity(queue, SQ_QUEUE_DEFAULT_CAPACITY);
}

SqqueueStatus SqqueueCreateWithCapacity(Sqqueue *queue, size_t capacity)
{
    if (queue == NULL || capacity == 0U
        || capacity > SIZE_MAX / sizeof(QueueElemType)) {
        return SQ_QUEUE_INVALID_ARGUMENT;
    }

    queue->data = (QueueElemType *)malloc(capacity * sizeof(QueueElemType));
    if (queue->data == NULL) {
        queue->front = 0U;
        queue->rear = 0U;
        queue->size = 0U;
        queue->capacity = 0U;
        return SQ_QUEUE_OUT_OF_MEMORY;
    }

    queue->front = 0U;
    queue->rear = 0U;
    queue->size = 0U;
    queue->capacity = capacity;
    return SQ_QUEUE_OK;
}

SqqueueStatus SqqueueDestroy(Sqqueue *queue)
{
    if (queue == NULL) {
        return SQ_QUEUE_INVALID_ARGUMENT;
    }

    free(queue->data);
    queue->data = NULL;
    queue->front = 0U;
    queue->rear = 0U;
    queue->size = 0U;
    queue->capacity = 0U;
    return SQ_QUEUE_OK;
}

int SqqueueEmpty(const Sqqueue *queue)
{
    return queue == NULL || queue->size == 0U;
}

int SqqueueFull(const Sqqueue *queue)
{
    return queue != NULL && queue->size == queue->capacity;
}

size_t SqqueueSize(const Sqqueue *queue)
{
    return queue == NULL ? 0U : queue->size;
}

SqqueueStatus SqqueueGetFront(const Sqqueue *queue, QueueElemType *value)
{
    if (queue == NULL || value == NULL) {
        return SQ_QUEUE_INVALID_ARGUMENT;
    }
    if (queue->size == 0U) {
        return SQ_QUEUE_EMPTY;
    }

    *value = queue->data[queue->front];
    return SQ_QUEUE_OK;
}

SqqueueStatus SqqueueEnqueue(Sqqueue *queue, QueueElemType value)
{
    SqqueueStatus status;

    if (queue == NULL) {
        return SQ_QUEUE_INVALID_ARGUMENT;
    }

    status = SqqueueEnsureCapacity(queue);
    if (status != SQ_QUEUE_OK) {
        return status;
    }

    queue->data[queue->rear] = value;
    queue->rear = (queue->rear + 1U) % queue->capacity;
    ++queue->size;
    return SQ_QUEUE_OK;
}

SqqueueStatus SqqueueDequeue(Sqqueue *queue, QueueElemType *value)
{
    if (queue == NULL) {
        return SQ_QUEUE_INVALID_ARGUMENT;
    }
    if (queue->size == 0U) {
        return SQ_QUEUE_EMPTY;
    }

    if (value != NULL) {
        *value = queue->data[queue->front];
    }
    queue->front = (queue->front + 1U) % queue->capacity;
    --queue->size;
    return SQ_QUEUE_OK;
}
