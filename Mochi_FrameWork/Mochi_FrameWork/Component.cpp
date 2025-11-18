#include "Component.h"
#include "GameObject.h"

// === 名前空間使用宣言 === //
using namespace MochiFramework::Components;
using namespace MochiFramework::GameObjects;

// === 名前空間 === //
namespace MochiFramework::Components
{
	Component::Component(GameObject* owner, int updateOrder)
		: mOwner(owner)
		, mUpdateOrder(updateOrder)
	{
	}

	// 仮想関数
	Component::~Component(){}
	void Component::Update(const float deltaTime){}
	void Component::Input(){}
	void Component::OnOwnerDestroy(){}

	GameObject* Component::GetOwner() const
	{
		return mOwner;
	}

	int Component::GetUpdateOrder() const
	{
		return mUpdateOrder;
	}
} // namespace MochiFramework::Components
