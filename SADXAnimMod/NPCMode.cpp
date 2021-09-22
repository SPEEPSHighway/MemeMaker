#include "SADXModLoader.h"
#include <math.h>
#include "Functions.h"

bool npcMode[8] = { false };
int npcIndex[8] = { 0 };
int npcAnim[8] = { 0 };
float npcSpeed = 0.4f;
int animType = 0;
bool npcAnimOn[8];
bool setNewAction = false;

PhysicsData backup[8] = {
	PhysicsArray[0],
	PhysicsArray[1],
	PhysicsArray[2],
	PhysicsArray[3],
	PhysicsArray[4],
	PhysicsArray[5],
	PhysicsArray[6],
	PhysicsArray[7],
};

void displayNPCModeInfo(int playerNo) {
	DisplayDebugString(4, "NPC Mode");
	DisplayDebugString(5, "Press A to toggle.");
	DisplayDebugStringFormatted(6, "Currently selected NPC: %d", npcIndex[playerNo]);
}

int getNPCIndex(int player) {
	return npcIndex[player];
}

bool isNPC(int player) {
	return npcMode[player];
}

void doNPCModeDisplay(int player) {

	switch (ControllerPointers[player]->PressedButtons) {
	case Buttons_Right:
		npcMode[player] = false;
		if (npcIndex[player] < 25) npcIndex[player] += 1;
		else npcIndex[player] = 0;
		npcAnim[player] = 0;
		break;
	case Buttons_Left:
		npcMode[player] = false;
		if (npcIndex[player] > 0) npcIndex[player] -= 1;
		else npcIndex[player] = 25;
		npcAnim[player] = 0;
		break;
	case Buttons_A:
		EV_ClrAction(EV_GetPlayer(player));
		if (!npcMode[player]) {
			LoadPVM("SS_PEOPLE", ADV00_TEXLISTS[6]);
			LoadPVM("SS_BURGER", ADV00_TEXLISTS[9]);
			LoadPVM("SS_EKIIN", ADV00_TEXLISTS[7]);
			LoadPVM("SS_MIZUGI", ADV00_TEXLISTS[8]);
			npcMode[player] = true;
		}
		else npcMode[player] = false;
		break;
	}
}

int getNPCMotion(int index, int anim) {
	switch (anim) {
	case 0:
		if (index <= 3 || index >= 24) return 1;
		if (index >= 4 && index <= 7) return 7;
		if (index >= 8 && index <= 11) return 14;
		if ((index >= 12 && index <= 13) || (index >= 15 && index <= 16)) return 21;
		if (index == 14 || (index >= 17 && index <= 19)) return 25;
		if (index >= 20 && index <= 23) return 7;
		break;
	case 1:
		if (index <= 3 || index >= 24) return 2;
		if (index >= 4 && index <= 7) return 6;
		if (index >= 8 && index <= 11) return 12;
		if ((index >= 12 && index <= 13) || (index >= 15 && index <= 16)) return 18;
		if (index == 14 || (index >= 17 && index <= 19)) return 28;
		if (index >= 20 && index <= 23) return 6;
		break;
	case 2:
		if (index <= 3 || index >= 24) return 0;
		if (index >= 4 && index <= 7) return 5;
		if (index >= 8 && index <= 11) return 11;
		if ((index >= 12 && index <= 13) || (index >= 15 && index <= 16)) return 20;
		if (index == 14 || (index >= 17 && index <= 19)) return 24;
		if (index >= 20 && index <= 23) return 5;
		break;
	case 3:
		if (index <= 3 || index >= 24) return 0;
		if (index >= 4 && index <= 7) return 1;
		if (index >= 8 && index <= 11) return 2;
		if ((index >= 12 && index <= 13) || (index >= 15 && index <= 16)) return 3;
		if (index == 14 || (index >= 17 && index <= 19)) return 4;
		if (index >= 20 && index <= 23) return 1;
		break;
	default:
		return 0;
		break;
	}
}

void npcModeFunction(int player) {
	NJS_TEXLIST* npcTex[8];

	if (npcIndex[player] == 23 || npcIndex[player] == 25) npcTex[player] = ADV00_TEXLISTS[9];
	if (npcIndex[player] == 24) npcTex[player] = ADV00_TEXLISTS[7];
	if (npcIndex[player] >= 20 && npcIndex[player] <= 22) npcTex[player] = ADV00_TEXLISTS[8];
	if (npcIndex[player] < 20) npcTex[player] = ADV00_TEXLISTS[6];

	//For some reason Sonic uses YOff rather than CollisionSize.
	if (GetCharacterID(player) == 0) {
		switch (getNPCMotion(npcIndex[player], 3)) {
		case 0: CharObj2Ptrs[player]->PhysicsData.YOff = 10; break;
		case 1: CharObj2Ptrs[player]->PhysicsData.YOff = 11; break;
		case 2: CharObj2Ptrs[player]->PhysicsData.YOff = 6; break;
		case 3: CharObj2Ptrs[player]->PhysicsData.YOff = 8; break;
		case 4: CharObj2Ptrs[player]->PhysicsData.YOff = 6; break;
		}
	}
	else {
		switch (getNPCMotion(npcIndex[player], 3)) {
		case 0: CharObj2Ptrs[player]->PhysicsData.CollisionSize = 20; break;
		case 1: CharObj2Ptrs[player]->PhysicsData.CollisionSize = 22; break;
		case 2: CharObj2Ptrs[player]->PhysicsData.CollisionSize = 12; break;
		case 3: CharObj2Ptrs[player]->PhysicsData.CollisionSize = 17; break;
		case 4: CharObj2Ptrs[player]->PhysicsData.CollisionSize = 12; break;
		}
	}
	switch (GetCharObj2(player)->AnimationThing.Index) {
	case 0: case 1: case 2:
		if (npcAnim[player] != 0) {
			npcSpeed = 0.4f;
			animType = 1;
			setNewAction = true;
			npcAnim[player] = 0;
		}
		break;
	case 9: case 10:
		if (npcAnim[player] != 9) {
			npcSpeed = 0.5f;
			animType = 2;
			setNewAction = true;
			npcAnim[player] = 9;
		}
		break;
	case 11:
		if (npcAnim[player] != 11) {
			npcSpeed = 1.0f;
			animType = 2;
			setNewAction = true;
			npcAnim[player] = 11;
		}
		break;
	case 12:
		if (npcAnim[player] != 12) {
			npcSpeed = 1.8f;
			animType = 2;
			setNewAction = true;
			npcAnim[player] = 12;
		}
		break;
	case 13:
		if (npcAnim[player] != 13) {
			npcSpeed = 2.4f;
			animType = 2;
			setNewAction = true;
			npcAnim[player] = 13;
		}
		break;
	case 14: case 18:
		if (npcAnim[player] != 14) {
			npcSpeed = 0.0f;
			animType = 2;
			setNewAction = true;
			npcAnim[player] = 14;
		}
		break;
	case 5: case 8:
		if (npcAnim[player] != 5) {
			npcSpeed = 1.0f;
			animType = 0;
			setNewAction = true;
			npcAnim[player] = 5;
		}
		break;
	default:
		break;
	}

	if (setNewAction) {
		EV_ClrAction(EV_GetPlayer(player));
		EV_SetMotion(EV_GetPlayer(player), MODEL_SS_PEOPLE_OBJECTS[npcIndex[player]], SS_PEOPLE_MOTIONS[getNPCMotion(npcIndex[player], animType)], npcTex[player], npcSpeed, 1, 10);
		setNewAction = false;
	}
}

void checkNPCMode(int player) {
	if (npcMode[player] && !DebugMode) {
		if (!npcAnimOn[player]) {
			EV_ClrAction(EV_GetPlayer(player));
			setNewAction = true;
		}
		npcAnimOn[player] = true;
		npcModeFunction(player);
		WriteData<1>((int*)0x413DA4, 0xFF); //Disable the HUD so the timer doesn't overlap the display.
	}
	else {
		if (npcAnimOn[player]) {
			npcMode[player] = false;
			EV_ClrAction(EV_GetPlayer(player));
			npcAnimOn[player] = false;
			CharObj2Ptrs[player]->PhysicsData.CollisionSize = backup[EntityData1Ptrs[player]->CharID].CollisionSize;
			CharObj2Ptrs[player]->PhysicsData.YOff = backup[EntityData1Ptrs[player]->CharID].YOff;
		}

	}
}