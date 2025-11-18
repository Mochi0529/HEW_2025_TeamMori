#include "GameObject.h"
#include "Component.h"
#include "Scene.h"
#include "Transform.h"

// === 名前空間使用宣言 === //
using namespace MochiFramework::SceneSystem;
using namespace MochiFramework::Components;

// === 名前空間 === //
namespace MochiFramework::GameObjects
{

	GameObject::GameObject(Scene* scene)
		: mScene(scene)
		, mState(eActive)
		, mName("GameObject")
		, mTag("Untagged")
	{
		// Transform コンポーネントをデフォルトで追加
		AddComponent<Transform>();
	}

	GameObject::~GameObject()
	{
		mComponents.clear();
	}

	void GameObject::Input()
	{
		if (mState == eActive) { // 状態がActiveなら
			for (const auto& component : mComponents) {
				component->Input();
			}
			GameObjectInput();
		}

		for (const auto& comp : mComponents) {
			comp->OnOwnerDestroy(); // オーナー削除される瞬間の処理
		}
	}

	void GameObject::Update(const float deltaTime)
	{
		if (mState == eActive) { // 状態がActiveなら
			for (const auto& component : mComponents) {
				component->Update(deltaTime);
			}
			UpdateGameObject();
		}
	}

	void GameObject::RequestDestroy()
	{
		SetState(eDead);
	}


	// === ゲッター・セッター === //
	void               GameObject::SetName(const std::string& name) { mName = name; }
	const std::string& GameObject::GetName() const                  { return mName; }
	void               GameObject::SetTag(const std::string& tag)   { mTag = tag; }
	const std::string& GameObject::GetTag() const                   { return mTag; }
	Scene*             GameObject::GetScene() const                 { return mScene; }
	void               GameObject::SetState(State state)            { mState = state; }
	GameObject::State  GameObject::GetState() const                 { return mState; }

	// === 仮想関数 === //
	void GameObject::OnDestroy() {}
	void GameObject::GameObjectInput() {}
	void GameObject::UpdateGameObject() {}

} // namespace MochiFramework::GameObjects

