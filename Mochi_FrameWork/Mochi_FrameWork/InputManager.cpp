#include "InputManager.h"
#include <cmath>
#include <cstring>

#include <iostream>


namespace MochiFramework::InputSystem {


	InputManager::InputManager(int playerIndex) : mPlayerIndex(playerIndex) {

		// \‘¢‘Ì‚ğ0‚Å‰Šú‰»
		std::memset(&mPadState, 0, sizeof(mPadState));
		std::memset(&mPrevPadState, 0, sizeof(mPrevPadState));
	}


	InputManager::~InputManager() {}


	void InputManager::Update() {
		// keep prev
		mPrevKeyState = mKeyState;
		mPrevPadState = mPadState;


		// keyboard
		for (int i = 0; i < 256; ++i) {
			SHORT keyState = GetAsyncKeyState(i);
			mKeyState[i] = (keyState & 0x8000) ? 0x80 : 0x00;
		}

		// gamepad
		::XInputGetState(mPlayerIndex, &mPadState);
	}


	// --- Keyboard ---
	bool InputManager::GetKeyPress(int vk)   const { return (mKeyState[vk] & 0x80) != 0; }
	bool InputManager::GetKeyTrigger(int vk) const { return (mKeyState[vk] & 0x80) && !(mPrevKeyState[vk] & 0x80); }
	bool InputManager::GetKeyRelease(int vk) const { return !(mKeyState[vk] & 0x80) && (mPrevKeyState[vk] & 0x80); }


	// --- Gamepad ---
	bool InputManager::GetButtonPress(WORD btn)   const { return (mPadState.Gamepad.wButtons & btn) != 0; }
	bool InputManager::GetButtonTrigger(WORD btn) const { return (mPadState.Gamepad.wButtons & btn) && !(mPrevPadState.Gamepad.wButtons & btn); }
	bool InputManager::GetButtonRelease(WORD btn) const { return !(mPadState.Gamepad.wButtons & btn) && (mPrevPadState.Gamepad.wButtons & btn); }


	// --- Sticks ---
	float InputManager::GetLeftStickX()  const { float v = mPadState.Gamepad.sThumbLX / 32767.0f; return (std::fabs(v) < DEADZONE) ? 0.0f : v; }
	float InputManager::GetLeftStickY()  const { float v = mPadState.Gamepad.sThumbLY / 32767.0f; return (std::fabs(v) < DEADZONE) ? 0.0f : v; }
	float InputManager::GetRightStickX() const { float v = mPadState.Gamepad.sThumbRX / 32767.0f; return (std::fabs(v) < DEADZONE) ? 0.0f : v; }
	float InputManager::GetRightStickY() const { float v = mPadState.Gamepad.sThumbRY / 32767.0f; return (std::fabs(v) < DEADZONE) ? 0.0f : v; }


	// --- Triggers ---
	float InputManager::GetLeftTrigger()  const { return mPadState.Gamepad.bLeftTrigger / 255.0f; }
	float InputManager::GetRightTrigger() const { return mPadState.Gamepad.bRightTrigger / 255.0f; }


} // namespace MochiFramework::InputSystem