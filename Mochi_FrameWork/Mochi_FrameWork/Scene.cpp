#include "Scene.h"
#include "GameObject.h"
#include "EventQueue.h"
#include "InputManager.h"
#include "InputComponent.h"
#include "FrameworkFacade.h"

#include <algorithm>

using namespace MochiFramework::Core;
using namespace MochiFramework::GameObjects;
using namespace MochiFramework::Components;
using namespace MochiFramework::InputSystem;

// === 名前空間 === //
namespace MochiFramework::SceneSystem
{
	Scene::Scene(FrameworkFacade& facade) : mFramework(facade) 
	{
	}

	Scene::~Scene() {
		UnInit(); // 明示的な破棄処理が必要ならここでも呼べる
	}

	void Scene::UnInit() {
		mGameObjects.clear(); // 所有権を破棄し、GameObject を delete する
		mPendingGameObjects.clear();
	}
	
	void Scene::Input()
	{
		mIsProcessingObjects = true;

		// InputComponent だけ更新して「今フレームのアクション値」を確定
		for (auto& object : mGameObjects) {
			if (auto* ic = object->GetComponent<InputComponent>()) {
				ic->UpdateInput(InputDevice());
			}
		}

		mIsProcessingObjects = false;
		CommitPendingObjects();
	}

	void Scene::Update(const float deltaTime)
	{
		mIsProcessingObjects = true;
		// 通常のオブジェクト更新
		for (const auto& obj : mGameObjects) {
			obj->Update(deltaTime);
		}
		mIsProcessingObjects = false;

		CommitPendingObjects();

		// 死亡オブジェクトに対して OnDestroy 呼び出し
		for (const auto& obj : mGameObjects) {
			if (obj->GetState() == GameObject::eDead) {
				obj->OnDestroy();  
			}
		}


		// 削除処理
		mGameObjects.erase(
			std::remove_if(mGameObjects.begin(), mGameObjects.end(),
				[](const std::unique_ptr<GameObject>& obj) {
					return obj->GetState() == GameObject::eDead;
				}),
			mGameObjects.end()
		);

		// シーン独自の処理
		SceneUpdate();
	}

	void Scene::Draw()
	{

	}

	bool Scene::ShouldDrawSceneBelow() const
	{
		return false;
	}


	GameObject* Scene::FindByName(const std::string& name) {
		for (auto& obj : mGameObjects) {
			if (obj->GetName() == name && obj->GetState() == GameObject::eActive) {
				return obj.get();
			}
		}
		return nullptr;
	}

	std::vector<GameObject*> Scene::FindByTag(const std::string& tag) {
		std::vector<GameObject*> result;
		for (auto& obj : mGameObjects) {
			if (obj->GetTag() == tag && obj->GetState() == GameObject::eActive) {
				result.push_back(obj.get());
			}
		}
		return result;
	}

	EventQueue& Scene::GetEventQueue()
	{
		return Events();
	}

	void Scene::SceneUpdate()
	{

	}

	void Scene::CommitPendingObjects() {
		for (auto& obj : mPendingGameObjects) {
			mGameObjects.emplace_back(std::move(obj));
		}
		mPendingGameObjects.clear();
	}
} // namespace MochiFramework::SceneSystem