#include "Transform.h"
#include "GameObject.h"

// === 名前空間使用宣言 === //
using namespace MochiFramework::GameObjects;
using namespace MochiFramework::Components;
using namespace MochiFramework::Math;

// === 名前空間 === //
namespace MochiFramework::Components
{
    //--- コンストラクタ ---//
    Transform::Transform(GameObject* owner)
        : ComponentBase(owner)
        , mLocalPosition(0, 0, 0)
        , mLocalRotation(0, 0, 0)
        , mLocalScale(1, 1, 1)
    {
    }

    // --- セッター --- //
    void Transform::SetLocalPosition(const Vector3& pos) { mLocalPosition = pos; MarkDirty(); }
    void Transform::SetLocalRotation(const Vector3& rot) { mLocalRotation = rot; MarkDirty(); }
    void Transform::SetLocalScale   (const Vector3& scl) { mLocalScale = scl;   MarkDirty(); }

    //--- ゲッター ---//
    const Vector3& Transform::GetLocalPosition() const { return mLocalPosition; }
    const Vector3& Transform::GetLocalRotation() const { return mLocalRotation; }
    const Vector3& Transform::GetLocalScale()    const { return mLocalScale; }

    //--- 加算操作 ---//
    void Transform::Translate(const Vector3& delta) { mLocalPosition += delta; MarkDirty(); }
    void Transform::Rotate   (const Vector3& delta) { mLocalRotation += delta; MarkDirty(); }
    void Transform::Scale    (const Vector3& factor)
    {
        mLocalScale.x *= factor.x;
        mLocalScale.y *= factor.y;
        mLocalScale.z *= factor.z;
        MarkDirty();
    }

    //--- ワールド座標操作 ---//
    Vector3 Transform::GetWorldPosition() { return GetWorldMatrix().GetTranslation(); }
    Vector3 Transform::GetWorldScale() { return GetWorldMatrix().ExtractScale(); }
    Vector3 Transform::GetWorldRotation() { return GetWorldMatrix().ExtractRotation(); }

    void Transform::SetWorldPosition(const Vector3& pos) {
        if (mParent) {
            Matrix4 invParent = mParent->GetWorldMatrix().Inverted();
            mLocalMatrix = Matrix4::Translation(pos) * invParent;
            mLocalPosition = mLocalMatrix.GetTranslation();
            mLocalRotation = mLocalMatrix.ExtractRotation();
            mLocalScale = mLocalMatrix.ExtractScale();
        }
        else {
            mLocalPosition = pos;
        }
        MarkDirty();
    }

    void Transform::SetWorldRotation(const Vector3& rot) {
        if (mParent) {
            Matrix4 invParent = mParent->GetWorldMatrix().Inverted();
            mLocalMatrix = Matrix4::RotationX(rot.x) *
                Matrix4::RotationY(rot.y) *
                Matrix4::RotationZ(rot.z) * invParent;
            mLocalRotation = mLocalMatrix.ExtractRotation();
        }
        else {
            mLocalRotation = rot;
        }
        MarkDirty();
    }

    void Transform::SetWorldScale(const Vector3& scale) {
        if (mParent) {
            Vector3 parentScale = mParent->GetWorldScale();
            mLocalScale = { scale.x / parentScale.x,
                            scale.y / parentScale.y,
                            scale.z / parentScale.z };
        }
        else {
            mLocalScale = scale;
        }
        MarkDirty();
    }

    //--- 親子関係 ---//
    void Transform::SetParent(Transform* parent, bool worldPositionStays) {
        if (mParent == parent) return;

        Matrix4 worldBefore = GetWorldMatrix();

        if (mParent) {
            mParent->RemoveChild(this);
        }

        mParent = parent;
        if (mParent) {
            mParent->AddChild(this);
        }

        if (worldPositionStays) {
            Matrix4 invParent = (mParent) ? mParent->GetWorldMatrix().Inverted()
                : Matrix4::IdentityMatrix();
            mLocalMatrix = worldBefore * invParent;
            mLocalPosition = mLocalMatrix.GetTranslation();
            mLocalRotation = mLocalMatrix.ExtractRotation();
            mLocalScale = mLocalMatrix.ExtractScale();
        }

        MarkDirty();
    }

    void Transform::AddChild(Transform* child) {
        if (child && std::find(mChildren.begin(), mChildren.end(), child) == mChildren.end()) {
            mChildren.push_back(child);
            child->mParent = this;
            child->MarkDirty();
        }
    }

    void Transform::RemoveChild(Transform* child) {
        auto it = std::remove(mChildren.begin(), mChildren.end(), child);
        if (it != mChildren.end()) {
            mChildren.erase(it, mChildren.end());
            child->mParent = nullptr;
            child->MarkDirty();
        }
    }

    const std::vector<Transform*>& Transform::GetChildren() const { return mChildren; }
    Transform* Transform::GetParent() const { return mParent; }

    //--- ワールド行列 ---//
    const Matrix4& Transform::GetWorldMatrix() {
        if (mDirty) {
            RecalculateMatrix();
        }
        return mWorldMatrix;
    }

    void Transform::MarkDirty() {
        mDirty = true;
        for (auto* child : mChildren) {
            child->MarkDirty();
        }
    }

    void Transform::RecalculateMatrix() {
        Matrix4 scaleMat = Matrix4::Scale(mLocalScale);
        Matrix4 rotX = Matrix4::RotationX(mLocalRotation.x);
        Matrix4 rotY = Matrix4::RotationY(mLocalRotation.y);
        Matrix4 rotZ = Matrix4::RotationZ(mLocalRotation.z);
        Matrix4 transMat = Matrix4::Translation(mLocalPosition);

        mLocalMatrix = scaleMat * rotX * rotY * rotZ * transMat;

        if (mParent) {
            mWorldMatrix = mLocalMatrix * mParent->GetWorldMatrix();
        }
        else {
            mWorldMatrix = mLocalMatrix;
        }

        mDirty = false;
    }
} // namespace MochiFramework::Components
