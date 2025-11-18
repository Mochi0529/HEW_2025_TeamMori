#pragma once
#include <unordered_map>
#include <vector>
#include <variant>
#include <string>
#include <algorithm>
#include <cmath>


#include "InputManager.h"

namespace MochiFramework::InputSystem {


	enum class EvalMode { Press, Trigger, Release };
	enum class Axis1DSource { LeftStickX, LeftStickY, RightStickX, RightStickY, LeftTrigger, RightTrigger };


	struct KeyBinding { int key = 0; EvalMode mode = EvalMode::Press; float value = 1.0f; };
	struct ButtonBinding { WORD button = 0; EvalMode mode = EvalMode::Press; float value = 1.0f; };
	struct TwoKeyAxisBinding { int negativeKey = 0; int positiveKey = 0; float scale = 1.0f; };
	struct StickAxisBinding { Axis1DSource src = Axis1DSource::LeftStickX; float deadzone = 0.2f; float scale = 1.0f; };


	using Binding = std::variant<KeyBinding, ButtonBinding, TwoKeyAxisBinding, StickAxisBinding>;


	struct ActionEntry {
		std::vector<Binding> bindings;
		float currentValue = 0.0f; // -1..+1
		bool triggered = false; // 今フレームで押された
		bool released = false; // 今フレームで離された
	};


	class InputMap {
	public:
		// --- Bind API ---
		void BindKey(const std::string& action, int key, EvalMode mode = EvalMode::Press, float value = 1.0f);
		void BindButton(const std::string& action, WORD button, EvalMode mode = EvalMode::Press, float value = 1.0f);
		void BindAxis1DFromTwoKeys(const std::string& action, int negativeKey, int positiveKey, float scale = 1.0f);
		void BindAxis1DFromStick(const std::string& action, Axis1DSource src, float deadzone = 0.2f, float scale = 1.0f);


		// --- Frame Update ---
		void Update(InputManager& input);


		// --- Pull API ---
		float GetActionValue(const std::string& name) const; // -1..+1
		bool WasActionTriggered(const std::string& name) const;
		bool WasActionReleased(const std::string& name) const;


	private:
		std::unordered_map<std::string, ActionEntry> mActions;


		static float ReadBinding(const Binding& b, InputManager* input, bool& outTrig, bool& outRel);
	};


} // namespace MochiFramework::InputSystem