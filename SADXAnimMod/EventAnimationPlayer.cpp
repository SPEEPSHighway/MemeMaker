#include "SADXModLoader.h"
#include <math.h>
#include "Functions.h"
#include "NPCMode.h"
#include <string>

int index = 0;
int object = 0;
NJS_ACTION* anim;
NJS_ACTION** animplane; //Needed for compatibility with DC Chars
NJS_TEXLIST* tex;
NJS_ACTION npcEventAnim;
double animSpeed = 0.5;
long double leftX;
std::string objectName;
std::string objectTexName;

enum oList
{
	oList_player,
	oList_event,
	oList_plane,
	oList_froggy,
	oList_zero,
	oList_police,
	oList_chaos7,
	oList_pachacamac,
	oList_eggmobile,
	oList_tikalchao,
	oList_greeneme,
	oList_blueeme,
	oList_purpleeme,
	oList_redeme,
	oList_cyaneme,
	oList_whiteeme,
	oList_yelloweme,
	oList_blackeme,
	oList_policecar,
	oList_npchotel1 = oList_policecar + 21,
	oList_npchotel2,
	oList_npchotel3,
	oList_burgerf,
	oList_trainstaff,
	oList_burgerm,
};

int npcListStart = oList_policecar + 1;
int npcListEnd = oList_policecar + 20;
int objectListSize = oList_burgerm;

NJS_ACTION * froggyAnims[] = {
	&FroggyAction, &FroggyAction2, &FroggyAction3, (NJS_ACTION*)0x2CBB4DC
};

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
	0x6BF039, 0x6B9527, 0x62AC2C, 0x6CDE0E
};

static const std::string npcList[] = {
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
};

void displayEventAnimInfo() {
	DisplayDebugString(4, "Event Animation Player");
	DisplayDebugStringFormatted(5, "Object: %s", objectName.c_str());
	if (object != oList_player && object != oList_plane) {
		DisplayDebugStringFormatted(6, "Index: %d (Pointer: %04X)", index, anim);
	}
	else {
		DisplayDebugStringFormatted(6, "Index: %d", index);
	}
	DisplayDebugStringFormatted(7, "Animation Speed: %f", animSpeed);
	DisplayDebugString(8, "Press A to Play.");
	DisplayDebugString(9, "Press X+A to Loop.");
}

void doEventAnimFunction(int playerNo, AnalogData analogData) {	
	DataPointer(NJS_TEXLIST, stru_3375ED4, 0x3375ED4);
	int loop = 0;
	int size = 0;
	DataPointer(NJS_ACTION, stru_2DC028C, 0x2DC028C);
	NonStaticFunctionPointer(void, chg_frogshape, (char), 0x5F1F60);

	VoidFunc(TR2_RemoveSonic, 0x6DA2B0);
	VoidFunc(TR2_AddSonic, 0x6DA2C0);
	VoidFunc(TR1_RemoveSonic, 0x6DF420);
	VoidFunc(TR1_AddSonic, 0x6DF440);

	switch (object) {
	case oList_player: //player
		objectName = "Player Character (Regular)";
		switch (EntityData1Ptrs[playerNo]->CharID) {
		case Characters_Sonic:
			if (CurrentLevel == LevelIDs_PerfectChaos || GetModuleHandle(L"sadx-super-sonic")) size = 149;
			else size = 130;
			anim = SONIC_ACTIONS[index];
			if ((index >= 130 && index <= 133) || (index >= 138 && index <= 144)) {
				tex = &SUPERSONIC_TEXLIST;
				objectTexName = "SUPERSONIC";
			}
			else
			{
				objectTexName = "SONIC";
				tex = &SONIC_TEXLIST;
			}
			break;
		case Characters_Eggman:
			size = 25;
			anim = Eggman_AniList[index].Animation;
			objectTexName = "EGGMAN";
			tex = &EGGMAN_TEXLIST;
			break;
		case Characters_Tails:
			size = 114;
			anim = MILES_ACTIONS[index];
			tex = &MILES_TEXLIST;
			objectTexName = "MILES";
			break;
		case Characters_Knuckles:
			size = 90;
			anim = KNUCKLES_ACTIONS[index];
			tex = &KNUCKLES_TEXLIST;
			objectTexName = "KNUCKLES";
			break;
		case Characters_Tikal:
			size = 9;
			anim = Tikal_Animations[index].Animation;
			tex = &TIKAL_TEXLIST;
			objectTexName = "TIKAL";
			break;
		case Characters_Amy:
			size = 80;
			anim = AMY_ACTIONS[index];
			tex = &AMY_TEXLIST;
			objectTexName = "AMY";
			break;
		case Characters_Gamma:
			size = 78;
			anim = E102_ACTIONS[index];
			tex = &E102_TEXLIST;
			objectTexName = "E102";
			break;
		case Characters_Big:
			size = 90;
			anim = BIG_ACTIONS[index];
			tex = &BIG_TEXLIST; 
			objectTexName = "BIG";
			break;
		} 
		break;
	case oList_event:
		objectName = "Player Character (Event)";
		switch (EntityData1Ptrs[playerNo]->CharID) {
		case Characters_Sonic:
			size = LengthOfArray(sonicEventAnims);
			anim = (NJS_ACTION*)sonicEventAnims[index];
			tex = &SONIC_TEXLIST;
			objectTexName = "SONIC";
			break;
		case Characters_Eggman:
			size = LengthOfArray(eggmanEventAnims);
			anim = (NJS_ACTION*)eggmanEventAnims[index];
			tex = &EGGMAN_TEXLIST;
			objectTexName = "EGGMAN";
			break;
		case Characters_Tails:
			size = LengthOfArray(tailsEventAnims);
			anim = (NJS_ACTION*)tailsEventAnims[index];
			tex = &MILES_TEXLIST;
			objectTexName = "MILES";
			break;
		case Characters_Knuckles:
			size = LengthOfArray(knucklesEventAnims);
			anim = (NJS_ACTION*)knucklesEventAnims[index];
			tex = &KNUCKLES_TEXLIST;
			objectTexName = "KNUCKLES";
			break;
		case Characters_Tikal:
			size = LengthOfArray(tikalEventAnims);
			anim = (NJS_ACTION*)tikalEventAnims[index];
			tex = &TIKAL_TEXLIST;
			objectTexName = "TIKAL";
			break;
		case Characters_Amy:
			size = LengthOfArray(amyEventAnims);
			anim = (NJS_ACTION*)amyEventAnims[index];
			tex = &AMY_TEXLIST;
			objectTexName = "AMY";
			break;
		case Characters_Gamma:
			size = LengthOfArray(e102EventAnims);
			anim = (NJS_ACTION*)e102EventAnims[index];
			tex = &E102_TEXLIST;
			objectTexName = "E102";
			break;
		case Characters_Big:
			size = LengthOfArray(bigEventAnims);
			anim = (NJS_ACTION*)bigEventAnims[index];
			tex = &BIG_TEXLIST;
			objectTexName = "BIG";
			break;
		} break;
	case oList_plane:
		size = 6;
		objectName = "Planes";
		switch (index) {
		case 0: //Tornado 1
			TR1_RemoveSonic();
			tex = &EV_TR1_WITH_SONIC_TEXLIST;
			objectTexName = "EV_TR1_WITH_SONIC";
			animplane = (NJS_ACTION**)tornadoAnims[0];
			break;
		case 1: //Tornado 1 w/ Sonic
			TR1_AddSonic();
			tex = &EV_TR1_WITH_SONIC_TEXLIST;
			objectTexName = "EV_TR1_WITH_SONIC";
			animplane = (NJS_ACTION**)tornadoAnims[0];
			break;
		case 2: //Tornado 2 
			TR2_RemoveSonic();
			tex = (NJS_TEXLIST*)0x2BF513C;
			objectTexName = "EV_TR2BEFORE_WITH_SONIC";
			animplane = (NJS_ACTION**)tornadoAnims[1];
			break;
		case 3://Tornado 2 w/ Sonic
			TR2_AddSonic();
			tex = (NJS_TEXLIST*)0x2BF513C;
			objectTexName = "EV_TR2BEFORE_WITH_SONIC";
			animplane = (NJS_ACTION**)tornadoAnims[1];
			break;
		case 4: //Tornado 2 Transforming
			tex = &SHOOTING2_TEXLIST;
			objectTexName = "SHOOTING2";
			animplane = (NJS_ACTION**)tornadoAnims[2];
			break;
		case 5: //Tornado 2 Transformed
			tex = (NJS_TEXLIST*)0x32D6548;
			objectTexName = "EV_TR2CHANGE_WITH_SONIC";
			animplane = (NJS_ACTION**)tornadoAnims[3];
			break;
		}
		break;
	case oList_froggy:
		objectName = "FROGGY";
		size = LengthOfArray(froggyAnims);
		anim = froggyAnims[index];
		tex = (NJS_TEXLIST *)0x2FF2960;
		objectTexName = "SHAPE_FROG";
		break;
	case oList_zero:
		objectName = "ZERO";
		size = ZERO_Animations_Length;
		anim = ZERO_Animations[index].Animation;
		tex = &EGGROB_TEXLIST;
		objectTexName = "EGGROB";
		break; //ZERO
	case oList_police:
		objectName = "SS POLICE";
		size = LengthOfArray(ssPoliceAnims);
		objectTexName = "MP_10000_POLICE";
		anim = (NJS_ACTION*)ssPoliceAnims[index];
		tex = &stru_3375ED4;
		break; //SS Police
	case oList_chaos7:
		objectName = "PERFECT CHAOS";
		size = PerfectChaos_AnimList_Length;
		objectTexName = "CHAOS7_0";
		anim = PerfectChaos_AnimList[index].Animation;
		tex = &CHAOS7_0_TEXLIST;
		break;
	case oList_pachacamac:
		objectName = "PACHACAMAC";
		size = LengthOfArray(pachacamacAnims);
		objectTexName = "EV_K_PATYA";
		anim = (NJS_ACTION*)pachacamacAnims[index];
		tex = ADV03_TEXLISTS[3];
		break; //Pachacamac
	case oList_eggmobile:
		objectName = "EGG MOBILE";
		size = LengthOfArray(eggMobileAnims);
		objectTexName = "EV_EGGMOBLE0";
		anim = (NJS_ACTION*)eggMobileAnims[index];
		tex = &EV_EGGMOBLE0_TEXLIST;
		break; //Egg Mobile
	case oList_tikalchao:
		objectName = "TIKAL'S CHAO";
		size = LengthOfArray(tikalChaoAnims);
		objectTexName = "EV_ALIFE";
		anim = (NJS_ACTION*)tikalChaoAnims[index];
		tex = ADV03_TEXLISTS[1];
		break; //Tikal's Chao
	case oList_greeneme:
		objectName = "GREEN EMERALD";
		size = LengthOfArray(emeraldAnims);
		objectTexName = "M_EM_GREEN";
		anim = (NJS_ACTION*)emeraldAnims[index];
		tex = &M_EM_GREEN_TEXLIST;
		break; //Chaos Emeralds 
	case oList_blueeme:
		objectName = "EMERALD (BLUE)";
		size = LengthOfArray(emeraldAnims);
		objectTexName = "M_EM_BLUE";
		anim = (NJS_ACTION*)emeraldAnims[index];
		tex = &M_EM_BLUE_TEXLIST;
		break; //Chaos Emeralds  
	case oList_purpleeme:
		objectName = "EMERALD (PURPLE)";
		size = LengthOfArray(emeraldAnims);
		objectTexName = "M_EM_PURPLE";
		anim = (NJS_ACTION*)emeraldAnims[index];
		tex = &M_EM_PURPLE_TEXLIST;
		break; //Chaos Emeralds  
	case oList_redeme:
		objectName = "EMERALD (RED)";
		size = LengthOfArray(emeraldAnims);
		objectTexName = "M_EM_RED";
		anim = (NJS_ACTION*)emeraldAnims[index];
		tex = &M_EM_RED_TEXLIST;
		break; //Chaos Emeralds  
	case oList_cyaneme:
		objectName = "EMERALD (CYAN)";
		size = LengthOfArray(emeraldAnims);
		objectTexName = "M_EM_SKY";
		anim = (NJS_ACTION*)emeraldAnims[index];
		tex = &M_EM_SKY_TEXLIST;
		break; //Chaos Emeralds  
	case oList_whiteeme:
		objectName = "EMERALD (WHITE)";
		size = LengthOfArray(emeraldAnims);
		objectTexName = "M_EM_WHITE";
		anim = (NJS_ACTION*)emeraldAnims[index];
		tex = &M_EM_WHITE_TEXLIST;
		break; //Chaos Emeralds  
	case oList_yelloweme:
		objectName = "EMERALD (YELLOW)";
		size = LengthOfArray(emeraldAnims);
		objectTexName = "M_EM_YELLOW";
		anim = (NJS_ACTION*)emeraldAnims[index];
		tex = &M_EM_YELLOW_TEXLIST;
		break; //Chaos Emeralds  
	case oList_blackeme:
		objectName = "EMERALD (DRAINED)";
		size = LengthOfArray(emeraldAnims);
		objectTexName = "M_EM_BLACK";
		anim = (NJS_ACTION*)emeraldAnims[index];
		tex = &M_EM_BLACK_TEXLIST;
		break; //Chaos Emeralds  
	case oList_policecar:
		objectName = "SS POLICE CAR";
		size = 1;
		objectTexName = "SSPATCAR_BODY";
		anim = (NJS_ACTION*)&stru_2DC028C;
		tex = &SSPATCAR_BODY_TEXLIST;
		break; //SS Cop Car
	case oList_npchotel1:
		objectName = "NPC HOTEL FEMALE 1";
		size = 6;
		npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[20];
		npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 5];
		objectTexName = "SS_MIZUGI";
		anim = (NJS_ACTION*)&npcEventAnim;
		tex = ADV00_TEXLISTS[8];
		break;
	case oList_npchotel2:
		objectName = "NPC HOTEL FEMALE 2";
		size = 6;
		npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[21];
		npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 5];
		objectTexName = "SS_MIZUGI";
		anim = (NJS_ACTION*)&npcEventAnim;
		tex = ADV00_TEXLISTS[8];
		break; //SS NPCs
	case oList_npchotel3:
		objectName = "NPC HOTEL FEMALE 3";
		size = 6;
		npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[22];
		npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 5];
		objectTexName = "SS_MIZUGI";
		anim = (NJS_ACTION*)&npcEventAnim;
		tex = ADV00_TEXLISTS[8];
		break; //SS NPCs
	case oList_burgerf:
		objectName = "NPC BURGER SHOP FEMALE";
		size = 6;
		npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[23];
		npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 5];
		objectTexName = "SS_BURGER";
		anim = (NJS_ACTION*)&npcEventAnim;
		tex = ADV00_TEXLISTS[9];
		break; //SS NPCs
	case oList_trainstaff:
		objectName = "NPC TRAIN WORKER";
		size = 5;
		npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[24];
		npcEventAnim.motion = SS_PEOPLE_MOTIONS[index];
		objectTexName = "SS_EKIIN";
		anim = (NJS_ACTION*)&npcEventAnim;
		tex = ADV00_TEXLISTS[7];
		break; //SS NPCs
	case oList_burgerm:
		objectName = "NPC BURGER SHOP MALE";
		size = 5;
		npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[25];
		npcEventAnim.motion = SS_PEOPLE_MOTIONS[index];
		objectTexName = "SS_BURGER";
		anim = (NJS_ACTION*)&npcEventAnim;
		tex = ADV00_TEXLISTS[9];
		break; //SS NPCs
	default:
		//NPCs
		if (object >= npcListStart && object <= npcListEnd) {
			switch (getNPCMotion(object - npcListStart, 3)) {
			case 0: npcEventAnim.motion = SS_PEOPLE_MOTIONS[index]; break;
			case 1: npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 5]; break;
			case 2: npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 11];  break;
			case 3: npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 18]; break;
			case 4: npcEventAnim.motion = SS_PEOPLE_MOTIONS[index + 24]; break;
			}
			objectName = npcList[object - npcListStart];
			objectTexName = "SS_PEOPLE";
			npcEventAnim.object = MODEL_SS_PEOPLE_OBJECTS[object - npcListStart];
			anim = (NJS_ACTION*)&npcEventAnim;
			tex = ADV00_TEXLISTS[6];
		}
		if (object >= npcListStart && object <= (npcListStart + 3)) size = 5; //Animations
		if (object >= (npcListStart + 4) && object <= npcListEnd) size = 6; //Animations
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
		if (object < objectListSize) object++;
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
		if (ControllerPointers[playerNo]->HeldButtons & Buttons_X) loop = 1;
		else loop = 0;

		LoadPVM(objectTexName.c_str(), tex);
		EV_ClrAction(EV_GetPlayer(playerNo)); //Clear the animation queue

		if (anim != 0) {
			if (object == oList_plane) {
				EV_SetAction(EV_GetPlayer(playerNo), *animplane, tex, (float)animSpeed, loop, 0);
			}
			else EV_SetAction(EV_GetPlayer(playerNo), anim, tex, (float)animSpeed, loop, 0);			
		}
	}
}