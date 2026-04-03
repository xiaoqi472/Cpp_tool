# baremetal_scheduler

一个轻量级的裸机协作式调度器，适用于 STM32 等嵌入式平台。无需 RTOS，仅依赖系统滴答计时器（SysTick / HAL_GetTick），即可实现多任务周期性调度。

---

## 特性

- 纯 C 实现，无动态内存分配，无第三方依赖
- 任务列表由应用层定义，调度器通过指针管理，实现解耦
- 利用无符号整数相减处理时间戳溢出（tick 回绕），无需额外保护
- 支持任意数量的周期任务（受限于 `uint8_t`，最多 255 个）

---

## 文件结构

```
baremetal_scheduler/
├── inc/
│   └── scheduler.h      # 数据结构与接口声明
└── src/
    └── scheduler.c      # 调度器实现
```

---

## API

### 数据结构

```c
typedef struct {
    void (*task_func)(void); // 任务函数指针
    uint32_t rate_ms;        // 执行周期 (ms)
    uint32_t last_run;       // 上次执行时间（初始化为 0）
} task_t;
```

### 函数

```c
// 初始化调度器，传入任务数组首地址和任务数量
void scheduler_init(task_t *tasks, uint8_t count);

// 调度器主运行函数，需在主循环中持续调用，传入当前系统时间戳 (ms)
void scheduler_run(uint32_t sys_tick_ms);
```

---

## 快速上手

```c
#include "scheduler.h"
#include "main.h" // 假设提供 HAL_GetTick()

/* 1. 定义具体的任务函数 */
void led_task(void)  { /* 闪烁 LED */ }
void comm_task(void) { /* 处理通信 */ }

/* 2. 定义并初始化任务列表（rate_ms 为执行周期，last_run 初始化为 0）*/
task_t my_tasks[] = {
    {led_task,  500, 0},  // 每 500ms 执行一次
    {comm_task,  10, 0},  // 每 10ms  执行一次
};

int main(void)
{
    Hardware_Init(); // 底层硬件初始化

    /* 3. 动态计算任务数量并初始化调度器 */
    uint8_t task_count = sizeof(my_tasks) / sizeof(task_t);
    scheduler_init(my_tasks, task_count);

    while (1)
    {
        /* 4. 将系统时间传入调度器驱动运行 */
        scheduler_run(HAL_GetTick());
    }
}
```

---

## 注意事项

- **协作式调度**：调度器不抢占，任务函数应尽量短小，避免阻塞（禁止在任务中使用 `HAL_Delay` 等忙等待）。
- **时间精度**：调度精度取决于主循环的执行速率，若单次任务耗时过长，将导致其他任务被延迟。
- **时间戳溢出**：调度器使用无符号减法 `(sys_tick_ms - last_run) >= rate_ms`，在 `uint32_t` 溢出时依然正确，无需额外处理。
- **线程安全**：本调度器不支持中断/多核场景下的并发调用，请勿在中断服务函数中调用 `scheduler_run`。
