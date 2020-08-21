#include "SADXModLoader.h"
#include <math.h>
#include "Animations.h"
#include "Functions.h"

bool npcMode[8];
int npcIndex[8];
int npcAnim[8];
double npcSpeed = 0.4;
bool npcAnimOn[8];

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
		npcAnim[player] = 98;
		break;
	case Buttons_Left:
		npcMode[player] = false;
		if (npcIndex[player] > 0) npcIndex[player] -= 1;
		else npcIndex[player] = 25;
		npcAnim[player] = 98;
		break;
	case Buttons_A:
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


void checkNPCMode() {
	if (isNPC(i)) {
		if (!npcAnimOn[i]) EV_ClrAction(EV_GetPlayer(i));
		npcAnimOn[i] = true;
		npcModeFunction(i);
		WriteData<1>((int*)0x413DA4, 0xFF); //Disable the HUD so the timer doesn't overlap the display.
	}
	else {
		if (npcAnimOn[i]) {
			EV_ClrAction(EV_GetPlayer(i));
			npcAnimOn[i] = false;
			CharObj2Ptrs[i]->PhysicsData.CollisionSize = backup[EntityData1Ptrs[i]->CharID].CollisionSize;
		}

	}
}


void npcModeFunction(int player) {
	NJS_ACTION nppc[8];
	NJS_TEXLIST* npcTex[8];

	if (npcIndex[player] == 23 || npcIndex[player] == 25) npcTex[player] = ADV00_TEXLISTS[9];
	if (npcIndex[player] == 24) npcTex[player] = ADV00_TEXLISTS[7];
	if (npcIndex[player] >= 20 && npcIndex[player] <= 22) npcTex[player] = ADV00_TEXLISTS[8];
	if (npcIndex[player] < 20) npcTex[player] = ADV00_TEXLISTS[6];

	switch (getNPCMotion(npcIndex[player], 0)) {
	case 1: CharObj2Ptrs[player]->PhysicsData.CollisionSize = 20; break;
	case 7: CharObj2Ptrs[player]->PhysicsData.CollisionSize = 22; break;
	case 14: CharObj2Ptrs[player]->PhysicsData.CollisionSize = 12; break;
	case 21: CharObj2Ptrs[player]->PhysicsData.CollisionSize = 17; break;
	case 25: CharObj2Ptrs[player]->PhysicsData.CollisionSize = 12; break;
	}

	switch (EntityData1Ptrs[player]->Action) {
	case 1:
		if (npcAnim[player] != 0) {
			npcSpeed = 0.4;
			EV_ClrAction(EV_GetPlayer(player));
			EV_SetMotion(EV_GetPlayer(player), MODEL_SS_PEOPLE_OBJECTS[npcIndex[player]], SS_PEOPLE_MOTIONS[getNPCMotion(npcIndex[player], 1)], npcTex[player], npcSpeed, 1, 4);
			npcAnim[player] = 0;
		}
		break;
	case 2:
		if (npcAnim[player] != 2) {
			npcSpeed = 1.2;
			EV_ClrAction(EV_GetPlayer(player));
			EV_SetMotion(EV_GetPlayer(player), MODEL_SS_PEOPLE_OBJECTS[npcIndex[player]], SS_PEOPLE_MOTIONS[getNPCMotion(npcIndex[player], 2)], npcTex[player], npcSpeed, 1, 4);
			npcAnim[player] = 2;
		}
		break;
	case 5: case 8:
		if (npcAnim[player] != 5) {
			npcSpeed = 1;
			EV_ClrAction(EV_GetPlayer(player));
			EV_SetMotion(EV_GetPlayer(player), MODEL_SS_PEOPLE_OBJECTS[npcIndex[player]], SS_PEOPLE_MOTIONS[getNPCMotion(npcIndex[player], 0)], npcTex[player], npcSpeed, 1, 4);
			npcAnim[player] = 5;
		}
		break;
	case 99:
		if (npcAnim[player] != 99) {
			npcSpeed = 99;
			EV_ClrAction(EV_GetPlayer(player));
			EV_SetMotion(EV_GetPlayer(player), MODEL_SS_PEOPLE_OBJECTS[npcIndex[player]], SS_PEOPLE_MOTIONS[getNPCMotion(npcIndex[player], 0)], npcTex[player], npcSpeed, 1, 4);
			npcAnim[player] = 99;
		}
	default:
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
		if (index >= 20 && index <= 23) return 1;
		break;
	case 1:
		if (index <= 3 || index >= 24) return 2;
		if (index >= 4 && index <= 7) return 6;
		if (index >= 8 && index <= 11) return 12;
		if ((index >= 12 && index <= 13) || (index >= 15 && index <= 16)) return 12;
		if (index == 14 || (index >= 17 && index <= 19)) return 18;
		if (index >= 20 && index <= 23) return 28;
		break;
	case 2:
		if (index <= 3 || index >= 24) return 0;
		if (index >= 4 && index <= 7) return 5;
		if (index >= 8 && index <= 11) return 11;
		if ((index >= 12 && index <= 13) || (index >= 15 && index <= 16)) return 11;
		if (index == 14 || (index >= 17 && index <= 19)) return 20;
		if (index >= 20 && index <= 23) return 24;
		break;
	}
}