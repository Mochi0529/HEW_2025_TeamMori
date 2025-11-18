#pragma once

#include "ComponentBase.h"
#include "Vector3.h"
#include "Matrix4.h"

#include <vector>
#include <algorithm>

// ===  前方宣言  === //
namespace MochiFramework::GameObjects { class GameObject; }

// === 名前空間 === //
namespace MochiFramework::Components
{
	class Transform : public MochiFramework::Components::ComponentBase<MochiFramework::Components::Transform> {
	public:
		explicit Transform(MochiFramework::GameObjects::GameObject* owner);

		//-- 基本セッター（ローカル座標系） --//
		void SetLocalPosition(const MochiFramework::Math::Vector3& pos);
		void SetLocalRotation(const MochiFramework::Math::Vector3& rot);
		void SetLocalScale   (const MochiFramework::Math::Vector3& scl);

		//-- 基本ゲッター（ローカル座標系） --//
		const MochiFramework::Math::Vector3& GetLocalPosition() const;
		const MochiFramework::Math::Vector3& GetLocalRotation() const;
		const MochiFramework::Math::Vector3& GetLocalScale()    const;

		//-- 加算操作 --//
		void Translate(const MochiFramework::Math::Vector3& delta);
		void Rotate   (const MochiFramework::Math::Vector3& delta);
		void Scale    (const MochiFramework::Math::Vector3& factor);

		//-- ワールド座標操作 --//
		MochiFramework::Math::Vector3 GetWorldPosition();
		MochiFramework::Math::Vector3 GetWorldRotation();
		MochiFramework::Math::Vector3 GetWorldScale();

		void SetWorldPosition(const MochiFramework::Math::Vector3& pos);
		void SetWorldRotation(const MochiFramework::Math::Vector3& rot);
		void SetWorldScale   (const MochiFramework::Math::Vector3& scale);

		//-- 親子関係 --//
		void SetParent  (Transform* parent, bool worldPositionStays = true);
		void AddChild   (Transform* child);
		void RemoveChild(Transform* child);

		const std::vector<Transform*>& GetChildren() const;
		Transform* GetParent() const;

		//-- ワールド行列 --//
		const MochiFramework::Math::Matrix4& GetWorldMatrix();

	private:
		void MarkDirty(); // Dirtyフラグを立てる
		void RecalculateMatrix(); // 行列を再計算

		//-- データ --//
		MochiFramework::Math::Vector3 mLocalPosition;
		MochiFramework::Math::Vector3 mLocalRotation; // オイラー角(たぶんクオータニオンに変えたほうがいい)
		MochiFramework::Math::Vector3 mLocalScale;

		MochiFramework::Math::Matrix4 mLocalMatrix;
		MochiFramework::Math::Matrix4 mWorldMatrix;

		Transform* mParent = nullptr;
		std::vector<Transform*> mChildren;

		bool mDirty = true;
	};
} // namespace MochiFramework::Components
