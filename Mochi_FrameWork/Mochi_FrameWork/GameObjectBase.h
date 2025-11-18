#pragma once
#include "GameObject.h"

// CRTP を使用して、GameObjectの派生クラスを定義

// === 名前空間 === //
namespace MochiFramework::GameObjects
{
	template <typename Derived>
	class GameObjectBase : public GameObject {
	public:
		using GameObject::GameObject; // コンストラクタ継承
		
	protected:
		std::type_index GetType() const final override { return typeid(Derived); } // 静的型情報を壊されないように安全を確保
	};
} // namespace MochiFramework::GameObjects