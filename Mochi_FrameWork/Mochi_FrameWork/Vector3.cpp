#include "Vector3.h"

namespace MochiFramework::Math {

    //-- コンストラクタ --//
    constexpr Vector3::Vector3() : x(0), y(0), z(0) {}
    constexpr Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    //-- 演算子オーバーロード --//
    Vector3 Vector3::operator+(const Vector3& rhs) const { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
    Vector3 Vector3::operator-(const Vector3& rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
    Vector3 Vector3::operator*(float scalar) const { return { x * scalar, y * scalar, z * scalar }; }
    Vector3 Vector3::operator/(float scalar) const { return { x / scalar, y / scalar, z / scalar }; }

    Vector3& Vector3::operator+=(const Vector3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
    Vector3& Vector3::operator-=(const Vector3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
    Vector3& Vector3::operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
    Vector3& Vector3::operator/=(float scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

    bool Vector3::operator==(const Vector3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
    bool Vector3::operator!=(const Vector3& rhs) const { return !(*this == rhs); }

    //-- ベクトル演算 --//
    float Vector3::LengthSq() const { return x * x + y * y + z * z; }
    float Vector3::Length() const { return std::sqrt(LengthSq()); }

    Vector3 Vector3::Normalized() const {
        float len = Length();
        return (len > 0) ? (*this / len) : Vector3();
    }

    void Vector3::Normalize() {
        float len = Length();
        if (len > 0) {
            x /= len; y /= len; z /= len;
        }
    }

    float Vector3::Dot(const Vector3& a, const Vector3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    Vector3 Vector3::Cross(const Vector3& a, const Vector3& b) {
        return {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }

    float Vector3::Distance(const Vector3& a, const Vector3& b) {
        return (a - b).Length();
    }

    float Vector3::DistanceSq(const Vector3& a, const Vector3& b) {
        return (a - b).LengthSq();
    }

    //-- 定数ベクトル --//
    const Vector3 Vector3::zero     = Vector3( 0.0f,  0.0f,  0.0f);
    const Vector3 Vector3::one      = Vector3( 1.0f,  1.0f,  1.0f);
    const Vector3 Vector3::up       = Vector3( 0.0f,  1.0f,  0.0f);
    const Vector3 Vector3::down     = Vector3( 0.0f, -1.0f,  0.0f);
    const Vector3 Vector3::left     = Vector3(-1.0f,  0.0f,  0.0f);
    const Vector3 Vector3::right    = Vector3( 1.0f,  0.0f,  0.0f);
    const Vector3 Vector3::forward  = Vector3( 0.0f,  0.0f,  1.0f);
    const Vector3 Vector3::back     = Vector3( 0.0f,  0.0f, -1.0f);

    //-- ユーティリティ --//
    Vector3 Vector3::MakeFloat3(float x, float y, float z) {
        return { x, y, z };
    }


} // namespace Math
