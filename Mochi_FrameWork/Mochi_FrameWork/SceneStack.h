#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

namespace MochiFramework::SceneSystem { class Scene; }
namespace MochiFramework::SceneSystem { class EventQueue; }
namespace MochiFramework::InputSystem { class InputManager; }

// === 名前空間 === //
namespace MochiFramework::SceneSystem
{
    class SceneStack { // メインシーンとオーバーレイシーンを一括管理するスタック

    public:
        // === 型エイリアス（using / typedef） === //

        using Factory = std::function < std::unique_ptr < MochiFramework::SceneSystem::Scene > (MochiFramework::SceneSystem::EventQueue*) > ;

        // === コンストラクタ・デストラクタ === //

        ~SceneStack();

        // === 公開関数（外部インターフェース） === //

        void RegisterScene(const std::string& name, Factory factory);
        void ChangeMainScene(const std::string& name, MochiFramework::SceneSystem::EventQueue* queue);
        void ReloadMainScene(MochiFramework::SceneSystem::EventQueue* queue);
        void PushOverlayScene(const std::string& name, MochiFramework::SceneSystem::EventQueue* queue);
        void PopOverlayScene();

        void Input(MochiFramework::InputSystem::InputManager* device);
        void Update(const float deltaTime);
        void Draw();



    private:
        // === 補助関数（内部ロジック） === //

        void Shutdown();

        // === メンバ変数（内部状態） === //

        std::unordered_map<std::string, Factory> factories;
        std::unique_ptr<MochiFramework::SceneSystem::Scene> mainScene;
        std::vector<std::unique_ptr<MochiFramework::SceneSystem::Scene>> overlays;
    };
} // namespace MochiFramework
