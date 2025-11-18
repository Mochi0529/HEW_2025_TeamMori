#pragma once
#include "GameObjectBase.h"

#include "InputComponent.h"
#include "DefaultPlayerProfile.h"

namespace MochiFramework::GameObjects {

    class KadaiGameObject2 : public GameObjectBase<KadaiGameObject2>
    {
    public:
        explicit KadaiGameObject2(MochiFramework::SceneSystem::Scene* scene)
            : GameObjectBase(scene)
        {
            using namespace MochiFramework::GameObjects;
            using namespace MochiFramework::InputSystem;

            auto* ic = AddComponent<MochiFramework::Components::InputComponent>();
            auto map = std::make_unique<InputMap>();
            DefaultPlayerProfile profile;
            profile.Apply(*map);          // 既定バインドをコピー
            ic->SetInputMap(std::move(map));
        }
        
        void UpdateGameObject() override;
     
    };

} // namespace MochiFramework::GameObjects