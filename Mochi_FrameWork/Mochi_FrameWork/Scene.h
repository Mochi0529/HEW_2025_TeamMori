#pragma once

#include <vector>
#include <memory>
#include <string>
#include <type_traits>
#include <typeindex>

#include "FrameworkFacade.h"

namespace MochiFramework::InputSystem { class InputManager; }
namespace MochiFramework::GameObjects { class GameObject; template <typename Derived> class GameObjectBase; }

// === 名前空間 === //
namespace MochiFramework::SceneSystem
{
    class EventQueue;

    class Scene
    {
    public:
        // ===  コンストラクタ・デストラクタ  === //

        explicit Scene(MochiFramework::Core::FrameworkFacade& facade);  // 必ず Facade を受け取る
        virtual ~Scene();
        // コピー・代入を禁止
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;

        // ===  公開関数（外部インターフェース）  === //

        virtual void Init() {};
        void UnInit();

        void Input();                    // ← 引数なし。内部で facade.Input() を使う
        void Update(const float deltaTime);
        void Draw();

        virtual bool ShouldDrawSceneBelow() const;  // 下のシーンを描画するか

        virtual std::string GetName() const = 0; // 現在のシーンの名前（シーン管理のために必要)

        template <typename T, typename... Args>
        T* CreateGameObject(Args&&... args);

        //-- オブジェクト検索 --//
        MochiFramework::GameObjects::GameObject* FindByName(const std::string& name);
        std::vector<MochiFramework::GameObjects::GameObject*> FindByTag(const std::string& tag);

        EventQueue& GetEventQueue();

        template <typename T>
        std::vector<T*> FindByType();

    protected:
        // ===  継承先でオーバーライド可能な関数（フック処理） === //

        virtual void SceneUpdate();

        // ---- Facade & 便利関数 ---- //

        MochiFramework::Core::FrameworkFacade& Framework() { return mFramework; }

        MochiFramework::SceneSystem::EventQueue& Events() { return mFramework.Events(); }
        MochiFramework::InputSystem::InputManager& InputDevice() { return mFramework.Input(); }

    private:
        // ===  補助関数（内部ロジック）  === //

        void CommitPendingObjects(); // 更新中に生成されたオブジェクトを登録する

        // ===  メンバ変数（内部状態）  === //

        MochiFramework::Core::FrameworkFacade& mFramework;

        std::vector<std::unique_ptr<MochiFramework::GameObjects::GameObject>> mGameObjects;
        std::vector<std::unique_ptr<MochiFramework::GameObjects::GameObject>> mPendingGameObjects; // 処理中だった場合のゲームオブジェクトの一時保管場所

        bool mIsProcessingObjects = false; // Update中フラグ
    };

    // ---- 以下、テンプレート実装はほぼそのまま ---- //

    template <typename T, typename... Args>
    T* Scene::CreateGameObject(Args&&... args) {
        static_assert(std::is_base_of<MochiFramework::GameObjects::GameObjectBase<T>, T>::value, "You must inherit from GameObjectBase<T>");

        auto obj = std::make_unique<T>(this, std::forward<Args>(args)...);
        T* rawPtr = obj.get();

        if (mIsProcessingObjects) {
            mPendingGameObjects.emplace_back(std::move(obj));
        }
        else {
            mGameObjects.emplace_back(std::move(obj));
        }

        return rawPtr;
    }

    template <typename T>
    std::vector<T*> Scene::FindByType() {
        std::vector<T*> result;
        const std::type_index targetType = typeid(T);

        for (auto& obj : mGameObjects) {
            if (obj->GetType() == targetType) {
                if (obj->GetState() == MochiFramework::GameObjects::GameObject::eActive) {
                    result.push_back(static_cast<T*>(obj.get()));
                }
            }
        }
        return result;
    }

} // namespace MochiFramework::SceneSystem
