#pragma once
#include "Scene.h"

namespace MochiFramework::Core { class FrameworkFacade; }

class KadaiScene :
    public MochiFramework::SceneSystem::Scene
{
public:
    explicit KadaiScene(MochiFramework::Core::FrameworkFacade& facade);

    void SceneUpdate() override;

    std::string GetName() const override;
};

