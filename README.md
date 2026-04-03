# C++ 工具集

个人 C++ 实用工具集合，按需持续添加。

---

## 工具列表

### 1. MjpegStreamer — MJPEG 视频推流

**路径：** `MjpegStreamer/`

将 OpenCV `cv::Mat` 图像通过 HTTP MJPEG 协议推送到指定端口，可在浏览器直接预览。适用于无显示器的嵌入式开发板（树莓派、Jetson 等）调试 OpenCV 算法时的远程可视化。

**依赖：** OpenCV

**构建：**
```bash
cd MjpegStreamer
mkdir build && cd build
cmake ..
make
```

**使用方式：**

```cpp
#include "MjpegStreamer.h"

MjpegStreamer streamer(8080);  // 指定端口，默认 8080

while (true) {
    cap >> frame;
    // ... 你的 OpenCV 处理逻辑 ...
    streamer.send(processed_frame);  // 传入要查看的帧
}
```

运行后在浏览器打开 `http://<设备IP>:8080` 即可实时查看画面。

> 注意：无头环境下不要使用 `cv::imshow` 和 `cv::waitKey`。

---

### 2. StateMachine — 基于 std::variant 的有限状态机

**路径：** `StateMachine/`

基于 C++17 `std::variant` + `std::visit` 实现的轻量级、类型安全的有限状态机（FSM）框架，Header-Only。通过编译期类型检查保证状态转换的正确性，无虚函数、无动态分配。

**依赖：** C++17

**构建：**
```bash
cd StateMachine
mkdir build && cd build
cmake ..
make
```

**使用方式：**

1. 定义状态和事件类型（`struct`）
2. 用 `std::variant` 聚合为 `StateVariant` 和 `EventVariant`
3. 用 `overloaded` lambda 编写状态转换规则
4. 实例化 `StateMachine`，调用 `dispatch()` 触发事件

参考 `StateMachine/lib/Door.h` 中门控状态机的完整示例。

```cpp
// 触发事件
my_door.dispatch(OpenEvent{});   // Closed -> Opened
my_door.dispatch(CloseEvent{});  // Opened -> Closed

// 查询当前状态
const auto& state = my_door.get_state();
```

**核心文件：**
- `lib/StateMachine.h` — 通用状态机模板（复用此文件）
- `lib/Door.h` — 业务层示例（仿照此文件编写自己的状态机）
- `main.cpp` — 使用示例

---

## 目录结构

```
C++_tool/
├── MjpegStreamer/
│   ├── lib/
│   │   └── MjpegStreamer.h
│   ├── MjpegStreamer.cpp
│   ├── main.cpp
│   └── CMakeLists.txt
└── StateMachine/
    ├── lib/
    │   ├── StateMachine.h
    │   └── Door.h
    ├── main.cpp
    └── CMakeLists.txt
```
