#pragma once
#include <array>
#include "Vector3.h"

namespace MochiFramework::Math {

    //====================================
    // 注意　左手系座標
    //====================================
    class Matrix4 {
    public:
    public:
        std::array<float, 16> m; // 行優先 (row-major)

        // --- コンストラクタ ---
        Matrix4();       // 単位行列で初期化

        // --- 基本操作 ---
        void Identity();               // 自身を単位行列に設定

        // --- 変換行列生成 ---
        static Matrix4 Translation(const Vector3& v);
        static Matrix4 Scale(const Vector3& v);
        static Matrix4 RotationX(float rad);
        static Matrix4 RotationY(float rad); // 左手系対応
        static Matrix4 RotationZ(float rad);

        // --- 行列演算 ---
        Matrix4 operator*(const Matrix4& rhs) const;

        // --- 座標変換 ---
        Vector3 TransformPoint(const Vector3& v) const;   // 点（平行移動あり）
        Vector3 TransformVector(const Vector3& v) const;  // ベクトル（平行移動なし）

        // --- ユーティリティ ---
        static Matrix4 IdentityMatrix(); // 単位行列を返す（static）
        Matrix4 Inverted() const;        // 逆行列を返す
        Vector3 GetTranslation() const;  // 平行移動成分を取得
        Vector3 ExtractScale() const;    // スケール成分を取得
        Vector3 ExtractRotation() const; // 回転成分をオイラー角で取得
    };

} // namespace Math
