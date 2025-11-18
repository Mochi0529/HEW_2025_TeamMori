#pragma once
#include "InputMap.h"


namespace MochiFramework::InputSystem {


	class IInputProfile {
	public:
		virtual ~IInputProfile() = default;
		virtual void Apply(InputMap& map) const = 0; // アクション定義を適用
	};


} // namespace MochiFramework::InputSystem