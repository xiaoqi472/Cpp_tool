#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include <stdint.h>

/* 任务结构体定义 */
typedef struct {
    void (*task_func)(void); // 任务函数指针
    uint32_t rate_ms;        // 执行周期 (ms)
    uint32_t last_run;       // 上次执行时间
} task_t;

/* 初始化调度器，传入任务数组首地址和任务数量 */
void scheduler_init(task_t *tasks, uint8_t count);

/* 调度器主运行函数，需要外部传入当前的系统时间戳 (ms) */
void scheduler_run(uint32_t sys_tick_ms);

#endif /* __SCHEDULER_H */