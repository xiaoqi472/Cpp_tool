#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <variant>
#include <utility>

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

template <typename StateVariant, typename EventVariant, typename TransitionHandler>
class StateMachine {
private:
    StateVariant current_state_;
    TransitionHandler handler_;

public:
    StateMachine(StateVariant initial, TransitionHandler handler)
        : current_state_(std::move(initial)), handler_(std::move(handler)) {}

    void dispatch(const EventVariant& event) {
        current_state_ = std::visit(handler_, current_state_, event);
    }

    const StateVariant& get_state() const {
        return current_state_;
    }
};

#endif // STATEMACHINE_H