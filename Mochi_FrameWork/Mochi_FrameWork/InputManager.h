#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <array>


#pragma comment(lib, "xinput.lib")


namespace MochiFramework::InputSystem {


	class InputManager {
	public:
		explicit InputManager(int playerIndex = 0);
		~InputManager();


		// 1フレーム分のデバイス状態を更新
		void Update();


		// --- Keyboard ---
		bool GetKeyPress(int vk) const;
		bool GetKeyTrigger(int vk) const;
		bool GetKeyRelease(int vk) const;


		// --- Gamepad Buttons ---
		bool GetButtonPress(WORD btn) const;
		bool GetButtonTrigger(WORD btn) const;
		bool GetButtonRelease(WORD btn) const;


		// --- Analog Sticks ---
		float GetLeftStickX() const;
		float GetLeftStickY() const;
		float GetRightStickX() const;
		float GetRightStickY() const;


		// --- Triggers ---
		float GetLeftTrigger() const; // 0..1
		float GetRightTrigger() const; // 0..1


	private:
		int mPlayerIndex;


		std::array<BYTE, 256> mKeyState{};
		std::array<BYTE, 256> mPrevKeyState{};


		XINPUT_STATE mPadState{};
		XINPUT_STATE mPrevPadState{};


		static constexpr float DEADZONE = 0.2f; // stick deadzone（正規化後）
	};


} // namespace MochiFramework::InputSystem