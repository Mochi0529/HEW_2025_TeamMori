#include "EventQueue.h"

// === 名前空間使用宣言 === //
using namespace MochiFramework::SceneSystem;

// === 名前空間 === //
namespace MochiFramework::SceneSystem
{
    void EventQueue::Push(const SceneEvent& e) {
        events.push(e);
    }

    void EventQueue::Push(SceneEvent&& e) {
        events.push(std::move(e));
    }

    bool EventQueue::IsEmpty() const {
        return events.empty();
    }

    std::optional<SceneEvent> EventQueue::TryPop() {
        if (events.empty()) return std::nullopt;
        SceneEvent e = std::move(events.front());
        events.pop();
        return e;
    }
}