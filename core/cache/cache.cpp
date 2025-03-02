#pragma once
#include <external/includes/includes.hpp>
#include <core/Cache/Cache.hpp>
#include <external/communication/communication.hpp>
#include <updater/offsets/offsets.hpp>

auto Cache::Caching() -> void
{
	for (;;)
	{
		Pointers::UWorld = read<uintptr_t>(Base + Offsets::UWorld);
		Pointers::OwningGameInstance = read<uintptr_t>(Pointers::UWorld + Offsets::OwningGameInstance);
		Pointers::LocalPlayers = read<uintptr_t>(read<uintptr_t>(Pointers::OwningGameInstance + Offsets::LocalPlayers));
		Pointers::PlayerController = read<uintptr_t>(Pointers::LocalPlayers + Offsets::PlayerController);

		Pointers::LocalPawn = read<uintptr_t>(Pointers::PlayerController + Offsets::LocalPawn);
	    
		Pointers::RootComponent = read<uintptr_t>(Pointers::LocalPawn + Offsets::RootComponent);
		Pointers::RelativeLocation = read<Vector3>(Pointers::RootComponent + Offsets::RelativeLocation);
		Pointers::PlayerState = read<uintptr_t>(Pointers::LocalPawn + Offsets::PlayerState);
		Pointers::MyTeamID = read<int>(Pointers::PlayerState + Offsets::TeamIndex);

		Pointers::GameState = read<uintptr_t>(Pointers::UWorld + Offsets::GameState);
		Pointers::PlayerArray = read<uintptr_t>(Pointers::GameState + Offsets::PlayerArray);
		Pointers::PlayerCount = read<int>(Pointers::GameState + (Offsets::PlayerArray + sizeof(uintptr_t)));
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));
}
