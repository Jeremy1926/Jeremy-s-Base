#pragma once
#include <external/includes/includes.hpp>
#include <external/math/vector.hpp>
#include <corecrt_math_defines.h>
#include <external/communication/communication.hpp>
#include <external/defines.hpp>

namespace Pointers
{
	inline uintptr_t UWorld;
	inline uintptr_t OwningGameInstance;
	inline uintptr_t PersistentLevel;
	inline uintptr_t LocalPlayers;
	inline uintptr_t LocalPlayer;
	inline uintptr_t PlayerController;
	inline uintptr_t LocalPawn;
	inline uintptr_t GameState;
	inline uintptr_t PlayerState;
	inline uintptr_t PawnPrivate;
	inline uintptr_t Mesh;
	inline uintptr_t PlayerArray;
	inline DWORD_PTR RootComponent;
	inline Vector3 RelativeLocation;
    inline float ClosestDistance;
    inline uintptr_t ClosestPawn;
    inline uintptr_t ClosestActor;
	inline int MyTeamID;
	inline int PlayerCount;
}

inline bool IsVisible(uintptr_t Mesh)
{
    auto LastSubmitTime = read<double>(Pointers::UWorld + 0x160);
    auto LastRenderTimeOnScreen = read<float>(Mesh + 0x32C);
    bool Visible = LastRenderTimeOnScreen + 0.06f >= LastSubmitTime;
    return Visible;
}

struct FTransform
{
    FQuat Rot;
    Vector3 Translation;
    char Pad[4];
    Vector3 Scale;
    char Pad1[4];

    D3DMATRIX ToMatrixWithScale()
    {
        Vector3 AdjustedScale
        (
            (Scale.x == 0.0f) ? 1.0f : Scale.x,
            (Scale.y == 0.0f) ? 1.0f : Scale.y,
            (Scale.z == 0.0f) ? 1.0f : Scale.z
        );

        D3DMATRIX M{};
        M._41 = Translation.x;
        M._42 = Translation.y;
        M._43 = Translation.z;

        float X2 = Rot.x + Rot.x;
        float Y2 = Rot.y + Rot.y;
        float Z2 = Rot.z + Rot.z;
        float Xx2 = Rot.x * X2;
        float Yy2 = Rot.y * Y2;
        float Zz2 = Rot.z * Z2;

        M._11 = (1.0f - (Yy2 + Zz2)) * AdjustedScale.x;
        M._22 = (1.0f - (Xx2 + Zz2)) * AdjustedScale.y;
        M._33 = (1.0f - (Xx2 + Yy2)) * AdjustedScale.z;

        float Yz2 = Rot.y * Z2;
        float Wx2 = Rot.w * X2;
        M._32 = (Yz2 - Wx2) * AdjustedScale.z;
        M._23 = (Yz2 + Wx2) * AdjustedScale.y;

        float Xy2 = Rot.x * Y2;
        float Wz2 = Rot.w * Z2;
        M._21 = (Xy2 - Wz2) * AdjustedScale.y;
        M._12 = (Xy2 + Wz2) * AdjustedScale.x;

        float Xz2 = Rot.x * Z2;
        float Wy2 = Rot.w * Y2;
        M._31 = (Xz2 + Wy2) * AdjustedScale.z;
        M._13 = (Xz2 - Wy2) * AdjustedScale.x;

        M._14 = 0.0f;
        M._24 = 0.0f;
        M._34 = 0.0f;
        M._44 = 1.0f;

        return M;
    }
};

inline D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
    D3DMATRIX pOut;
    pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
    pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
    pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
    pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
    pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
    pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
    pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
    pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
    pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
    pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
    pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
    pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
    pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
    pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
    pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
    pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

    return pOut;
}

inline Vector3 GetBone(uintptr_t Mesh, int BoneIndex)
{
	int32_t ActiveTransforms = read<int32_t>(Mesh + 0x5C8 + 0x48);
	uintptr_t BoneArrayPtr = Mesh + 0x5C8 + (ActiveTransforms * 0x10);

	uintptr_t BoneDataPtr = read<uintptr_t>(BoneArrayPtr);
	int32_t BoneCount = read<int32_t>(BoneArrayPtr + 8);

	if (BoneIndex < 0 || BoneIndex >= BoneCount)
	{
		return Vector3();
	}

	FTransform Bone = read<FTransform>(BoneDataPtr + (BoneIndex * sizeof(FTransform)));
	FTransform ComponentToWorld = read<FTransform>(Mesh + 0x1e0);
	D3DMATRIX matrix = MatrixMultiplication(Bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

	return Vector3(matrix._41, matrix._42, matrix._43);
}

inline D3DMATRIX Matrix(Vector3 Rot, Vector3 Origin = Vector3(0, 0, 0))
{
    float RadPitch = (Rot.x * M_PI / 180);
    float RadYaw = (Rot.y * M_PI / 180);
    float RadRoll = (Rot.z * M_PI / 180);
    float Sp = sinf(RadPitch);
    float Cp = cosf(RadPitch);
    float Sy = sinf(RadYaw);
    float Cy = cosf(RadYaw);
    float Sr = sinf(RadRoll);
    float Cr = cosf(RadRoll);

    D3DMATRIX Matrix{};
    Matrix.m[0][0] = Cp * Cy;
    Matrix.m[0][1] = Cp * Sy;
    Matrix.m[0][2] = Sp;
    Matrix.m[0][3] = 0.f;

    Matrix.m[1][0] = Sr * Sp * Cy - Cr * Sy;
    Matrix.m[1][1] = Sr * Sp * Sy + Cr * Cy;
    Matrix.m[1][2] = -Sr * Cp;
    Matrix.m[1][3] = 0.f;

    Matrix.m[2][0] = -(Cr * Sp * Cy + Sr * Sy);
    Matrix.m[2][1] = Cy * Sr - Cr * Sp * Sy;
    Matrix.m[2][2] = Cr * Cp;
    Matrix.m[2][3] = 0.f;

    Matrix.m[3][0] = Origin.x;
    Matrix.m[3][1] = Origin.y;
    Matrix.m[3][2] = Origin.z;
    Matrix.m[3][3] = 1.f;

    return Matrix;
}

namespace Camera
{
    inline Vector3 Location;
    inline Vector3 Rotation;
    inline float FieldOfView;
};

inline void UpdateCamera()
{
    auto LocationPointer = read<uintptr_t>(Pointers::UWorld + 0x140);
    auto RotationPointer = read<uintptr_t>(Pointers::UWorld + 0x150);

    double Pitch = read<double>(RotationPointer);
    double Yaw = read<double>(RotationPointer + 0x20);
    double Roll = read<double>(RotationPointer + 0x1d0);

    Camera::Rotation.x = asin(Roll) * (180.0 / M_PI);
    Camera::Rotation.y = ((atan2(Pitch * -1, Yaw) * (180.0 / M_PI)) * -1) * -1;
    Camera::Rotation.z = 0;
    Camera::Location = read<Vector3>(LocationPointer);
    Camera::FieldOfView = read<float>(Pointers::PlayerController + 0x3ac) * 90.0f;
}

inline Vector3 ProjectWorldToScreen(Vector3 WorldLocation)
{
    UpdateCamera();
    D3DMATRIX TempMatrix = Matrix(Camera::Rotation);
    Vector3 vAxisX = Vector3(TempMatrix.m[0][0], TempMatrix.m[0][1], TempMatrix.m[0][2]);
    Vector3 vAxisY = Vector3(TempMatrix.m[1][0], TempMatrix.m[1][1], TempMatrix.m[1][2]);
    Vector3 vAxisZ = Vector3(TempMatrix.m[2][0], TempMatrix.m[2][1], TempMatrix.m[2][2]);
    Vector3 vDelta = WorldLocation - Camera::Location;
    Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));
    if (vTransformed.z < 1.f)
        vTransformed.z = 1.f;
    return Vector3((Width / 2.0f) + vTransformed.x * (((Width / 2.0f) / tanf(Camera::FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, (Height / 2.0f) - vTransformed.y * (((Width / 2.0f) / tanf(Camera::FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, 0);
}
