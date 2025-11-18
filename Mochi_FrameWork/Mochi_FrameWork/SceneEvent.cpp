#include "SceneEvent.h"

// === 名前空間 === //
namespace MochiFramework::SceneSystem
{

    SceneEvent::SceneEvent(Type t, SceneEventData d)
        : data(std::move(d)), type(t) {
    }


    //--- getter ---//

    SceneEvent::Type const SceneEvent::GetType() const
    {
        return type;
    }

    SceneEventData const& SceneEvent::GetSceneEventData() const
    {
        return data;
    }


    //--- ファクトリメソッド ---//

    SceneEvent SceneEvent::ChangeTo(const std::string& name) {
        return SceneEvent(Type::ChangeScene, ChangeScenePayload{ name });
    }

    SceneEvent SceneEvent::PushOverlay(const std::string& name) {
        return SceneEvent(Type::PushOverlay, PushOverlayPayload{ name });
    }

    SceneEvent SceneEvent::Pop() {
        return SceneEvent(Type::PopOverlay, std::monostate{});
    }

    SceneEvent SceneEvent::Quit() {
        return SceneEvent(Type::QuitGame, std::monostate{});
    }

    SceneEvent SceneEvent::Reload() {
        return SceneEvent(Type::ReloadScene, std::monostate{});
    }
} // namespace MochiFramework