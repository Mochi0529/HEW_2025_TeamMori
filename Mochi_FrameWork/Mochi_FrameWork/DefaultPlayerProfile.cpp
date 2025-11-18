#include "DefaultPlayerProfile.h"
#include <Windows.h> // VK_*


namespace MochiFramework::InputSystem {


	void DefaultPlayerProfile::Apply(InputMap& map) const {
		// 移動: 矢印 + 左スティック
		map.BindAxis1DFromTwoKeys("MoveX", VK_LEFT, VK_RIGHT);
		map.BindAxis1DFromTwoKeys("MoveY", VK_DOWN, VK_UP);
		map.BindAxis1DFromStick("MoveX", Axis1DSource::LeftStickX, 0.2f);
		map.BindAxis1DFromStick("MoveY", Axis1DSource::LeftStickY, 0.2f);


		// ジャンプ: Space or Aボタン（Trigger）
		map.BindKey("Jump", VK_SPACE, EvalMode::Trigger);
		map.BindKey("ESC", VK_ESCAPE, EvalMode::Trigger);
		map.BindButton("Jump", XINPUT_GAMEPAD_A, EvalMode::Trigger);
	}


} // namespace MochiFramework::InputSystem