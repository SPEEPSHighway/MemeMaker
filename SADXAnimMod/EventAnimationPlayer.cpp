#include "SADXModLoader.h"
#include <math.h>
#include "Functions.h"
#include "NPCMode.h"
#include <string>

int index = 0;
int object = 0;
NJS_ACTION npcEventAnim;
double animSpeed = 0.5;
long double leftX;

int bigEventAnims[] = {
		0x3C846C8, 0x3C846D0, 0x3C846D8, 0x3C846E0, 0x3C846E8, 0x3C846F0, // ??
		0x3C84A28, 0x3C84A30, 0x3C84A40, // Before Chaos 6
		0x3C84B9C, 0x3C84BBC, 0x3C84BCC, //After Tails past
		0x3C84BFC, 0x3C84C04, 0x3C84C0C, 0x3C84C14, 0x3C84C1C, 0x3C84C24, 0x3C84C2C, 0x3C84C34, //Intro
		0x3C84D64, 0x3C84D6C, 0x3C84D74, 0x3C84D7C, 0x3C84D84, 0x3C84D8C, //Big's Ending
		0x3C84FCC, 0x3C84FD4, 0x3C84FDC, //Froggy stolen by Gamma
		0x3C84FE4, 0x3C84FEC, 0x3C84FF4, //In Hot Shelter
		0x3C84FFC, 0x3C85004, 0x3C8500C, //After Past
};

int sonicEventAnims[] = {
	0x3C84758, 0x3C84760, 0x3C84768, 0x3C847C0, 0x3C847C8, 0x3C847D0, //Light Shoes
	0x3C847D8, 0x3C847E0, 0x3C847E8, //Crystal Ring
	0x3C84878, 0x3C84888, 0x3C84890, 0x3C84898, 0x3C848A0, 0x3C848A8, 0x3C848C8, 0x3C848D8, //After Egg Hornet
	0x3C84900, 0x3C84908, 0x3C84910,//After Casinopolis
	0x3C84920, 0x3C84928,		//^ ?
	0x3C84938,
	0x3C84948, 0x3C84958, 0x3C84960, 0x3C84968,
	0x3C84998, //Vs Knuckles
	0x3C84A10, 0x3C84A18, 0x3C84A20, //Ah yeah, this is happenin'!
	0x3C84A38, //Before Chaos 6
	0x3C84A78, 0x3C84A90, 0x3C84AA0, //After Gamma fight
	0x3C84B58, 0x3C84B60, 0x3C84B70, 0x3C84B78, 0x3C84B80, //Before Perfect Chaos
	0x3C84C78, //Sonic meets Amy
	0x3C84C80, 0x3C84C90, 0x3C84C98, //Before Twinkle Park
	0x3C84D94, 0x3C84DAC, //Rescuing Tails at EmC
	0x3C84DE4, 0x3C84DEC, 0x3C84E04, //Finding Knuckles in SS Story
	0x3C84E34, 0x3C84E3C, //Waking up after Casinopolis?
	0x3C84E7C, //Super Sonic flashback at the altar I think
	0x3C84EC4, 0x3C84ECC, 0x3C84ED4, 0x3C84EDC, //After Sky Chase 1
	0x3C84EE4, 0x3C84EEC, 0x3C84EF4, 0x3C84EFC, //Guess Eggman learned his lesson yep
	0x3C85074, 0x3C8507C, //Gamma about to nuke his ass
	0x3C85190, 0x3C851A0,  //Buggin scene
	0x3C851F0, 0x3C851F8, 0x3C85200, 0x3C85208, //Before Emerald Coast
	0x3C85218, 0x3C85220, 0x3C85228, //Sonic's Ending
	0x3C853B8, 0x3C853C0, //Before twinkle park again I think
	0x3C853C8, 0x3C853D0, 0x3C853D8, //Before Lost World
	0x3C853F0, //Amy's Intro
	0x3C8544C, //Tails Ending
};

int eggmanEventAnims[] = {
	0x8A8A9C
};

int tailsEventAnims[] = {
	0x3C84730, 0x3C84738, 0x3C84740, 0x3C84748, 0x3C84750, //Upgrades
	0x3C84880, 0x3C848B0, 0x3C848B8, 0x3C848C0, 0x3C848D0, //After Egg Hornet
	0x3C84918, 0x3C84930, //After Casinopolis
	0x3C84A48, 0x3C84A50, 0x3C84A58, 0x3C84A60, //After Sky Chase 1
	0x3C84B8C, 0x3C84B94, 0x3C84BA4, 0x3C84BAC, 0x3C84BB4, 0x3C84BC4, 0x3C84BD4, 0x3C84BDC, //After Past
	0x3C84D9C, 0x3C84DA4, 0x3C84DB4, //Emerald Coast
	0x3C84DBC, 0x3C84DC4, 0x3C84DCC, //Before Sand Hill
	0x3C84E0C, 0x3C84E14, 0x3C84E1C, 0x3C84E24, 0x3C84E2C, //Before Speed Highway
	0x3C84E44, 0x3C84E4C, //Waking up after Casinopolis?
	0x3C85308, 0x3C85310, 0x3C85318, 0x3C85320, 0x3C85328, 0x3C85330, //After Egg Carrier falls
	0x3C85444, 0x3C85454, 0x3C8545C, 0x3C85464, 0x3C8546C, 0x3C85474, 0x3C8547C, 0x3C85484, //Tails ending
	0x3C85494, 0x3C8549C, 0x3C854A4, 0x3C854AC, 0x3C854B4, //Flashback sequence
	0x3C854BC, 0x3C854C4, 0x3C854CC, 0x3C854D4, //After Tails vs. Knuckles
	0x3C84728
};

int knucklesEventAnims[] = {
	0x3C848E0, 0x3C848E8, 0x3C848F0, 0x3C848F8, //After Past 2
	0x3C84940, 0x3C84950, 0x3C84970, 0x3C84978, 0x3C84980, 0x3C84988, 0x3C84990, //Sonic vs knuckles
	0x3C84DD4, 0x3C84DDC, 0x3C84DF4, 0x3C84DFC, //Finding Knuckles in SS Story
	0x3C84F04, 0x3C84F0C, 0x3C84F14, 0x3C84F1C, 0x3C84F24, //Knuckles finding Eggman in SS Story
	0x3C84F2C, 0x3C84F34, 0x3C84F3C, 0x3C84F44, 0x3C84F4C, 0x3C84F54, //Intro
	0x3C85198, 0x3C851A8, 0x3C851B0, //Buggin scene
	0x3C851B8, 0x3C851C0, 0x3C851C8, //Also after past 2? idk one of these is his ending
	0x3C851D4, 0x3C851DC, 0x3C851E4 //Getting put into past 1 I think
};

int tikalEventAnims[] = {
	0x08F83F4, 0x2F1694C, 0x2EBF89C, 0x2F1694C, 0x2F1CBB4, 0x30F854C, 0x3100084, 0x3103DCC,
	0x2F180B4, 0x2EBACB4, 0x2C81AA4, 0x2FA7544, 0x2F33DFC, 0x2FAA58C, 0x2FAB794, 0x2D4B2DC,
	0x2D53284, 0x2F3E204, 0x2D5C1FC, 0x2F21B8C, 0x2D61174, 0x2ECD67C, 0x2EDBE6C, 0x2EE08F4,

};

int amyEventAnims[] = {
	0x3C846F8, 0x3C84700, 0x3C84708, 0x3C84710, 0x3C84718, 0x3C84720, //Upgrades
	0x3C84840, 0x3C84848, 0x3C84850, 0x3C84858, 0x3C84860, 0x3C84868, 0x3C84870, //Birdie
	0x3C84A68, 0x3C84A80, 0x3C84A98, //After gamma fight
	0x3C84AB8, 0x3C84AC0, 0x3C84AC8, 0x3C84AD0, 0x3C84AD8, 0x3C84AE0, 0x3C84AF0, 0x3C84AF8, 0x3C84B00, //Before ZERO
	0x3C84BE4, 0x3C84BEC, 0x3C84BF4, //?? various stages of holding the bird
	0x3C84C70, //Sonic meets Amy 
	0x3C84C88, 0x3C84CA0, 0x3C84CA8, //Before twinkle park
	0x3C8509C, 0x3C850A4, 0x3C850BC, 0x3C850C4, 0x3C850CC, 0x3C850DC, //Jail cell
	0x3C85260, 0x3C85268, 0x3C85270, 0x3C85278, 0x3C85280, 0x3C85288, 0x3C85290, 0x3C85298, //Amy's Ending
	0x3C852A0, 0x3C852A8, 0x3C852B0, 0x3C852B8, 0x3C852C0, 0x3C852C8, 0x3C852D0, 0x3C852D8, 0x3C852E0, 0x3C852E8, //After Egg Carrier I think
	0x3C852F0, 0x3C852F8, 0x3C85300, //Final Egg cutscene
	0x3C85340, 0x3C85348, 0x3C85350, 0x3C85358, 0x3C85360, 0x3C85368, 0x3C85370, 0x3C85378, 0x3C85380, 0x3C85388, 0x3C85390, //Intro 2
	0x3C85398, 0x3C853A0, 0x3C853A8, 0x3C853B0, //??
	0x3C853E0, 0x3C853E8, 0x3C853F8, 0x3C85400, 0x3C85408, 0x3C85410, 0x3C85418, 0x3C85420, 0x3C85428, 0x3C85430, 0x3C85438 //Intro 1
};

int e102EventAnims[] = {
	0x3C849A0, 0x3C849A8, 0x3C849B0, 0x3C849B8, 0x3C849C0, 0x3C849C8, 0x3C849D0,
	0x3C849D8, 0x3C849E0, 0x3C849E8, 0x3C849F0, 0x3C849F8, //Last scenes
	0x3C84A70, 0x3C84A88, 0x3C84AA8, 0x3C84AB0, //After Gamma fight
	0x3C85060, 0x3C85068, //Something I don't remember
	0x3C85084, 0x3C8508C, 0x3C85094, //After Sonic fight
	0x3C850AC, 0x3C850B4, 0x3C850D4, 0x3C850E4, //Jail cell
	0x3C85120, //gammas bros binned
	0x3C85180, 0x3C85188, //Something I don't remember, maybe after beta 1
	0x3C85338 //??
};

int miscEventAnims[] = {
	0x3C84A08, //???
};

int pachacamacAnims[] = {
	0x3C84818, 0x3C84820, 0x3C84828, 0x3C84CB0, 0x3C84CB8
};

int tikalChaoAnims[] = {
	0x3C84830, 0x3C84838,
	0x3C84C40, 0x3C84C48, 0x3C84C50, 0x3C84C58, 0x3C84C60, 0x3C84C68, //Tikal's Chao
	0x3C84EBC//Tikal's Chao
};

int eggMobileAnims[] = {
	0x2D41A34, 0x2E2C844, 0x2D41A34, 0x301C41C, 0x301DE94, 0x302015C, 0x3019E5C, 0x3018C44, 0x301B074, 0x3019E5C, 0x3021854 //Egg Mobile
};

int emeraldAnims[] = {
	0x2CBE0F4, 0x2D3E084, 0x2D3E18C, //Perfect Chaos Emeralds
	0x2E652A4, 0x2E657AC, 0x2E65E54, 0x2E66894, //&stru_2CBCF48, tails red emerald
	0x3016AB4
};

int ssPoliceAnims[] = {
	 0x337E734, 0x3380B7C, 0x3382FB4, 0x338510C, 0x3387984, 0x338954C, 0x338B934, 0x338E22C, 0x3392554, 0x33958DC, 0x3398454 // 0x339984C, 0x339A7D4, 	//Station Square Police
};

int tornadoAnims[] = {
	0x3261BBC, //&EV_TR1_WITH_SONIC_TEXLIST
	0x2C0DB54 //&stru_2BF513C
};


static const std::string objectList[] = {
	"Player Character (Regular)",
	"Player Character (Event)",
	"ZERO",
	"SS POLICE",
	"CHAOS 0",
	"CHAOS 6",
	"PERFECT CHAOS",
	"PACHACAMAC",
	"EGG MOBILE EGGMAN",
	"TIKAL'S CHAO",
	"CHAOS EMERALD (GREEN)",
	"CHAOS EMERALD (BLUE)",
	"CHAOS EMERALD (PURPLE)",
	"CHAOS EMERALD (RED)",
	"CHAOS EMERALD (SKY)",
	"CHAOS EMERALD (WHITE)",
	"CHAOS EMERALD (YELLOW)",
	"CHAOS EMERALD (BLACK)",
	"SS POLICE CAR",
	"NPC MALE 1",
	"NPC MALE 2",
	"NPC MALE 3",
	"NPC MALE 4",
	"NPC FEMALE 1",
	"NPC FEMALE 2",
	"NPC FEMALE 3",
	"NPC FEMALE 4",
	"NPC GIRL 1",
	"NPC GIRL 2",
	"NPC GIRL 3",
	"NPC GIRL 4",
	"NPC FAT 1",
	"NPC FAT 2",
	"NPC BOY 1",
	"NPC FAT 3",
	"NPC FAT 4",
	"NPC BOY 2",
	"NPC BOY 3",
	"NPC BOY 4",
	"NPC HOTEL FEMALE 1",
	"NPC HOTEL FEMALE 2",
	"NPC HOTEL FEMALE 3",
	"NPC BURGER SHOP FEMALE",
	"NPC TRAIN STATION WORKER",
	"NPC BURGER SHOP MALE"
};


void displayEventAnimInfo() {
	DisplayDebugString(4, "Event Animation Player");
	DisplayDebugStringFormatted(5, "Object: %s", objectList[object].c_str());
	DisplayDebugStringFormatted(6, "Index: %d", index);
	DisplayDebugStringFormatted(7, "Animation Speed: %f", animSpeed);
	DisplayDebugString(8, "Press A to Play.");
	DisplayDebugString(9, "Press X+A to Loop.");
}

void doEventAnimFunction(int playerNo, AnalogData analogData) {
	DataPointer(NJS_TEXLIST, stru_3375ED4, 0x3375ED4);
	int loop = 0;
	int size = 0;
	DataPointer(NJS_ACTION, stru_2DC028C, 0x2DC028C);

	switch (object) {
	case 0:
		switch (EntityData1Ptrs[playerNo]->CharID) {
		case 0:
			if (CurrentLevel == LevelIDs_PerfectChaos || GetModuleHandle(L"sadx-super-sonic")) size = 149;
			else size = 130;
			break;
		case 1: size = 25; break;
		case 2: size = 114; break;
		case 3: size = 90; break;
		case 4: size = 9; break;
		case 5: size = 80; break;
		case 6: size = 78; break;
		case 7: size = 90; break;
		} break;
	case 1:
		switch (EntityData1Ptrs[playerNo]->CharID) {
		case 0: size = LengthOfArray(sonicEventAnims); break;
		case 1: size = LengthOfArray(eggmanEventAnims); break;
		case 2: size = LengthOfArray(tailsEventAnims); break;
		case 3: size = LengthOfArray(knucklesEventAnims); break;
		case 4: size = LengthOfArray(tikalEventAnims); break;
		case 5: size = LengthOfArray(amyEventAnims); break;
		case 6: size = LengthOfArray(e102EventAnims); break;
		case 7: size = LengthOfArray(bigEventAnims); break;
		} break;
	case 2: size = ZERO_Animations_Length; break; //ZERO
	case 3: size = LengthOfArray(ssPoliceAnims); break; //SS Police
	case 4: size = 17; break; //Chaos 0
	case 5: size = Chaos6_Animations_Length; break;
	case 6: size = PerfectChaos_AnimList_Length; break;
	case 7: size = LengthOfArray(pachacamacAnims); break; //Pachacamac
	case 8: size = LengthOfArray(eggMobileAnims); break; //Egg Mobile
	case 9: size = LengthOfArray(tikalChaoAnims); break; //Tikal's Chao
	case 18: size = 1; break; //SS Cop Car
	default:
		if (object >= 10 && object <= 17) size = LengthOfArray(emeraldAnims); //Emeralds
		if ((object >= 19 && object <= 22) || (object >= 43 && object <= 44)) size = 5; //Animations
		if (object >= 23 && object <= 42) size = 6; //Animations
		break;
	}
	size--;

	if (index > size) index = size; //Stop range errors after moving to an object with fewer animations.

	if (ControllerPointers[playerNo]->HeldButtons & Buttons_X) {
		if (analogData.leftX > 30072.0)
			if (animSpeed < 4.9) animSpeed = animSpeed + 0.1;
		if (analogData.leftX < -30072.0) {
			if (animSpeed > 0.0) animSpeed = animSpeed - 0.1;
			if (animSpeed < 0.0) animSpeed = 0.0;
		}
	}

	switch (ControllerPointers[playerNo]->PressedButtons) {
	case Buttons_Right:
		if (index < size) index += 1; break;
	case Buttons_Left:
		if (index > 0) index -= 1; break;
	case Buttons_Up:
		if (object < 44) object++;
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
				anim = SONIC_ACTIONS[index];
				if((index >= 130 && index <= 133) || (index >= 138 && index <= 144)) tex = &SUPERSONIC_TEXLIST;
				else tex = &SONIC_TEXLIST;
				break;
			case 1:
				anim = Eggman_AniList[index].Animation;
				tex = &EGGMAN_TEXLIST;
				break;
			case 2:
				anim = MILES_ACTIONS[index];
				tex = &MILES_TEXLIST;
				break;
			case 3:
				anim = KNUCKLES_ACTIONS[index];
				tex = &KNUCKLES_TEXLIST;
				break;
			case 4:
				anim = Tikal_Animations[index].Animation;
				tex = &TIKAL_TEXLIST;
				break;
			case 5:
				anim = AMY_ACTIONS[index];
				tex = &AMY_TEXLIST;
				break;
			case 6:
				anim = E102_ACTIONS[index];
				tex = &E102_TEXLIST;
				break;
			case 7:
				anim = BIG_ACTIONS[index];
				tex = &BIG_TEXLIST;
				break;
			} break;
		case 1:
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
		case 2:
			LoadPVM("EGGROB", &EGGROB_TEXLIST);
			anim = ZERO_Animations[index].Animation;
			tex = &EGGROB_TEXLIST;
			break; //ZERO
		case 3:
			LoadPVM("MP_10000_POLICE", &stru_3375ED4);
			anim = (NJS_ACTION*)ssPoliceAnims[index];
			tex = &stru_3375ED4;
			break; //SS Police
		case 4:
			LoadPVM("CHAOS0", BOSSCHAOS0_TEXLISTS[0]);
			anim = BOSSCHAOS0_ACTIONS[index];
			tex = BOSSCHAOS0_TEXLISTS[0];
			break; //Chaos 0
		case 5:
			LoadPVM("CHAOS6", &CHAOS6_TEXLIST);
			anim = Chaos6_Animations[index].Animation;
			tex = &CHAOS6_TEXLIST;
			break; //Chaos 6
		case 6:
			LoadPVM("CHAOS7_0", &CHAOS7_0_TEXLIST);
			anim = PerfectChaos_AnimList[index].Animation;
			tex = &CHAOS7_0_TEXLIST;
			break; //Perfect Chaos
		case 7:
			LoadPVM("EV_K_PATYA", ADV03_TEXLISTS[3]);
			anim = (NJS_ACTION*)pachacamacAnims[index];
			tex = ADV03_TEXLISTS[3];
			break; //Pachacamac
		case 8:
			LoadPVM("EV_EGGMOBLE0", &EV_EGGMOBLE0_TEXLIST);
			anim = (NJS_ACTION*)eggMobileAnims[index];
			tex = &EV_EGGMOBLE0_TEXLIST;
			break; //Egg Mobile
		case 9:
			LoadPVM("EV_ALIFE", ADV03_TEXLISTS[1]);
			anim = (NJS_ACTION*)tikalChaoAnims[index];
			tex = ADV03_TEXLISTS[1];
			break; //Tikal's Chao
		case 10:
			LoadPVM("M_EM_GREEN", &M_EM_GREEN_TEXLIST);
			anim = (NJS_ACTION*)emeraldAnims[index];
			tex = &M_EM_GREEN_TEXLIST;
			break; //Chaos Emeralds 
		case 11:
			LoadPVM("M_EM_BLUE", &M_EM_BLUE_TEXLIST);
			anim = (NJS_ACTION*)emeraldAnims[index];
			tex = &M_EM_BLUE_TEXLIST;
			break; //Chaos Emeralds  
		case 12:
			LoadPVM("M_EM_PURPLE", &M_EM_PURPLE_TEXLIST);
			anim = (NJS_ACTION*)emeraldAnims[index];
			tex = &M_EM_PURPLE_TEXLIST;
			break; //Chaos Emeralds  
		case 13:
			LoadPVM("M_EM_RED", &M_EM_RED_TEXLIST);
			anim = (NJS_ACTION*)emeraldAnims[index];
			tex = &M_EM_RED_TEXLIST;
			break; //Chaos Emeralds  
		case 14:
			LoadPVM("M_EM_SKY", &M_EM_SKY_TEXLIST);
			anim = (NJS_ACTION*)emeraldAnims[index];
			tex = &M_EM_SKY_TEXLIST;
			break; //Chaos Emeralds  
		case 15:
			LoadPVM("M_EM_WHITE", &M_EM_WHITE_TEXLIST);
			anim = (NJS_ACTION*)emeraldAnims[index];
			tex = &M_EM_WHITE_TEXLIST;
			break; //Chaos Emeralds  
		case 16:
			LoadPVM("M_EM_YELLOW", &M_EM_YELLOW_TEXLIST);
			anim = (NJS_ACTION*)emeraldAnims[index];
			tex = &M_EM_YELLOW_TEXLIST;
			break; //Chaos Emeralds  
		case 17:
			LoadPVM("M_EM_BLACK", &M_EM_BLACK_TEXLIST);
			anim = (NJS_ACTION*)emeraldAnims[index];
			tex = &M_EM_BLACK_TEXLIST;
			break; //Chaos Emeralds  
		case 18:
			LoadPVM("SSPATCAR_BODY", &SSPATCAR_BODY_TEXLIST);
			anim = (NJS_ACTION*)&stru_2DC028C;
			tex = &SSPATCAR_BODY_TEXLIST;
			break;
		case 39:
			npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[20];
			npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 5];
			LoadPVM("SS_MIZUGI", ADV00_TEXLISTS[8]);
			anim = (NJS_ACTION*)&npcEventAnim;
			tex = ADV00_TEXLISTS[8];
			break;
		case 40:
			npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[21];
			npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 5];
			LoadPVM("SS_MIZUGI", ADV00_TEXLISTS[8]);
			anim = (NJS_ACTION*)&npcEventAnim;
			tex = ADV00_TEXLISTS[8];
			break; //SS NPCs
		case 41:
			npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[22];
			npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 5];
			LoadPVM("SS_MIZUGI", ADV00_TEXLISTS[8]);
			anim = (NJS_ACTION*)&npcEventAnim;
			tex = ADV00_TEXLISTS[8];
			break; //SS NPCs
		case 42:
			npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[23];
			npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 5];
			LoadPVM("SS_BURGER", ADV00_TEXLISTS[9]);
			anim = (NJS_ACTION*)&npcEventAnim;
			tex = ADV00_TEXLISTS[9];
			break; //SS NPCs
		case 43:
			npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[24];
			npcEventAnim.motion = SS_PEOPLE_MOTIONS[index];
			LoadPVM("SS_EKIIN", ADV00_TEXLISTS[7]);
			anim = (NJS_ACTION*)&npcEventAnim;
			tex = ADV00_TEXLISTS[7];
			break; //SS NPCs
		case 44:
			npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[25];
			npcEventAnim.motion = SS_PEOPLE_MOTIONS[index];
			LoadPVM("SS_BURGER", ADV00_TEXLISTS[9]);
			anim = (NJS_ACTION*)&npcEventAnim;
			tex = ADV00_TEXLISTS[9];
			break; //SS NPCs
		default:
			break;
		}

		if (object >= 19 && object <= 38) {
			switch (getNPCMotion(object - 19, 3)) {
			case 0: npcEventAnim.motion = SS_PEOPLE_MOTIONS[index]; break;
			case 1: npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 5]; break;
			case 2: npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 11];  break;
			case 3: npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 18]; break;
			case 4: npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 24]; break;
			}

			LoadPVM("SS_PEOPLE", ADV00_TEXLISTS[6]);
			npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[object - 19];
			anim = (NJS_ACTION*)&npcEventAnim;
			tex = ADV00_TEXLISTS[6];
		}

		EV_ClrAction(EV_GetPlayer(playerNo));
		if(anim != 0) EV_SetAction(EV_GetPlayer(playerNo), anim, tex, (float)animSpeed, loop, 0);	
		//LoadPVM("SONIC", &SONIC_TEXLIST);
		//if (anim != 0) EV_SetMotion(EV_GetPlayer(playerNo), SONIC_ACTIONS[1]->object, anim->motion, &SONIC_TEXLIST, (float)animSpeed, loop, 0);
	}
}