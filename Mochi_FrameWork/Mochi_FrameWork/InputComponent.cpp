#include "InputComponent.h"
#include "InputMap.h"

using namespace MochiFramework::GameObjects;
using namespace MochiFramework::InputSystem;

namespace MochiFramework::Components {
	InputComponent::InputComponent(GameObject* owner)
		: ComponentBase<InputComponent>(owner)
	{
	}
	
	void InputComponent::SetInputMap(std::unique_ptr<InputMap> map) { mInputMap = std::move(map); }
	InputMap* InputComponent::GetInputMap() const { return mInputMap.get(); }


	void InputComponent::UpdateInput(InputManager& device) {
		if (mInputMap) mInputMap->Update(device);
	}


} // namespace MochiFramework::GameObjects