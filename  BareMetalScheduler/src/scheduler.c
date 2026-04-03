#include "scheduler.h"
#include <stddef.h>

/* 内部指针与变量，指向外部传入的任务列表 */
static task_t *p_task_list = NULL;
static uint8_t task_num = 0;

void scheduler_init(task_t *tasks, uint8_t count)
{
    if (tasks != NULL && count > 0)
    {
        p_task_list = tasks;
        task_num = count;
    }
}

void scheduler_run(uint32_t sys_tick_ms)
{
    if (p_task_list == NULL || task_num == 0) return;

    for(uint8_t i = 0; i < task_num; i++)
    {
        /* 利用无符号整数相减解决时间戳溢出问题 */
        if((sys_tick_ms - p_task_list[i].last_run) >= p_task_list[i].rate_ms)
        {
            p_task_list[i].last_run = sys_tick_ms;
            
            if(p_task_list[i].task_func != NULL)
            {
                p_task_list[i].task_func();
            }
        }
    }
}