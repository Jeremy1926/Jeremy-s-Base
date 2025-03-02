#pragma once
#include <external/includes/includes.hpp>
#include <core/cache/cache.hpp>
#include <external/render/render.hpp>

auto main() -> int
{
	if (!Driver::Init())
	{
		printf(_("(!) failed to establish a connection with the kernel module.\n"));
		return false;
	}

	processID = Driver::FindProcess(_(L"FortniteClient-Win64-Shipping.exe"));

	bWindowHandle = Driver::GetProcessWND(processID);

	if (!Driver::CR3())
	{
		printf(_("(!) failed to resolve process cr3.\n"));
		return false;
	}

	Base = Driver::GetBase();

	std::thread([&]() { Cache_.Caching(); }).detach();

	if (!Render_.CreateOverlay())
	{
		printf(_("(!) failed to initialize dx11.\n"));
		return false;
	}
	else
		Render_.RenderThread();

	return 0;
}