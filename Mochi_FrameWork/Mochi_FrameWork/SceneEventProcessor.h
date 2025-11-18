#pragma once
#include "SceneEvent.h"

#include <unordered_map>
#include <functional>

namespace MochiFramework::SceneSystem { class SceneStack; }
namespace MochiFramework::SceneSystem { class EventQueue; }

// === 名前空間 === //
namespace MochiFramework::SceneSystem
{
	class SceneEventProcessor { // イベントキューからイベントを取り出して SceneStack に指示を出す

	public:
		// ===  コンストラクタ・デストラクタ  === //

		SceneEventProcessor(SceneStack& stack, EventQueue& queue);

		// ===  公開関数（外部インターフェース）  === //

		void Process();

		bool ShouldQuit() const;



	private:
		// ===  補助関数（内部ロジック）  === //
		
		void ProcessEvent(MochiFramework::SceneSystem::SceneEvent&& e);              // 単一のシーンイベントを処理
		void RegisterHandlers();

		// ===  メンバ変数(内部変数)  === //

		bool mQuitRequested;

		std::unordered_map<MochiFramework::SceneSystem::SceneEvent::Type, std::function<void(const MochiFramework::SceneSystem::SceneEvent&)>> mHandlers; // イベントタイプごとの関数マップ

		SceneStack& mSceneStack;
		EventQueue& mEventQueue;
		// --- 参照の理由 --- //
		// ・ポインタだとnullチェックが必要
		// ・呼び出し側が nullptr を渡すミスのリスクがある
		// ・「絶対に有効なオブジェクトが渡されている」という設計意図
	};
} // namespace MochiFramework