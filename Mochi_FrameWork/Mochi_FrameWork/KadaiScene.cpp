#include "KadaiScene.h"

#include "EventQueue.h"
#include "KadaiGameObject2.h"

KadaiScene::KadaiScene(MochiFramework::SceneSystem::EventQueue* queue)
	:Scene(queue)
{
	CreateGameObject<MochiFramework::GameObjects::KadaiGameObject2>();
}

void KadaiScene::SceneUpdate()
{
	
}

std::string KadaiScene::GetName() const
{
	return "KadaiScene";
}
