#pragma once
#include <memory>
#include "ComponentBase.h" 

namespace MochiFramework::InputSystem { class InputMap; }
namespace MochiFramework::InputSystem { class InputManager; }

namespace MochiFramework::Components {

	class InputComponent : public MochiFramework::Components::ComponentBase<InputComponent> {
	public:
		explicit InputComponent(MochiFramework::GameObjects::GameObject* owner);

		// -- 入力マップの設定・取得 --//
		void SetInputMap(std::unique_ptr<MochiFramework::InputSystem::InputMap> map);
		MochiFramework::InputSystem::InputMap* GetInputMap() const;

		void UpdateInput(MochiFramework::InputSystem::InputManager& device);


	private:
		std::unique_ptr<MochiFramework::InputSystem::InputMap> mInputMap;
	};


} // namespace MochiFramework::Components