#pragma once

namespace hookOffsets
{
	uintptr_t baseGame = 0x0;
	uintptr_t KillTargetPlayer = 0x0;
}

namespace hookFunction
{
	inline void(__fastcall* KillTargetPlayer_o)(SDK::ABP_Entity_C*, SDK::ACharacter*); // first param is always class member
	void __fastcall KillTargetPlayer_Hook(SDK::ABP_Entity_C* This, SDK::ACharacter* TargetPlayer)
	{
		KillTargetPlayer_o(This, TargetPlayer);
		printf("entity name: %s\n", This->EntityIdentity.ToString());
		printf("APawn: %s\n", TargetPlayer->GetName());
		printf("----------------------------------------------------------\n");
	}
}