#pragma once
#include "Scene.h"
class KadaiScene :
    public MochiFramework::SceneSystem::Scene
{
public:
    explicit KadaiScene(MochiFramework::SceneSystem::EventQueue* queue);

    void SceneUpdate() override;

    std::string GetName() const override;
};

