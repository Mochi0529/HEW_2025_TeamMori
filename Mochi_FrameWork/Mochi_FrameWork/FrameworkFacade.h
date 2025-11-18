#pragma once
#include <cassert>
#include "FrameworkContext.h"

namespace MochiFramework::Core {

    class FrameworkFacade
    {
    public:
        explicit FrameworkFacade(FrameworkContext& ctx)
            : mContext(ctx)
        {
        }

        // ---- 低レベルアクセス（EventQueue / InputManager へのゲート） ---- //

        SceneSystem::EventQueue& Events()
        {
            assert(mContext.eventQueue != nullptr);
            return *mContext.eventQueue;
        }

        InputSystem::InputManager& Input()
        {
            assert(mContext.input != nullptr);
            return *mContext.input;
        }

        // ---- 高レベル API をここに増やしていける ---- //
        //
        // 例:
        // void RequestSceneChange(const std::string& name)
        // {
        //     SceneChangeEvent ev{name};
        //     Events().Post(ev);
        // }
        //
        // bool IsConfirmPressed()
        // {
        //     return Input().IsKeyPressed(KeyCode::Enter)
        //         || Input().IsButtonPressed(GamepadButton::A);
        // }

    private:
        FrameworkContext& mContext;
    };
} // namespace MochiFramework::Core
