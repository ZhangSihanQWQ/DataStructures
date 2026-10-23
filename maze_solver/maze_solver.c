/**
 * 迷宫外围增加一圈不可通行的边界，因此实际数组大小为 10 * 10，
 * 有效区域为第 1～8 行、第 1～8 列。程序使用栈保存当前路径：
 *   1. 从入口开始，将当前位置压入栈；
 *   2. 按指定顺序寻找可以前进的相邻位置；
 *   3. 找到可行位置就压栈，遇到死路就出栈回退；
 *   4. 栈顶到达出口时，栈中保存的就是入口到出口的路径。
 *
 * 编译示例（在项目根目录执行）：
 *   cc -std=c11 -Wall -Wextra -Wpedantic \
 *      maze_solver/maze_solver.c sequential_stack/sequential_stack.c \
 *      -o maze_solver/maze_solver
 */

#include "../sequential_stack/sequential_stack.h"

#include <stdio.h>

#define MAZE_SIZE 10U
#define MAZE_START_ROW 1U
#define MAZE_START_COLUMN 1U
#define MAZE_END_ROW 8U
#define MAZE_END_COLUMN 8U
#define PATH_COORDINATES_PER_LINE 8U

/* 0 表示可通行，1 表示不可通行，2 表示已经访问过。 */
static const int INITIAL_MAZE[MAZE_SIZE][MAZE_SIZE] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 1, 1, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

typedef struct {
    int row;
    int column;
} Position;

/*
 * 试探顺序：上、右、下、左。
 * 该顺序可以得到题目给出的示例路径；即使改变顺序，仍可能得到其他正确路径。
 */
static const Position DIRECTIONS[] = {
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1}
};

/** 将二维坐标编码为一个整数，供前面顺序栈的 int 元素使用。 */
static StackElemType EncodePosition(size_t row, size_t column)
{
    return (StackElemType)(row * MAZE_SIZE + column);
}

/** 将栈中保存的整数还原为二维坐标。 */
static Position DecodePosition(StackElemType code)
{
    Position position;
    size_t unsigned_code = (size_t)code;

    position.row = (int)(unsigned_code / MAZE_SIZE);
    position.column = (int)(unsigned_code % MAZE_SIZE);
    return position;
}

/**
 * @brief 在迷宫中寻找从入口到出口的路径。
 * @param path 用于保存最终路径的顺序栈。
 * @return 1 表示找到出口，0 表示不存在通路或参数无效。
 */
static int SolveMaze(int maze[MAZE_SIZE][MAZE_SIZE], SqStack *path)
{
    Position current;
    Position next;
    StackElemType code;
    size_t direction;

    if (maze == NULL || path == NULL) {
        return 0;
    }

    if (SqStackPush(path, EncodePosition(MAZE_START_ROW, MAZE_START_COLUMN))
        != SQ_STACK_OK) {
        return 0;
    }
    maze[MAZE_START_ROW][MAZE_START_COLUMN] = 2;

    while (!SqStackEmpty(path)) {
        if (SqStackGetTop(path, &code) != SQ_STACK_OK) {
            return 0;
        }
        current = DecodePosition(code);

        if ((size_t)current.row == MAZE_END_ROW
            && (size_t)current.column == MAZE_END_COLUMN) {
            return 1;
        }

        for (direction = 0U;
             direction < sizeof(DIRECTIONS) / sizeof(DIRECTIONS[0]);
             ++direction) {
            next.row = current.row + DIRECTIONS[direction].row;
            next.column = current.column + DIRECTIONS[direction].column;

            if (maze[next.row][next.column] == 0) {
                maze[next.row][next.column] = 2;
                if (SqStackPush(path,
                                EncodePosition((size_t)next.row,
                                               (size_t)next.column))
                    != SQ_STACK_OK) {
                    return 0;
                }
                break;
            }
        }

        /* 四个方向都不能前进，说明当前位置是死路，需要回退。 */
        if (direction == sizeof(DIRECTIONS) / sizeof(DIRECTIONS[0])) {
            if (SqStackPop(path, NULL) != SQ_STACK_OK) {
                return 0;
            }
        }
    }

    return 0;
}

/** 输出栈中从入口到出口的路径。 */
static void PrintPath(const SqStack *path)
{
    size_t index;

    for (index = 0U; index < path->top; ++index) {
        Position position = DecodePosition(path->data[index]);
        printf("(%d,%d)", position.row, position.column);
        if (index + 1U < path->top) {
            if ((index + 1U) % PATH_COORDINATES_PER_LINE == 0U) {
                putchar('\n');
            } else {
                putchar(' ');
            }
        }
    }
    putchar('\n');
}

int main(void)
{
    int maze[MAZE_SIZE][MAZE_SIZE];
    SqStack path;

    /* 复制初始迷宫，避免直接修改常量地图。 */
    for (size_t row = 0U; row < MAZE_SIZE; ++row) {
        for (size_t column = 0U; column < MAZE_SIZE; ++column) {
            maze[row][column] = INITIAL_MAZE[row][column];
        }
    }

    if (SqStackCreateWithCapacity(&path, MAZE_SIZE * MAZE_SIZE)
        != SQ_STACK_OK) {
        fprintf(stderr, "创建路径栈失败。\n");
        return 1;
    }

    printf("迷宫路径：\n");
    if (!SolveMaze(maze, &path)) {
        printf("没有找到从入口到出口的通路。\n");
        SqStackDestroy(&path);
        return 1;
    }

    PrintPath(&path);
    SqStackDestroy(&path);
    return 0;
}
