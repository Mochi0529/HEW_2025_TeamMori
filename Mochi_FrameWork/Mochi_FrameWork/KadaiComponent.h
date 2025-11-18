#pragma once
#include "ComponentBase.h"

namespace MochiFramework::GameObjects { class GameObject; }

namespace MochiFramework::GameObjects {
	class KadaiComponent : public MochiFramework::Components::ComponentBase<KadaiComponent>
	{
	public:
		explicit KadaiComponent(MochiFramework::GameObjects::GameObject* owner)
			: ComponentBase(owner)
		{
		}
	};
}


