#pragma once
#include <cmath>
#include <ostream>

namespace MochiFramework::Math {

    class Vector3 {
    public:
        float x, y, z;

        //-- コンストラクタ --//
        constexpr Vector3();
        constexpr Vector3(float x, float y, float z);
        constexpr Vector3(const Vector3& v) = default;

        //-- 演算子オーバーロード --//
        Vector3 operator+(const Vector3& rhs) const;
        Vector3 operator-(const Vector3& rhs) const;
        Vector3 operator*(float scalar) const;
        Vector3 operator/(float scalar) const;

        Vector3& operator+=(const Vector3& rhs);
        Vector3& operator-=(const Vector3& rhs);
        Vector3& operator*=(float scalar);
        Vector3& operator/=(float scalar);

        bool operator==(const Vector3& rhs) const;
        bool operator!=(const Vector3& rhs) const;

        //-- ベクトル演算 --//
        float LengthSq() const;
        float Length() const;

        Vector3 Normalized() const;
        void Normalize();

        static float Dot(const Vector3& a, const Vector3& b);
        static Vector3 Cross(const Vector3& a, const Vector3& b);

        static float Distance(const Vector3& a, const Vector3& b);
        static float DistanceSq(const Vector3& a, const Vector3& b);
        
        //-- 定数ベクトル --//
        static const Vector3 zero;
        static const Vector3 one;
        static const Vector3 up;
        static const Vector3 down;
        static const Vector3 left;
        static const Vector3 right;
        static const Vector3 forward;
        static const Vector3 back;
        // 定数式不可(constexprを使用していない)

        //-- ユーティリティ --//
        static Vector3 MakeFloat3(float x, float y, float z);
    };
} // namespace Math
