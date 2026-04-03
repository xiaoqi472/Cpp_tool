#ifndef DOOR_H
#define DOOR_H

#include <iostream>
#include <variant>
#include "StateMachine.h"

// ------------------------------------------
// 1. 状态与事件定义
// ------------------------------------------
struct Closed {};
struct Opened {};
using DoorState = std::variant<Closed, Opened>;

struct OpenEvent {};
struct CloseEvent {};
using DoorEvent = std::variant<OpenEvent, CloseEvent>;

// ------------------------------------------
// 2. 业务规则与实例化封装 (工厂函数)
// ------------------------------------------
// 使用 inline 防止在多个源文件包含此头文件时引发重定义错误
// 使用 auto 让编译器自动推导包含复杂 Lambda 类型的 StateMachine
inline auto create_door_fsm() {
    // 编写业务规则 (操作手册)
    auto door_rules = overloaded {
        [](Closed&, const OpenEvent&) -> DoorState {
            std::cout << "[动作] 正在开门...\n";
            return Opened{};
        },
        [](Opened&, const CloseEvent&) -> DoorState {
            std::cout << "[动作] 正在关门...\n";
            return Closed{};
        },
        // 兜底规则
        [](auto& state, const auto&) -> DoorState {
            std::cout << "[警告] 无效操作，保持原状。\n";
            return state; 
        }
    };

    // 实例化并返回通用状态机，初始状态设为 Closed{}
    return StateMachine<DoorState, DoorEvent, decltype(door_rules)>(Closed{}, door_rules);
}

#endif // DOOR_H