#include "InputMap.h"

namespace MochiFramework::InputSystem {


	void InputMap::BindKey(const std::string& action, int key, EvalMode mode, float value) {
		mActions[action].bindings.emplace_back(KeyBinding{ key, mode, value });
	}
	void InputMap::BindButton(const std::string& action, WORD button, EvalMode mode, float value) {
		mActions[action].bindings.emplace_back(ButtonBinding{ button, mode, value });
	}
	void InputMap::BindAxis1DFromTwoKeys(const std::string& action, int negativeKey, int positiveKey, float scale) {
		mActions[action].bindings.emplace_back(TwoKeyAxisBinding{ negativeKey, positiveKey, scale });
	}
	void InputMap::BindAxis1DFromStick(const std::string& action, Axis1DSource src, float deadzone, float scale) {
		mActions[action].bindings.emplace_back(StickAxisBinding{ src, deadzone, scale });
	}


	float InputMap::ReadBinding(const Binding& b, InputManager* input, bool& outTrig, bool& outRel) {
		return std::visit([&](auto&& bind) -> float {
			using T = std::decay_t<decltype(bind)>;
			if constexpr (std::is_same_v<T, KeyBinding>) {
				switch (bind.mode) {
				case EvalMode::Press: return input->GetKeyPress(bind.key) ? bind.value : 0.0f;
				case EvalMode::Trigger: { bool t = input->GetKeyTrigger(bind.key); outTrig |= t; return t ? bind.value : 0.0f; }
				case EvalMode::Release: { bool r = input->GetKeyRelease(bind.key); outRel |= r; return 0.0f; }
				}
			}
			else if constexpr (std::is_same_v<T, ButtonBinding>) {
				switch (bind.mode) {
				case EvalMode::Press: return input->GetButtonPress(bind.button) ? bind.value : 0.0f;
				case EvalMode::Trigger: { bool t = input->GetButtonTrigger(bind.button); outTrig |= t; return t ? bind.value : 0.0f; }
				case EvalMode::Release: { bool r = input->GetButtonRelease(bind.button); outRel |= r; return 0.0f; }
				}
			}
			else if constexpr (std::is_same_v<T, TwoKeyAxisBinding>) {
				float v = 0.0f;
				if (input->GetKeyPress(bind.negativeKey)) v -= bind.scale;
				if (input->GetKeyPress(bind.positiveKey)) v += bind.scale;
				if (v > 1.0f) v = 1.0f; if (v < -1.0f) v = -1.0f; return v;
			}
			else if constexpr (std::is_same_v<T, StickAxisBinding>) {
				float v = 0.0f;
				switch (bind.src) {
				case Axis1DSource::LeftStickX: v = input->GetLeftStickX(); break;
				case Axis1DSource::LeftStickY: v = input->GetLeftStickY(); break;
				case Axis1DSource::RightStickX: v = input->GetRightStickX(); break;
				case Axis1DSource::RightStickY: v = input->GetRightStickY(); break;
				case Axis1DSource::LeftTrigger: v = input->GetLeftTrigger(); break;
				case Axis1DSource::RightTrigger:v = input->GetRightTrigger(); break;
				}
				if (std::fabs(v) < bind.deadzone) v = 0.0f;
				v *= bind.scale; if (v > 1.0f) v = 1.0f; if (v < -1.0f) v = -1.0f; return v;
			}
			return 0.0f;
			}, b);
	}


	void InputMap::Update(InputManager& input) {
		for (auto& [name, entry] : mActions) {
			float sum = 0.0f; bool trig = false, rel = false;
			for (const auto& b : entry.bindings) sum += ReadBinding(b, &input, trig, rel);
			if (sum > 1.0f) sum = 1.0f; if (sum < -1.0f) sum = -1.0f;
			entry.currentValue = sum; entry.triggered = trig; entry.released = rel;
		}
	}


	float InputMap::GetActionValue(const std::string& name) const {
		if (auto it = mActions.find(name); it != mActions.end()) return it->second.currentValue; return 0.0f;
	}
	bool InputMap::WasActionTriggered(const std::string& name) const {
		if (auto it = mActions.find(name); it != mActions.end()) return it->second.triggered; return false;
	}
	bool InputMap::WasActionReleased(const std::string& name) const {
		if (auto it = mActions.find(name); it != mActions.end()) return it->second.released; return false;
	}


} // namespace MochiFramework::InputSystem