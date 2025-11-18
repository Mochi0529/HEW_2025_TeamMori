#include "KadaiGameObject2.h"

#include "KadaiGameObject.h"
#include "Scene.h"
#include "EventQueue.h"

#include "InputComponent.h"
#include <conio.h>
#include <iostream>

using namespace MochiFramework::GameObjects;
using namespace MochiFramework::Components;
using namespace MochiFramework::SceneSystem;

void KadaiGameObject2::UpdateGameObject()
{
	auto* ic = GetComponent<InputComponent>();
	if (!ic) return;
	auto* map = ic->GetInputMap();

	Scene* scene = GetScene();

	if (map->WasActionTriggered("Jump")) {
		scene->CreateGameObject<KadaiGameObject>();
	}
	if (map->WasActionTriggered("ESC"))
	{
		auto objs = scene->FindByType<KadaiGameObject>();
		if (!objs.empty()) {
			// 先頭のオブジェクトを削除要求
			objs.front()->RequestDestroy();
		}
		// GetScene()->GetEventQueue()->Push(MochiFramework::SceneSystem::SceneEvent::Quit());
	}
	
	KadaiGameObject::DisplayCounter();
}
