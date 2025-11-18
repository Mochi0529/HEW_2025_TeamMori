#pragma once
#include "Component.h"

// CRTP を使用して、Componentの派生クラスを定義

// === 名前空間 === //
namespace MochiFramework::Components
{
	template <typename Derived>
	class ComponentBase : public Component {
	public:
		using Component::Component; // コンストラクタ継承
		
	protected:
		std::type_index GetType() const final override { return typeid(Derived); } // 静的型情報を壊されないように安全を確保
	};
} // namespace MochiFramework::Components