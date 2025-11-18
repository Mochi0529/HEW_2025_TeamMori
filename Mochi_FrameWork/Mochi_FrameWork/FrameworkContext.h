#pragma once

namespace MochiFramework::SceneSystem { class EventQueue; }
namespace MochiFramework::InputSystem { class InputManager; }

// === 名前空間 === //
namespace MochiFramework::Core {
    // 所有権を持たない「配線用」のコンテキスト
    struct FrameworkContext
    {
        MochiFramework::SceneSystem::EventQueue*   eventQueue = nullptr;
        MochiFramework::InputSystem::InputManager* input = nullptr;
        // 将来: Audio* audio = nullptr; Renderer* renderer = nullptr; etc...
    };

} // namespace MochiFramework::Core
