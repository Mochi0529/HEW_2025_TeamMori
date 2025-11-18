#include "Matrix4.h"
#include <cmath>

namespace MochiFramework::Math {
    Matrix4::Matrix4() {
        Identity();
    }

    void Matrix4::Identity() {
        m = { 1,0,0,0,
             0,1,0,0,
             0,0,1,0,
             0,0,0,1 };
    }

    Matrix4 Matrix4::IdentityMatrix() {
        Matrix4 mat;
        mat.Identity();
        return mat;
    }

    Matrix4 Matrix4::Translation(const Vector3& v) {
        Matrix4 mat = IdentityMatrix();
        mat.m[12] = v.x;
        mat.m[13] = v.y;
        mat.m[14] = v.z;
        return mat;
    }

    Matrix4 Matrix4::Scale(const Vector3& v) {
        Matrix4 mat = IdentityMatrix();
        mat.m[0] = v.x;
        mat.m[5] = v.y;
        mat.m[10] = v.z;
        return mat;
    }

    Matrix4 Matrix4::RotationX(float rad) {
        Matrix4 mat = IdentityMatrix();
        float c = std::cos(rad);
        float s = std::sin(rad);
        mat.m[5] = c;  mat.m[6] = s;
        mat.m[9] = -s; mat.m[10] = c;
        return mat;
    }

    Matrix4 Matrix4::RotationY(float rad) {
        Matrix4 mat = IdentityMatrix();
        float c = std::cos(rad);
        float s = std::sin(rad);
        mat.m[0] = c;  mat.m[2] = -s;
        mat.m[8] = s;  mat.m[10] = c;
        return mat;
    }

    Matrix4 Matrix4::RotationZ(float rad) {
        Matrix4 mat = IdentityMatrix();
        float c = std::cos(rad);
        float s = std::sin(rad);
        mat.m[0] = c;  mat.m[1] = s;
        mat.m[4] = -s; mat.m[5] = c;
        return mat;
    }

    Matrix4 Matrix4::operator*(const Matrix4& rhs) const {
        Matrix4 result;
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                result.m[row * 4 + col] =
                    m[row * 4 + 0] * rhs.m[0 * 4 + col] +
                    m[row * 4 + 1] * rhs.m[1 * 4 + col] +
                    m[row * 4 + 2] * rhs.m[2 * 4 + col] +
                    m[row * 4 + 3] * rhs.m[3 * 4 + col];
            }
        }
        return result;
    }

    Vector3 Matrix4::TransformPoint(const Vector3& v) const {
        float x = v.x * m[0] + v.y * m[4] + v.z * m[8] + m[12];
        float y = v.x * m[1] + v.y * m[5] + v.z * m[9] + m[13];
        float z = v.x * m[2] + v.y * m[6] + v.z * m[10] + m[14];
        return { x,y,z };
    }

    Vector3 Matrix4::TransformVector(const Vector3& v) const {
        float x = v.x * m[0] + v.y * m[4] + v.z * m[8];
        float y = v.x * m[1] + v.y * m[5] + v.z * m[9];
        float z = v.x * m[2] + v.y * m[6] + v.z * m[10];
        return { x,y,z };
    }

    Matrix4 Matrix4::Inverted() const {
        Matrix4 inv;
        const float* a = m.data();
        float* o = inv.m.data();

        // 余因子展開による4x4逆行列計算
        float invOut[16];
        {
            invOut[0] = a[5] * a[10] * a[15] -
                a[5] * a[11] * a[14] -
                a[9] * a[6] * a[15] +
                a[9] * a[7] * a[14] +
                a[13] * a[6] * a[11] -
                a[13] * a[7] * a[10];

            invOut[4] = -a[4] * a[10] * a[15] +
                a[4] * a[11] * a[14] +
                a[8] * a[6] * a[15] -
                a[8] * a[7] * a[14] -
                a[12] * a[6] * a[11] +
                a[12] * a[7] * a[10];

            invOut[8] = a[4] * a[9] * a[15] -
                a[4] * a[11] * a[13] -
                a[8] * a[5] * a[15] +
                a[8] * a[7] * a[13] +
                a[12] * a[5] * a[11] -
                a[12] * a[7] * a[9];

            invOut[12] = -a[4] * a[9] * a[14] +
                a[4] * a[10] * a[13] +
                a[8] * a[5] * a[14] -
                a[8] * a[6] * a[13] -
                a[12] * a[5] * a[10] +
                a[12] * a[6] * a[9];

            invOut[1] = -a[1] * a[10] * a[15] +
                a[1] * a[11] * a[14] +
                a[9] * a[2] * a[15] -
                a[9] * a[3] * a[14] -
                a[13] * a[2] * a[11] +
                a[13] * a[3] * a[10];

            invOut[5] = a[0] * a[10] * a[15] -
                a[0] * a[11] * a[14] -
                a[8] * a[2] * a[15] +
                a[8] * a[3] * a[14] +
                a[12] * a[2] * a[11] -
                a[12] * a[3] * a[10];

            invOut[9] = -a[0] * a[9] * a[15] +
                a[0] * a[11] * a[13] +
                a[8] * a[1] * a[15] -
                a[8] * a[3] * a[13] -
                a[12] * a[1] * a[11] +
                a[12] * a[3] * a[9];

            invOut[13] = a[0] * a[9] * a[14] -
                a[0] * a[10] * a[13] -
                a[8] * a[1] * a[14] +
                a[8] * a[2] * a[13] +
                a[12] * a[1] * a[10] -
                a[12] * a[2] * a[9];

            invOut[2] = a[1] * a[6] * a[15] -
                a[1] * a[7] * a[14] -
                a[5] * a[2] * a[15] +
                a[5] * a[3] * a[14] +
                a[13] * a[2] * a[7] -
                a[13] * a[3] * a[6];

            invOut[6] = -a[0] * a[6] * a[15] +
                a[0] * a[7] * a[14] +
                a[4] * a[2] * a[15] -
                a[4] * a[3] * a[14] -
                a[12] * a[2] * a[7] +
                a[12] * a[3] * a[6];

            invOut[10] = a[0] * a[5] * a[15] -
                a[0] * a[7] * a[13] -
                a[4] * a[1] * a[15] +
                a[4] * a[3] * a[13] +
                a[12] * a[1] * a[7] -
                a[12] * a[3] * a[5];

            invOut[14] = -a[0] * a[5] * a[14] +
                a[0] * a[6] * a[13] +
                a[4] * a[1] * a[14] -
                a[4] * a[2] * a[13] -
                a[12] * a[1] * a[6] +
                a[12] * a[2] * a[5];

            invOut[3] = -a[1] * a[6] * a[11] +
                a[1] * a[7] * a[10] +
                a[5] * a[2] * a[11] -
                a[5] * a[3] * a[10] -
                a[9] * a[2] * a[7] +
                a[9] * a[3] * a[6];

            invOut[7] = a[0] * a[6] * a[11] -
                a[0] * a[7] * a[10] -
                a[4] * a[2] * a[11] +
                a[4] * a[3] * a[10] +
                a[8] * a[2] * a[7] -
                a[8] * a[3] * a[6];

            invOut[11] = -a[0] * a[5] * a[11] +
                a[0] * a[7] * a[9] +
                a[4] * a[1] * a[11] -
                a[4] * a[3] * a[9] -
                a[8] * a[1] * a[7] +
                a[8] * a[3] * a[5];

            invOut[15] = a[0] * a[5] * a[10] -
                a[0] * a[6] * a[9] -
                a[4] * a[1] * a[10] +
                a[4] * a[2] * a[9] +
                a[8] * a[1] * a[6] -
                a[8] * a[2] * a[5];
        }

        float det = a[0] * invOut[0] + a[1] * invOut[4] + a[2] * invOut[8] + a[3] * invOut[12];
        if (fabs(det) < 1e-6f) {
            return IdentityMatrix(); // 特異行列なら単位行列
        }

        det = 1.0f / det;
        for (int i = 0; i < 16; i++) {
            o[i] = invOut[i] * det;
        }

        return inv;
    }

    Vector3 Matrix4::GetTranslation() const {
        return { m[12], m[13], m[14] };
    }

    Vector3 Matrix4::ExtractScale() const {
        Vector3 xAxis(m[0], m[1], m[2]);
        Vector3 yAxis(m[4], m[5], m[6]);
        Vector3 zAxis(m[8], m[9], m[10]);

        return { xAxis.Length(), yAxis.Length(), zAxis.Length() };
    }

    Vector3 Matrix4::ExtractRotation() const {
        Vector3 scale = ExtractScale();

        float m00 = m[0] / scale.x;
        float m01 = m[4] / scale.y;
        float m02 = m[8] / scale.z;
        float m10 = m[1] / scale.x;
        float m11 = m[5] / scale.y;
        float m12 = m[9] / scale.z;
        float m20 = m[2] / scale.x;
        float m21 = m[6] / scale.y;
        float m22 = m[10] / scale.z;

        // 左手系オイラー角 (Pitch=X, Yaw=Y, Roll=Z)
        float pitch, yaw, roll;

        yaw = std::atan2(m02, m22);   // Y軸回り
        pitch = std::asin(-m12);        // X軸回り
        roll = std::atan2(m10, m11);   // Z軸回り

        return { pitch, yaw, roll };
    }

} // namespace Math
