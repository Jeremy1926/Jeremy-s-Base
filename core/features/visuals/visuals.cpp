#pragma once
#include <core/features/visuals/visuals.hpp>
#include <external/includes/includes.hpp>
#include <external/communication/communication.hpp>
#include <external/settings/settings.hpp>
#include <core/features/aimbot/aimbot.hpp>
#include <updater/offsets/offsets.hpp>
#include <string>

void DrawNormalBox(int X, int Y, int W, int H, ImColor Color, int Thickness)
{
	float LineW = (W / 1);
	float LineH = (H / 1);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + LineH), Color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + LineW, Y), Color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W - LineW, Y), ImVec2(X + W, Y), Color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + LineH), Color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y + H - LineH), ImVec2(X, Y + H), Color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + LineW, Y + H), Color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W - LineW, Y + H), ImVec2(X + W, Y + H), Color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W, Y + H - LineH), ImVec2(X + W, Y + H), Color, Thickness);
}

void DrawCornerBox(int X, int Y, int W, int H, ImColor Color, int Thickness)
{
	float LineW = (W / 4);
	float LineH = (H / 4);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + LineH), Color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + LineW, Y), Color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W - LineW, Y), ImVec2(X + W, Y), Color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + LineH), Color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y + H - LineH), ImVec2(X, Y + H), Color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + LineW, Y + H), Color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W - LineW, Y + H), ImVec2(X + W, Y + H), Color, Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W, Y + H - LineH), ImVec2(X + W, Y + H), Color, Thickness);
}

auto Visuals::LoopPlayers() -> void
{
	Pointers::ClosestDistance = FLT_MAX;
	Pointers::ClosestActor = NULL;
	Pointers::ClosestPawn = NULL;

	for (uint32_t i = 0; i < Pointers::PlayerCount; i++)
	{
		Pointers::PlayerState = read<uintptr_t>(Pointers::PlayerArray + (i * sizeof(uintptr_t)));

		uintptr_t PlayerState = read<uintptr_t>(Pointers::PlayerArray + (i * sizeof(uintptr_t)));

		int PlayerTeam = read<int>(Pointers::PlayerState + Offsets::TeamIndex);

		if (!PlayerState)
			continue;

		Pointers::PawnPrivate = read<uintptr_t>(PlayerState + Offsets::PawnPrivate);

		if (!Pointers::PawnPrivate)
			continue;

		Pointers::Mesh = read<uintptr_t>(Pointers::PawnPrivate + Offsets::Mesh);

		if (!Pointers::Mesh)
			continue;

		if (Pointers::LocalPawn)
		{
			if (PlayerTeam == Pointers::MyTeamID)
				continue;

			if (Pointers::LocalPawn == Pointers::PawnPrivate)
				continue;
		}

		Vector3 Head = GetBone(Pointers::Mesh, 110);
		Vector3 Bottom = GetBone(Pointers::Mesh, 0);
		Vector3 Bottom2D = ProjectWorldToScreen(Bottom);
		Vector3 Head2D = ProjectWorldToScreen(Vector3(Head.x, Head.y, Head.z + 15));

		float CornerHeight = abs(Head2D.y - Bottom2D.y);
		float BoxHeight = abs(Head2D.y - Bottom2D.y);
		float CornerWidth = BoxHeight * 0.40;
		float BoxWidth = BoxHeight * 0.50;

		float Distance = Pointers::RelativeLocation.GetDistance(Bottom) / 100;

		if (jtx::Box)
		{
			if (IsVisible(Pointers::Mesh))
			{
				DrawNormalBox(Head2D.x - (BoxWidth / 2), Head2D.y, BoxWidth, BoxHeight, ImColor(245, 66, 245), 1);
			}
			else
			{
				DrawNormalBox(Head2D.x - (BoxWidth / 2), Head2D.y, BoxWidth, BoxHeight, ImColor(255, 0, 25), 1);
			}
		}

		if (jtx::Distance)
		{
			std::string DistanceText = (std::to_string(int(Distance)) + "m");
			ImGui::CalcTextSize(DistanceText.c_str());
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(Bottom2D.x, Bottom2D.y + 10), ImColor(255, 255, 255), DistanceText.c_str(), 0, true);
		}

		if (jtx::Lines)
		{
			if (IsVisible(Pointers::Mesh))
			{
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Width / 2, Height), ImVec2(Bottom2D.x, Bottom2D.y), ImColor(245, 66, 245), 1.0f);
			}
			else
			{
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Width / 2, Height), ImVec2(Bottom2D.x, Bottom2D.y), ImColor(255, 0, 25), 1.0f);
			}
		}

		if (jtx::Corner)
		{
			if (IsVisible(Pointers::Mesh))
			{
				DrawCornerBox(Head2D.x - (BoxWidth / 2), Head2D.y, BoxWidth, BoxHeight, ImColor(245, 66, 245), 1);
			}
			else
			{
				DrawCornerBox(Head2D.x - (BoxWidth / 2), Head2D.y, BoxWidth, BoxHeight, ImColor(255, 0, 25), 1);
			}
		}

		if (IsVisible(Pointers::Mesh))
		{
			double dx = Head2D.x - Width / 2;

			double dy = Head2D.y - Height / 2;

			float dist = sqrtf(dx * dx + dy * dy);

			if (dist <= jtx::AimFOV && dist < Pointers::ClosestDistance)
			{
				Pointers::ClosestDistance = dist; Pointers::ClosestPawn = Pointers::Mesh; Pointers::ClosestActor = Pointers::PawnPrivate;

			}
		}
	}

	if (jtx::Aimbot)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			Aimbot_.RunAimbot(Pointers::ClosestPawn, Pointers::ClosestActor);
		}
	};
}