#pragma once
#include <external/includes/includes.hpp>
#include <core//features/aimbot/aimbot.hpp>

void Aimbot::RunAimbot(uintptr_t TargetMesh, uintptr_t TargetActor)
{
	int ScreenCenterX = Width / 2;
	int ScreenCenterY = Height / 2;

	if (!TargetMesh) return;
	if (!IsVisible(TargetMesh)) return;

	Vector3 Head3d = GetBone(TargetMesh, 110);
	Vector3 RootHeadOut;

	RootHeadOut = ProjectWorldToScreen(Head3d);

	Vector3 Target{};

	if (RootHeadOut.x != 0)
	{
		if (RootHeadOut.x > ScreenCenterX)
		{
			Target.x = -(ScreenCenterX - RootHeadOut.x);

			Target.x /= 2;

			if (Target.x + ScreenCenterX > ScreenCenterX * 2) Target.x = 0;
		}
		if (RootHeadOut.x < ScreenCenterX)
		{
			Target.x = RootHeadOut.x - ScreenCenterX;

			Target.x /= 2;

			if (Target.x + ScreenCenterX < 0) Target.x = 0;
		}
	}

	if (RootHeadOut.y != 0)
	{
		if (RootHeadOut.y > ScreenCenterY)
		{
			Target.y = -(ScreenCenterY - RootHeadOut.y);

			Target.y /= 2;

			if (Target.y + ScreenCenterY > ScreenCenterY * 2) Target.y = 0;
		}
		if (RootHeadOut.y < ScreenCenterY)
		{
			Target.y = RootHeadOut.y - ScreenCenterY;

			Target.y /= 2;

			if (Target.y + ScreenCenterY < 0) Target.y = 0;
		}
	}

	Driver::MoveMouse(Target.x, Target.y, NULL);
}