#include <iostream>
// 仅需引入业务头文件，StateMachine.h 已在 Door.h 中被引入
#include "Door.h" 

int main() {
    // 1. 获取已配置好业务规则的“门”状态机实例
    auto my_door = create_door_fsm();

    // 2. 运行并测试
    std::cout << "--- 状态机启动 ---\n";
    
    my_door.dispatch(OpenEvent{});   // 触发规则：关 -> 开
    my_door.dispatch(OpenEvent{});   // 触发兜底：开 -> 开 (无效操作)
    my_door.dispatch(CloseEvent{});  // 触发规则：开 -> 关

    return 0;
}