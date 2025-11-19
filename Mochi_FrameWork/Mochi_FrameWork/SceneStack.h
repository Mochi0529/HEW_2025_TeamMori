#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

#include "FrameworkFacade.h"

namespace MochiFramework::SceneSystem { class Scene; }

// === 名前空間 === //
namespace MochiFramework::SceneSystem
{
    class SceneStack { // メインシーンとオーバーレイシーンを一括管理するスタック

    public:
        // === 型エイリアス（using / typedef） === //

        using Factory = std::function<std::unique_ptr<Scene>(MochiFramework::Core::FrameworkFacade&)>;

        // === コンストラクタ・デストラクタ === //

        explicit SceneStack(MochiFramework::Core::FrameworkFacade& facade);
        ~SceneStack();

        // === 公開関数（外部インターフェース） === //

        void RegisterScene(const std::string& name, Factory factory);

        void ChangeMainScene(const std::string& name);
        void ReloadMainScene();
        void PushOverlayScene(const std::string& name);
        void PopOverlayScene();

        void Input();
        void Update(const float deltaTime);
        void Draw();

    private:
        // === 補助関数（内部ロジック） === //

        void Shutdown();

        // === メンバ変数（内部状態） === //

        MochiFramework::Core::FrameworkFacade& mFramework;

        std::unordered_map<std::string, Factory> factories;
        std::unique_ptr<Scene> mainScene;
        std::vector<std::unique_ptr<Scene>> overlays;
    };
} // namespace MochiFramework::SceneSystem
