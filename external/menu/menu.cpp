#pragma once
#include <external/includes/includes.hpp>
#include <external/menu/menu.hpp>
#include <external/settings/settings.hpp>
#include <external/encryption/encryption.hpp>

void Menu::RenderMenu()
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		jtx::ShowMenu = !jtx::ShowMenu;
	}

	if (jtx::ShowMenu)
	{
		ImGui::SetNextWindowSize(ImVec2(328, 519));

		ImGui::Begin(_("Jeremy's Base"));

		ImGui::Text(_("Aimbot Settings:"));

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Checkbox(_("Aimbot"), &jtx::Aimbot);
		ImGui::Checkbox(_("Render FOV"), &jtx::RenderFOV);
		ImGui::SliderFloat(_("AimFOV"), &jtx::AimFOV, 1, 360);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text(_("ESP Settings:"));

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Checkbox(_("Box"), &jtx::Box);
		ImGui::Checkbox(_("Corner"), &jtx::Corner);
		ImGui::Checkbox(_("Lines"), &jtx::Lines);
		ImGui::Checkbox(_("Distance"), &jtx::Distance);


		ImGui::End();
	}
}