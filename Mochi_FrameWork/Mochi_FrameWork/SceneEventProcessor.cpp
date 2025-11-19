#include "SceneEventProcessor.h"
#include "SceneStack.h"
#include "EventQueue.h"

using namespace MochiFramework::SceneSystem;

// === –¼‘O‹óŠÔ === //
namespace MochiFramework::SceneSystem
{
    SceneEventProcessor::SceneEventProcessor(SceneStack& stack, EventQueue& queue)
        : mSceneStack(stack), mEventQueue(queue), mQuitRequested(false)
    {
        RegisterHandlers(); // “o˜^
    }

    void SceneEventProcessor::Process() {
        while (auto eOpt = mEventQueue.TryPop()) {
            SceneEvent e = std::move(*eOpt);
            ProcessEvent(std::move(e));
        }
    }

    bool SceneEventProcessor::ShouldQuit() const
    {
        return mQuitRequested;
    }

    void SceneEventProcessor::ProcessEvent(SceneEvent&& e) {
        auto it = mHandlers.find(e.GetType());
        if (it != mHandlers.end()) {
            it->second(e);
        }
        else {
            // debug log: –¢“o˜^‚ÌƒCƒxƒ“ƒgƒ^ƒCƒv
        }
    }

    void SceneEventProcessor::RegisterHandlers() {
        mHandlers[SceneEvent::Type::ChangeScene] = [this](const SceneEvent& e) {
            if (auto* p = std::get_if<ChangeScenePayload>(&e.GetSceneEventData()))
                mSceneStack.ChangeMainScene(p->sceneName);
            };

        mHandlers[SceneEvent::Type::PushOverlay] = [this](const SceneEvent& e) {
            if (auto* p = std::get_if<PushOverlayPayload>(&e.GetSceneEventData()))
                mSceneStack.PushOverlayScene(p->overlayName);
            };

        mHandlers[SceneEvent::Type::PopOverlay] = [this](const SceneEvent&) {
            mSceneStack.PopOverlayScene();
            };

        mHandlers[SceneEvent::Type::ReloadScene] = [this](const SceneEvent&) {
            mSceneStack.ReloadMainScene();
            };

        mHandlers[SceneEvent::Type::QuitGame] = [this](const SceneEvent&) {
            mQuitRequested = true;
            };
    }
} // namespace MochiFramework