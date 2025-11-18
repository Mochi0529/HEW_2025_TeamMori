#pragma once
#include <string>
#include <variant> // std::variant は「型の選べる変数」

// === 名前空間 === //
namespace MochiFramework::SceneSystem
{
	// 「データクラス（DTO）」なので構造体で定義
	struct ChangeScenePayload { std::string sceneName; };
	struct PushOverlayPayload { std::string overlayName; };

	// シーンイベントで使用するペイロード型（イベントの種類ごとに異なる）
	using SceneEventData = std::variant<std::monostate, ChangeScenePayload, PushOverlayPayload>; // std::monostate はデータなしを表す

	// シーン制御イベント
	class SceneEvent {

	public:
		// イベントの種類を表す列挙型
		enum class Type {
			ChangeScene,   // メインシーン切り替え
			QuitGame,      // ゲーム終了要求
			PushOverlay,   // オーバーレイ追加
			PopOverlay,    // オーバーレイ削除
			ReloadScene    // メインシーンの再読み込み
		};

		SceneEvent() = delete; // デフォルトコンストラクタを削除

		//--- getter ---//

		Type const GetType() const;
		SceneEventData const& GetSceneEventData() const;


		//---  シーン制御用イベント生成ファクトリメソッド群 ---//

		// [nodiscard] = この関数の戻り値を無視しないでくださいというアノテーション(属性)

		[[nodiscard]] static SceneEvent ChangeTo(const std::string& name);
		[[nodiscard]] static SceneEvent PushOverlay(const std::string& name);
		[[nodiscard]] static SceneEvent Pop();
		[[nodiscard]] static SceneEvent Quit();
		[[nodiscard]] static SceneEvent Reload();


	private:
		// コンストラクタを private にすることで、整合性のある生成しか許可しないシングルトン設計
		SceneEvent(Type t, SceneEventData d);

		// イベント本体
		Type type; // イベントの種類(シーン遷移・ゲーム終了など)
		SceneEventData data; // イベントに関連するデータ(シーン名・オーバーレイ名など)

		// 軽量かつ寿命がはっきりしているので直接持つ
	};
} // namespace MochiFramework
