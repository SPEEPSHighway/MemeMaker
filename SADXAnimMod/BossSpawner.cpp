#include "SADXModLoader.h"
#include "Functions.h"

bool evChaos0Spawned = false;


void displayBossSpawnerInfo() {
	DisplayDebugString(4, "Chaos 0 Spawner");
	if (evChaos0Spawned) DisplayDebugString(5, "Boss spawned. Press A to remove.");
	else DisplayDebugString(5, "Press A to spawn Chaos 0 at the current position.");
}

void doBossSpawner(int playerNo) {
	switch (ControllerPointers[playerNo]->PressedButtons) {
	case Buttons_A:

		LoadPVM("CHAOS0", BOSSCHAOS0_TEXLISTS[0]);
	//	LoadPVM("CHAOS_SURFACE", &CHAOS_SURFACE_TEXLIST);
		//LoadPVM("CHAOS_BRAINFRAME", &CHAOS_BRAINFRAME_TEXLIST);
	//	LoadPVM("CHAOS0_EFFECT", BOSSCHAOS0_TEXLISTS[1]);
		LoadPVM("EV_CHAOS0_MANJU", &EV_CHAOS0_MANJU_TEXLIST);
		
		if (!evChaos0Spawned) {
			WriteData<1>((void*)0x6EE99C, 0);
			CreateChaos0(
				EntityData1Ptrs[playerNo]->Position.x,
				EntityData1Ptrs[playerNo]->Position.y,
				EntityData1Ptrs[playerNo]->Position.z,
				EntityData1Ptrs[playerNo]->Rotation.x,
				(0x4000 - EntityData1Ptrs[playerNo]->Rotation.y) + 16000,
				EntityData1Ptrs[playerNo]->Rotation.z,
				2);
			//Stupid hack to make the animation play at normal speed without rewriting CreateChaos0().
			SetSclChaos0(1.0f, 1.0f, 1.0f);
			WriteData<1>((void*)0x6EE99C, 80);
		}
		else {
			DeleteChaos0();
		}
		evChaos0Spawned = !evChaos0Spawned;
		break;
		
	}
}