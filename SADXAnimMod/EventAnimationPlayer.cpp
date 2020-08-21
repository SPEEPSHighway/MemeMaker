#include "SADXModLoader.h"
#include <math.h>
#include "Animations.h"
#include "Functions.h"
#include "NPCMode.h"

int index = 0;
int object = 0;
NJS_ACTION npcEventAnim;
double animSpeed = 0.5;
long double leftX;


int getEventObject() {
	return object;
}
int getEventIndex() {
	return index;
}
double getEventSpeed() {
	return animSpeed;
}

void doEventAnimFunction(int playerNo) {
	DataPointer(NJS_TEXLIST, stru_3375ED4, 0x3375ED4);
	int loop = 0;
	int size = 0;
	DataPointer(NJS_ACTION, stru_2DC028C, 0x2DC028C);


	switch (object) {
	case 0:
		switch (EntityData1Ptrs[playerNo]->CharID) {
		case 0: size = 77; break;
		case 1: size = 0; break;
		case 2: size = 60; break;
		case 3: size = 34; break;
		case 4: size = 11; break;
		case 5: size = 84; break;
		case 6: size = 28; break;
		case 7: size = 34; break;
		} break;
	case 1: size = ZERO_Animations_Length; break; //ZERO
	case 2: size = 12; break; //SS Police
	case 3: size = 16; break; //Chaos 0
	case 4: size = Chaos6_Animations_Length; break;
	case 5: size = PerfectChaos_AnimList_Length - 1; break;
	case 6: size = 4; break; //Pachacamac
	case 7: size = 10; break; //Egg Mobile
	case 8: size = 11; break; //Tikal's Chao
	case 17: size = 0; break; //SS Cop Car
	default:
		if (object >= 9 && object <= 16) size = 6; //Emeralds
		if ((object >= 18 && object <= 21) || (object >= 42 && object <= 43)) size = 4; //Animations
		if (object >= 22 && object <= 37) size = 5; //Animations
		break;
	}

	LengthOfArray(sonicEventAnims);


	if (index > size) index = size; //Stop range errors after moving to an object with fewer animations.


	if (ControllerPointers[playerNo]->HeldButtons & Buttons_X) {
		if (leftX > 30072.0)
			if (animSpeed < 19.9) animSpeed = animSpeed + 0.1;
		if (leftX < -30072.0)
			if (animSpeed >= 0.1) animSpeed = animSpeed - 0.1;
	}

	switch (ControllerPointers[playerNo]->PressedButtons) {
	case Buttons_Right:
		if (index < size) index += 1; break;
	case Buttons_Left:
		if (index > 0) index -= 1; break;
	case Buttons_Up:
		if (object < 43) object++;
		else object = 0;
		index = 0;
		break;
	case Buttons_Down:
		if (object > 0) {
			object--;
			index = 0;
		}
		break;
	case Buttons_A:
		NJS_ACTION* anim = 0;
		NJS_TEXLIST* tex = 0;

		if (ControllerPointers[playerNo]->HeldButtons & Buttons_X) loop = 1;
		else loop = 0;

		switch (object) {
		case 0:
			switch (EntityData1Ptrs[playerNo]->CharID) {
			case 0:
				anim = (NJS_ACTION*)sonicEventAnims[index];
				tex = &SONIC_TEXLIST;
				break;
			case 1:
				anim = (NJS_ACTION*)eggmanEventAnims[index];
				tex = &EGGMAN_TEXLIST;
				break;
			case 2:
				anim = (NJS_ACTION*)tailsEventAnims[index];
				tex = &MILES_TEXLIST;
				break;
			case 3:
				anim = (NJS_ACTION*)knucklesEventAnims[index];
				tex = &KNUCKLES_TEXLIST;
				break;
			case 4:
				anim = (NJS_ACTION*)tikalEventAnims[index];
				tex = &TIKAL_TEXLIST;
				break;
			case 5:
				anim = (NJS_ACTION*)amyEventAnims[index];
				tex = &AMY_TEXLIST;
				break;
			case 6:
				anim = (NJS_ACTION*)e102EventAnims[index];
				tex = &E102_TEXLIST;
				break;
			case 7:
				anim = (NJS_ACTION*)bigEventAnims[index];
				tex = &BIG_TEXLIST;
				break;
			} break;
		case 1:
			LoadPVM("EGGROB", &EGGROB_TEXLIST);
			anim = ZERO_Animations[index].Animation;
			tex = &EGGROB_TEXLIST;
			break; //ZERO
		case 2:
			LoadPVM("MP_10000_POLICE", &stru_3375ED4);
			anim = (NJS_ACTION*)ssPoliceAnims[index];
			tex = &stru_3375ED4;
			break; //SS Police
		case 3:
			LoadPVM("CHAOS0", BOSSCHAOS0_TEXLISTS[0]);
			anim = BOSSCHAOS0_ACTIONS[index];
			tex = BOSSCHAOS0_TEXLISTS[0];
			break; //Chaos 0
		case 4:
			LoadPVM("CHAOS6", &CHAOS6_TEXLIST);
			anim = Chaos6_Animations[index].Animation;
			tex = &CHAOS6_TEXLIST;
			break; //Chaos 6
		case 5:
			LoadPVM("CHAOS7_0", &CHAOS7_0_TEXLIST);
			anim = PerfectChaos_AnimList[index].Animation;
			tex = &CHAOS7_0_TEXLIST;
			break; //Perfect Chaos
		case 6:
			LoadPVM("EV_K_PATYA", ADV03_TEXLISTS[3]);
			anim = (NJS_ACTION*)pachacamacAnims[index];
			tex = ADV03_TEXLISTS[3];
			break; //Pachacamac
		case 7:
			LoadPVM("EV_EGGMOBLE0", &EV_EGGMOBLE0_TEXLIST);
			anim = (NJS_ACTION*)eggMobileAnims[index];
			tex = &EV_EGGMOBLE0_TEXLIST;
			break; //Egg Mobile
		case 8:
			LoadPVM("EV_ALIFE", ADV03_TEXLISTS[1]);
			anim = (NJS_ACTION*)tikalChaoAnims[index];
			tex = ADV03_TEXLISTS[1];
			break; //Tikal's Chao
		case 9:
			LoadPVM("M_EM_GREEN", &M_EM_GREEN_TEXLIST);
			anim = (NJS_ACTION*)emeraldAnims[index];
			tex = &M_EM_GREEN_TEXLIST;
			break; //Chaos Emeralds 
		case 10:
			LoadPVM("M_EM_BLUE", &M_EM_BLUE_TEXLIST);
			anim = (NJS_ACTION*)emeraldAnims[index];
			tex = &M_EM_BLUE_TEXLIST;
			break; //Chaos Emeralds  
		case 11:
			LoadPVM("M_EM_PURPLE", &M_EM_PURPLE_TEXLIST);
			anim = (NJS_ACTION*)emeraldAnims[index];
			tex = &M_EM_PURPLE_TEXLIST;
			break; //Chaos Emeralds  
		case 12:
			LoadPVM("M_EM_RED", &M_EM_RED_TEXLIST);
			anim = (NJS_ACTION*)emeraldAnims[index];
			tex = &M_EM_RED_TEXLIST;
			break; //Chaos Emeralds  
		case 13:
			LoadPVM("M_EM_SKY", &M_EM_SKY_TEXLIST);
			anim = (NJS_ACTION*)emeraldAnims[index];
			tex = &M_EM_SKY_TEXLIST;
			break; //Chaos Emeralds  
		case 14:
			LoadPVM("M_EM_WHITE", &M_EM_WHITE_TEXLIST);
			anim = (NJS_ACTION*)emeraldAnims[index];
			tex = &M_EM_WHITE_TEXLIST;
			break; //Chaos Emeralds  
		case 15:
			LoadPVM("M_EM_YELLOW", &M_EM_YELLOW_TEXLIST);
			anim = (NJS_ACTION*)emeraldAnims[index];
			tex = &M_EM_YELLOW_TEXLIST;
			break; //Chaos Emeralds  
		case 16:
			LoadPVM("M_EM_BLACK", &M_EM_BLACK_TEXLIST);
			anim = (NJS_ACTION*)emeraldAnims[index];
			tex = &M_EM_BLACK_TEXLIST;
			break; //Chaos Emeralds  
		case 17:
			LoadPVM("SSPATCAR_BODY", &SSPATCAR_BODY_TEXLIST);
			anim = (NJS_ACTION*)&stru_2DC028C;
			tex = &SSPATCAR_BODY_TEXLIST;
			break;
		case 38:
			npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[20];
			npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 5];
			LoadPVM("SS_MIZUGI", ADV00_TEXLISTS[8]);
			anim = (NJS_ACTION*)&npcEventAnim;
			tex = ADV00_TEXLISTS[8];
			break;
		case 39:
			npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[21];
			npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 5];
			LoadPVM("SS_MIZUGI", ADV00_TEXLISTS[8]);
			anim = (NJS_ACTION*)&npcEventAnim;
			tex = ADV00_TEXLISTS[8];
			break; //SS NPCs
		case 40:
			npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[22];
			npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 5];
			LoadPVM("SS_MIZUGI", ADV00_TEXLISTS[8]);
			anim = (NJS_ACTION*)&npcEventAnim;
			tex = ADV00_TEXLISTS[8];
			break; //SS NPCs
		case 41:
			npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[23];
			npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 5];
			LoadPVM("SS_BURGER", ADV00_TEXLISTS[9]);
			anim = (NJS_ACTION*)&npcEventAnim;
			tex = ADV00_TEXLISTS[9];
			break; //SS NPCs
		case 42:
			npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[24];
			npcEventAnim.motion = SS_PEOPLE_MOTIONS[index];
			LoadPVM("SS_EKIIN", ADV00_TEXLISTS[7]);
			anim = (NJS_ACTION*)&npcEventAnim;
			tex = ADV00_TEXLISTS[7];
			break; //SS NPCs
		case 43:
			npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[25];
			npcEventAnim.motion = SS_PEOPLE_MOTIONS[index];
			LoadPVM("SS_BURGER", ADV00_TEXLISTS[9]);
			anim = (NJS_ACTION*)&npcEventAnim;
			tex = ADV00_TEXLISTS[9];
			break; //SS NPCs
		default:
			break;
		}

		if (object >= 18 && object <= 37) {
			switch (getNPCType(object - 18, 0)) {
			case 0: npcEventAnim.motion = SS_PEOPLE_MOTIONS[index]; break;
			case 1: npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 5]; break;
			case 2: npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 11];  break;
			case 3: npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 18]; break;
			case 4: npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 24]; break;
			}

			LoadPVM("SS_PEOPLE", ADV00_TEXLISTS[6]);
			npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[object - 18];
			anim = (NJS_ACTION*)&npcEventAnim;
			tex = ADV00_TEXLISTS[6];
		}

		EV_ClrAction(EV_GetPlayer(playerNo));
		EV_SetAction(EV_GetPlayer(playerNo), anim, tex, animSpeed, loop, 0);
	}
}