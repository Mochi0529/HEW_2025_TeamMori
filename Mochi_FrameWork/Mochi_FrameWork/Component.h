#pragma once

#include <typeindex> // std::type_index を使用して型情報を管理
#include <memory>

namespace MochiFramework::GameObjects { class GameObject; }

// === 名前空間 === //
namespace MochiFramework::Components
{
	class Component
	{
	protected: // protected にすることで、直接 new できなくする(CRTP補強)

		// ===  コンストラクタ・デストラクタ  === //

		explicit Component(MochiFramework::GameObjects::GameObject* owner, int updateOrder = 100);

		// コピー禁止
		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;

		// ComponentBase<T> だけ継承可能に制限((CRTP補強))
		template <typename Derived>
		friend class ComponentBase;

	public:
		virtual ~Component(); // デストラクタは仮想関数にすることで、派生クラスのデストラクタが正しく呼ばれるようにする

		// ===  公開関数（外部インターフェース）  === //

		virtual void Input();
		virtual void Update(const float deltaTime);

		virtual void OnOwnerDestroy();  // オーナー削除される瞬間の処理

		//-- ゲッター・セッター --//

		// 型判定用（CRTP）
		virtual std::type_index GetType() const = 0;

		MochiFramework::GameObjects::GameObject* GetOwner() const;
		int GetUpdateOrder() const;



	private:
		// ===  メンバ変数（内部状態）  === //

		MochiFramework::GameObjects::GameObject* mOwner;   // このComponentを所有するGameObject
		int mUpdateOrder;	// 優先度(小さい方優先で順番に処理)
	};
} // namespace MochiFramework


