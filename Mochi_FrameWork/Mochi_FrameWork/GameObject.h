#pragma once

#include <vector> 
#include <memory>
#include <unordered_map>
#include <typeindex> // std::type_index を使用して型情報を管理
#include <algorithm> // std::find_if を使用してコンポーネントの順序を維持
#include <stdexcept> // 例外処理のために std::runtime_error を使用
#include <string> 

#include "Component.h"

namespace MochiFramework::SceneSystem { class Scene; }
namespace MochiFramework::Components { template <class T> class ComponentBase; }

// === 名前空間 === //
namespace MochiFramework::GameObjects
{
	class GameObject
	{

	public:
		// ===  列挙型（enum）  === //

		enum State { // ゲームオブジェクトの状態
			eActive, ePause, eDead
		};

	protected: // protected にすることで、直接 new できなくする(CRTP補強)

		// ===  コンストラクタ・デストラクタ  === //

		explicit GameObject(MochiFramework::SceneSystem::Scene* scene);

		// コピー禁止
		GameObject(const MochiFramework::GameObjects::GameObject&) = delete;
		GameObject& operator=(const MochiFramework::GameObjects::GameObject&) = delete;

		// GameObjectBase<T> だけ継承可能に制限((CRTP補強))
		template <typename Derived>
		friend class GameObjectBase;

	public:
		virtual ~GameObject(); 

		// ===  公開関数（外部インターフェース）  === //

		void Input();
		void Update(const float deltaTime);

		void RequestDestroy();

		virtual void OnDestroy(); // 削除される瞬間の処理

		//-- コンポーネント --//

		template <typename T, typename... Args>
		T* AddComponent(Args&&... args);

		template <typename T>
		T* GetComponent() const;

		template <typename T>
		void RemoveComponent();

		template <typename T>
		bool HasComponent() const {
			return mComponentMap.count(typeid(T)) > 0;
		}

		// 同じ型を複数持たせたい要件が将来的に出てきたら、mComponentMap の値を std::vector<Component*> にする設計も考える

		//-- ゲッター・セッター --//

		// 型判定用（CRTP）
		virtual std::type_index GetType() const = 0;

		void SetName(const std::string& name);
		const std::string& GetName() const;

		void SetTag(const std::string& tag);
		const std::string& GetTag() const;

		MochiFramework::SceneSystem::Scene* GetScene() const;

		void SetState(State state);
		State GetState() const;

	protected:
		// ===  継承先でオーバーライド可能な関数（フック処理）  === //

		// 派生クラスでそのクラス固有の処理を書く
		virtual void GameObjectInput();
		virtual void UpdateGameObject();

	private:
		// ===  メンバ変数(内部変数)  === //

		std::string mName;
		std::string mTag;

		State mState; // 現在の状態
		MochiFramework::SceneSystem::Scene* const mScene;

		std::vector<std::unique_ptr<MochiFramework::Components::Component>> mComponents;
		std::unordered_map<std::type_index, MochiFramework::Components::Component*> mComponentMap;
	};



	// ===  テンプレート関数の定義  === //

	template <typename T, typename... Args>
	T* GameObject::AddComponent(Args&&... args) {
		static_assert(std::is_base_of<MochiFramework::Components::ComponentBase<T>, T>::value, "You must inherit from ComponentBase<T>");

		if (mComponentMap.count(typeid(T)) > 0) {
			throw std::runtime_error(std::string("Component already exists: ") + typeid(T).name()); // 存在していた場合に例外を投げて処理を中断

		}
		T* comp = new T(this, std::forward<Args>(args)...); //std::forward<Args>(args)...で引数を完璧転送
		std::unique_ptr<MochiFramework::Components::Component> uptr(comp);

		// 登録済みコンポーネントの中で、comp より更新順序（UpdateOrder）が大きい最初の位置を探す
		auto iter = std::find_if(mComponents.begin(), mComponents.end(),
			[comp](const std::unique_ptr<MochiFramework::Components::Component>& existing) {
				return comp->GetUpdateOrder() < existing->GetUpdateOrder();
			});

		auto inserted = mComponents.insert(iter, std::move(uptr)); // 優先度順を維持したまま、適切な位置に挿入

		mComponentMap[typeid(T)] = comp; // コンポーネントマップに登録(typeid(T)：型 T の一意な型情報)

		return static_cast<T*>(inserted->get()); // 呼び出し元がすぐに操作できるよう、元の型でポインタを返す
	}

	template <typename T>
	T* GameObject::GetComponent() const {
		auto it = mComponentMap.find(typeid(T));
		if (it != mComponentMap.end()) {
			return static_cast<T*>(it->second);
		}
		return nullptr;
	}

	template <typename T>
	void GameObject::RemoveComponent() {
		auto it = mComponentMap.find(typeid(T));
		if (it != mComponentMap.end()) {
			MochiFramework::Components::Component* target = it->second;
			// vector から削除
			mComponents.erase(std::remove_if(mComponents.begin(), mComponents.end(),
				[target](const std::unique_ptr<MochiFramework::Components::Component>& comp) {
					return comp.get() == target;
				}), mComponents.end());

			mComponentMap.erase(it);
		}
	}
} // namespace MochiFramework::GameObjects