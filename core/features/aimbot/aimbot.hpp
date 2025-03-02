#pragma once
#include <Windows.h>

class Aimbot
{
public:
	auto RunAimbot(uintptr_t TargetMesh, uintptr_t TargetActor) -> void;
};

inline Aimbot Aimbot_;