#include "KadaiScene.h"

#include "EventQueue.h"
#include "KadaiGameObject2.h"
#include "FrameworkFacade.h"

KadaiScene::KadaiScene(MochiFramework::Core::FrameworkFacade& facade)
	:Scene(facade)
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
