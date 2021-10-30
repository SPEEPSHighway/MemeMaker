#include "SADXModLoader.h"
#include <math.h>
#include "Functions.h"
#include "NPCMode.h"
#include "EventAnimationPlayer.h"
#include "HeadEditor.h"
#include "FreeCamera.h"
#include "FreeMove.h"
#include "BossSpawner.h"
#include "ObjectSpawner.h"

#include <iostream>
#include <fstream>
#include <string>

extern "C"
{
	bool inEditMode = false;
	BYTE mode = 0;
	bool wasFreeCam;
	std::string subUpper = "";
	std::string subLower = "";
	std::string subVoice = "";
	std::string subtitleFilePath;
	char customSubtitle[128];
	const char* const customMessage[] = { customSubtitle, NULL };
	BYTE exitMode;
	bool showDisplay;
	AnalogData analogData = { 0 };
	int playerNo;
	std::string faceFilePath;
	double movementSpeed = 5.0;
	bool isEditorEnabled();
	void editorStart();
	void editorStop();
	void replaceAllDebugChecks();
	bool checkFontSize();
	int isObjectDebug(task* tp); 
	int tailsECgoalFix(task* tp);

	//Perfect Chaos: PerfectChaos_AnimList
	void doSpeedModifier();
	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions) {
		subtitleFilePath = std::string(path) + "\\text.txt";
		faceFilePath = std::string(path) + "\\face.txt";

		//Record pointer to player's head.
		WriteCall((void*)0x49587D, getHeadDataS); //Sonic
		WriteCall((void*)0x46146D, getHeadDataT); //Tails
		WriteCall((void*)0x477D4D, getHeadDataK); //Knuckles
		WriteCall((void*)0x7B407D, getHeadDataTi); //Tikal
		WriteCall((void*)0x48883D, getHeadDataA); //Amy
		WriteCall((void*)0x4909BD, getHeadDataB); //Big
		replaceAllDebugChecks();

		//Stop Pressing B resetting position when exiting the object editor
		WriteJump((void*)0x49AA1E, (void*)0x49AA2C);
		WriteJump((void*)0x48AC63, (void*)0X48AC71);
		WriteJump((void*)0x47A7E8, (void*)0x47A7F4);

		//Load Tails' textures for debug Emerald Coast goal
		WriteCall((void*)0x4F9FBA, tailsECgoalFix);

		initFaceTableBackup();
	}

	int tailsECgoalFix(task *tp) {
		if (!VerifyTexList(&MILES_TEXLIST)) {
			LoadPVM("MILES", &MILES_TEXLIST);
		}
		return ClipSetObject((ObjectMaster*)tp);
	}

	void replaceAllDebugChecks() {
		WriteCall((void*)0x40195D, isObjectDebug);
		WriteCall((void*)0x40C0EE, isObjectDebug);
		WriteCall((void*)0x46AE6A, isObjectDebug);
		WriteCall((void*)0x46B13A, isObjectDebug);
		WriteCall((void*)0x46B1D1, isObjectDebug);
		WriteCall((void*)0x49D347, isObjectDebug);
		WriteCall((void*)0x49D397, isObjectDebug);
		WriteCall((void*)0x49D416, isObjectDebug);
		WriteCall((void*)0x49D467, isObjectDebug);
		WriteCall((void*)0x49D87D, isObjectDebug);
		WriteCall((void*)0x49D9F1, isObjectDebug);
		WriteCall((void*)0x49DBA7, isObjectDebug);
		WriteCall((void*)0x49DD03, isObjectDebug);
		WriteCall((void*)0x49DE36, isObjectDebug);
		WriteCall((void*)0x49DEB6, isObjectDebug);
		WriteCall((void*)0x49DF36, isObjectDebug);
		WriteCall((void*)0x49DFB6, isObjectDebug);
		WriteCall((void*)0x49E036, isObjectDebug);
		WriteCall((void*)0x49E1AD, isObjectDebug);
		WriteCall((void*)0x4A2583, isObjectDebug);
		WriteCall((void*)0x4A2B07, isObjectDebug);
		WriteCall((void*)0x4A670D, isObjectDebug);
		WriteCall((void*)0x4A7D2B, isObjectDebug);
		WriteCall((void*)0x4AB7CB, isObjectDebug);
		WriteCall((void*)0x4AE9B8, isObjectDebug);
		WriteCall((void*)0x4AEB78, isObjectDebug);
		WriteCall((void*)0x4AFFC6, isObjectDebug);
		WriteCall((void*)0x4B0118, isObjectDebug);
		WriteCall((void*)0x4B02D8, isObjectDebug);
		WriteCall((void*)0x4B2A68, isObjectDebug);
		WriteCall((void*)0x4B4980, isObjectDebug);
		WriteCall((void*)0x4B8D2B, isObjectDebug);
		WriteCall((void*)0x4B8DCE, isObjectDebug);
		WriteCall((void*)0x4BF654, isObjectDebug);
		WriteCall((void*)0x4C082C, isObjectDebug);
		WriteCall((void*)0x4C8FBB, isObjectDebug);
		WriteCall((void*)0x4C90E5, isObjectDebug);
		WriteCall((void*)0x4CA2B5, isObjectDebug);
		WriteCall((void*)0x4CA3DE, isObjectDebug);
		WriteCall((void*)0x4CA53A, isObjectDebug);
		WriteCall((void*)0x4CA66A, isObjectDebug);
		WriteCall((void*)0x4CA7DA, isObjectDebug);
		WriteCall((void*)0x4CA90A, isObjectDebug);
		WriteCall((void*)0x4CB9D8, isObjectDebug);
		WriteCall((void*)0x4CBACB, isObjectDebug);
		WriteCall((void*)0x4CBC7D, isObjectDebug);
		WriteCall((void*)0x4D451C, isObjectDebug);
		WriteCall((void*)0x4D4ABE, isObjectDebug);
		WriteCall((void*)0x4DBD20, isObjectDebug);
		WriteCall((void*)0x4DBFC1, isObjectDebug);
		WriteCall((void*)0x4DC42D, isObjectDebug);
		WriteCall((void*)0x4DF438, isObjectDebug);
		WriteCall((void*)0x4DF44C, isObjectDebug);
		WriteCall((void*)0x4DF803, isObjectDebug);
		WriteCall((void*)0x4DF856, isObjectDebug);
		WriteCall((void*)0x4DFF7C, isObjectDebug);
		WriteCall((void*)0x4DFF92, isObjectDebug);
		WriteCall((void*)0x4E0017, isObjectDebug);
		WriteCall((void*)0x4E06C1, isObjectDebug);
		WriteCall((void*)0x4E13A5, isObjectDebug);
		WriteCall((void*)0x4E1410, isObjectDebug);
		WriteCall((void*)0x4E177F, isObjectDebug);
		WriteCall((void*)0x4E1AF4, isObjectDebug);
		WriteCall((void*)0x4E1B60, isObjectDebug);
		WriteCall((void*)0x4E1BA0, isObjectDebug);
		WriteCall((void*)0x4E1BE0, isObjectDebug);
		WriteCall((void*)0x4E1C20, isObjectDebug);
		WriteCall((void*)0x4E1C80, isObjectDebug);
		WriteCall((void*)0x4E1CC0, isObjectDebug);
		WriteCall((void*)0x4E1FD4, isObjectDebug);
		WriteCall((void*)0x4E2050, isObjectDebug);
		WriteCall((void*)0x4E2090, isObjectDebug);
		WriteCall((void*)0x4E262E, isObjectDebug);
		WriteCall((void*)0x4E28B9, isObjectDebug);
		WriteCall((void*)0x4E2997, isObjectDebug);
		WriteCall((void*)0x4E2ACC, isObjectDebug);
		WriteCall((void*)0x4E2BE2, isObjectDebug);
		WriteCall((void*)0x4E2BF8, isObjectDebug);
		WriteCall((void*)0x4E2C50, isObjectDebug);
		WriteCall((void*)0x4E2C90, isObjectDebug);
		WriteCall((void*)0x4E2D35, isObjectDebug);
		WriteCall((void*)0x4E33F2, isObjectDebug);
		WriteCall((void*)0x4E3408, isObjectDebug);
		WriteCall((void*)0x4E3D5A, isObjectDebug);
		WriteCall((void*)0x4E4030, isObjectDebug);
		WriteCall((void*)0x4E4D21, isObjectDebug);
		WriteCall((void*)0x4E527F, isObjectDebug);
		WriteCall((void*)0x4E557C, isObjectDebug);
		WriteCall((void*)0x4E5701, isObjectDebug);
		WriteCall((void*)0x4E5853, isObjectDebug);
		WriteCall((void*)0x4E7F5F, isObjectDebug);
		WriteCall((void*)0x4EA289, isObjectDebug);
		WriteCall((void*)0x4EB2CA, isObjectDebug);
		WriteCall((void*)0x4EB730, isObjectDebug);
		WriteCall((void*)0x4EBE04, isObjectDebug);
		WriteCall((void*)0x4EBED4, isObjectDebug);
		WriteCall((void*)0x4EBFEF, isObjectDebug);
		WriteCall((void*)0x4EC06E, isObjectDebug);
		WriteCall((void*)0x4ED028, isObjectDebug);
		WriteCall((void*)0x4ED03C, isObjectDebug);
		WriteCall((void*)0x4ED1DD, isObjectDebug);
		WriteCall((void*)0x4ED20A, isObjectDebug);
		WriteCall((void*)0x4EDA6D, isObjectDebug);
		WriteCall((void*)0x4EDE11, isObjectDebug);
		WriteCall((void*)0x4EDF29, isObjectDebug);
		WriteCall((void*)0x4EE77E, isObjectDebug);
		WriteCall((void*)0x4EEA15, isObjectDebug);
		WriteCall((void*)0x4EEA66, isObjectDebug);
		WriteCall((void*)0x4EF1E8, isObjectDebug);
		WriteCall((void*)0x4EF2B3, isObjectDebug);
		WriteCall((void*)0x4EF6C5, isObjectDebug);
		WriteCall((void*)0x4EFC4C, isObjectDebug);
		WriteCall((void*)0x4F0527, isObjectDebug);
		WriteCall((void*)0x4F0572, isObjectDebug);
		WriteCall((void*)0x4F0DF8, isObjectDebug);
		WriteCall((void*)0x4F1115, isObjectDebug);
		WriteCall((void*)0x4F1C02, isObjectDebug);
		WriteCall((void*)0x4F1C42, isObjectDebug);
		WriteCall((void*)0x4F26EA, isObjectDebug);
		WriteCall((void*)0x4F2D82, isObjectDebug);
		WriteCall((void*)0x4F2DC2, isObjectDebug);
		WriteCall((void*)0x4F385A, isObjectDebug);
		WriteCall((void*)0x4F3F04, isObjectDebug);
		WriteCall((void*)0x4F3FD2, isObjectDebug);
		WriteCall((void*)0x4F401A, isObjectDebug);
		WriteCall((void*)0x4F4728, isObjectDebug);
		WriteCall((void*)0x4F4788, isObjectDebug);
		WriteCall((void*)0x4F483F, isObjectDebug);
		WriteCall((void*)0x4F4C3E, isObjectDebug);
		WriteCall((void*)0x4F4D9E, isObjectDebug);
		WriteCall((void*)0x4F5737, isObjectDebug);
		WriteCall((void*)0x4F57C8, isObjectDebug);
		WriteCall((void*)0x4F9FFE, isObjectDebug);
		WriteCall((void*)0x4FA012, isObjectDebug);
		WriteCall((void*)0x4FA344, isObjectDebug);
		WriteCall((void*)0x4FA358, isObjectDebug);
		WriteCall((void*)0x4FA4C1, isObjectDebug);
		WriteCall((void*)0x4FA856, isObjectDebug);
		WriteCall((void*)0x4FACF4, isObjectDebug);
		WriteCall((void*)0x4FAD31, isObjectDebug);
		WriteCall((void*)0x4FADE9, isObjectDebug);
		WriteCall((void*)0x4FAE75, isObjectDebug);
		WriteCall((void*)0x4FAEBB, isObjectDebug);
		WriteCall((void*)0x4FB027, isObjectDebug);
		WriteCall((void*)0x4FB061, isObjectDebug);
		WriteCall((void*)0x4FB137, isObjectDebug);
		WriteCall((void*)0x4FB166, isObjectDebug);
		WriteCall((void*)0x4FB1DE, isObjectDebug);
		WriteCall((void*)0x4FB5BF, isObjectDebug);
		WriteCall((void*)0x4FB607, isObjectDebug);
		WriteCall((void*)0x4FB7A0, isObjectDebug);
		WriteCall((void*)0x4FB810, isObjectDebug);
		WriteCall((void*)0x4FBA1C, isObjectDebug);
	// 	WriteCall((void*)0x4FBBA3, isObjectDebug); //Whale in Emerald Coast. Crashes for some reason so I've disabled it.
	//	WriteCall((void*)0x4FBDD3, isObjectDebug);
		WriteCall((void*)0x4FC4FC, isObjectDebug);
		WriteCall((void*)0x4FC58C, isObjectDebug);
		WriteCall((void*)0x4FC9F7, isObjectDebug);
		WriteCall((void*)0x4FD0BF, isObjectDebug);
		WriteCall((void*)0x4FD7CF, isObjectDebug);
		WriteCall((void*)0x4FDA34, isObjectDebug);
		WriteCall((void*)0x4FDA53, isObjectDebug);
		WriteCall((void*)0x4FDAB8, isObjectDebug);
		WriteCall((void*)0x4FE022, isObjectDebug);
		WriteCall((void*)0x4FE808, isObjectDebug);
		WriteCall((void*)0x4FED53, isObjectDebug);
		WriteCall((void*)0x4FF0A3, isObjectDebug);
		WriteCall((void*)0x4FF4DE, isObjectDebug);
		WriteCall((void*)0x4FF622, isObjectDebug);
		WriteCall((void*)0x4FFB91, isObjectDebug);
		WriteCall((void*)0x4FFC61, isObjectDebug);
		WriteCall((void*)0x4FFDD0, isObjectDebug);
		WriteCall((void*)0x4FFE10, isObjectDebug);
		WriteCall((void*)0x4FFE50, isObjectDebug);
		WriteCall((void*)0x4FFE90, isObjectDebug);
		WriteCall((void*)0x500249, isObjectDebug);
		WriteCall((void*)0x50026E, isObjectDebug);
		WriteCall((void*)0x500796, isObjectDebug);
		WriteCall((void*)0x500BBA, isObjectDebug);
		WriteCall((void*)0x500BFC, isObjectDebug);
		WriteCall((void*)0x500EF1, isObjectDebug);
		WriteCall((void*)0x500F1B, isObjectDebug);
		WriteCall((void*)0x500F81, isObjectDebug);
		WriteCall((void*)0x500FB3, isObjectDebug);
		WriteCall((void*)0x501021, isObjectDebug);
		WriteCall((void*)0x501053, isObjectDebug);
		WriteCall((void*)0x5010C1, isObjectDebug);
		WriteCall((void*)0x5010F3, isObjectDebug);
		WriteCall((void*)0x501C45, isObjectDebug);
		WriteCall((void*)0x5020E7, isObjectDebug);
		WriteCall((void*)0x502EAD, isObjectDebug);
		WriteCall((void*)0x51C897, isObjectDebug);
		WriteCall((void*)0x51C9AE, isObjectDebug);
		WriteCall((void*)0x51CB77, isObjectDebug);
		WriteCall((void*)0x51CC3E, isObjectDebug);
		WriteCall((void*)0x51CFCC, isObjectDebug);
		WriteCall((void*)0x51D00C, isObjectDebug);
		WriteCall((void*)0x51D43E, isObjectDebug);
		WriteCall((void*)0x51D6B0, isObjectDebug);
		WriteCall((void*)0x51D77C, isObjectDebug);
		WriteCall((void*)0x51D7B1, isObjectDebug);
		WriteCall((void*)0x51D877, isObjectDebug);
		WriteCall((void*)0x51D899, isObjectDebug);
		WriteCall((void*)0x51D965, isObjectDebug);
		WriteCall((void*)0x51E18A, isObjectDebug);
		WriteCall((void*)0x51E33C, isObjectDebug);
		WriteCall((void*)0x51E4C8, isObjectDebug);
		WriteCall((void*)0x51E4FD, isObjectDebug);
		WriteCall((void*)0x51E6E7, isObjectDebug);
		WriteCall((void*)0x51E763, isObjectDebug);
		WriteCall((void*)0x51E93F, isObjectDebug);
		WriteCall((void*)0x51E9A9, isObjectDebug);
		WriteCall((void*)0x51EAAC, isObjectDebug);
		WriteCall((void*)0x51EB5A, isObjectDebug);
		WriteCall((void*)0x51EF4A, isObjectDebug);
		WriteCall((void*)0x51F56A, isObjectDebug);
		WriteCall((void*)0x51F6DA, isObjectDebug);
		WriteCall((void*)0x51FA17, isObjectDebug);
		WriteCall((void*)0x51FBE7, isObjectDebug);
		WriteCall((void*)0x51FE8A, isObjectDebug);
		WriteCall((void*)0x51FFCC, isObjectDebug);
		WriteCall((void*)0x520098, isObjectDebug);
		WriteCall((void*)0x520272, isObjectDebug);
		WriteCall((void*)0x521B3E, isObjectDebug);
		WriteCall((void*)0x5228AA, isObjectDebug);
		WriteCall((void*)0x522A99, isObjectDebug);
		WriteCall((void*)0x522ACE, isObjectDebug);
		WriteCall((void*)0x522E88, isObjectDebug);
		WriteCall((void*)0x522F6A, isObjectDebug);
		WriteCall((void*)0x523173, isObjectDebug);
		WriteCall((void*)0x52319E, isObjectDebug);
		WriteCall((void*)0x5231A4, isObjectDebug);
		WriteCall((void*)0x523488, isObjectDebug);
		WriteCall((void*)0x523668, isObjectDebug);
		WriteCall((void*)0x523EFE, isObjectDebug);
		WriteCall((void*)0x52416D, isObjectDebug);
		WriteCall((void*)0x5242D0, isObjectDebug);
		WriteCall((void*)0x5245DE, isObjectDebug);
		WriteCall((void*)0x52468B, isObjectDebug);
		WriteCall((void*)0x524727, isObjectDebug);
		WriteCall((void*)0x524780, isObjectDebug);
		WriteCall((void*)0x5247AD, isObjectDebug);
		WriteCall((void*)0x52488A, isObjectDebug);
		WriteCall((void*)0x5248DF, isObjectDebug);
		WriteCall((void*)0x524E51, isObjectDebug);
		WriteCall((void*)0x524E90, isObjectDebug);
		WriteCall((void*)0x52507C, isObjectDebug);
		WriteCall((void*)0x526175, isObjectDebug);
		WriteCall((void*)0x526666, isObjectDebug);
		WriteCall((void*)0x52679D, isObjectDebug);
		WriteCall((void*)0x5267D2, isObjectDebug);
		WriteCall((void*)0x5268FF, isObjectDebug);
		WriteCall((void*)0x52694A, isObjectDebug);
		WriteCall((void*)0x526A94, isObjectDebug);
		WriteCall((void*)0x526ACF, isObjectDebug);
		WriteCall((void*)0x526B83, isObjectDebug);
		WriteCall((void*)0x528A06, isObjectDebug);
		WriteCall((void*)0x528E37, isObjectDebug);
		WriteCall((void*)0x52B13C, isObjectDebug);
		WriteCall((void*)0x52B1F6, isObjectDebug);
		WriteCall((void*)0x52B60D, isObjectDebug);
		WriteCall((void*)0x52B738, isObjectDebug);
		WriteCall((void*)0x52BA00, isObjectDebug);
		WriteCall((void*)0x52C728, isObjectDebug);
		WriteCall((void*)0x52CB82, isObjectDebug);
		WriteCall((void*)0x52D524, isObjectDebug);
		WriteCall((void*)0x52D72C, isObjectDebug);
		WriteCall((void*)0x5323B1, isObjectDebug);
		WriteCall((void*)0x53244B, isObjectDebug);
		WriteCall((void*)0x53365A, isObjectDebug);
		WriteCall((void*)0x53388A, isObjectDebug);
		WriteCall((void*)0x5340BA, isObjectDebug);
		WriteCall((void*)0x5341EA, isObjectDebug);
		WriteCall((void*)0x53433A, isObjectDebug);
		WriteCall((void*)0x53496A, isObjectDebug);
		WriteCall((void*)0x534C3A, isObjectDebug);
		WriteCall((void*)0x53508A, isObjectDebug);
		WriteCall((void*)0x535474, isObjectDebug);
		WriteCall((void*)0x535526, isObjectDebug);
		WriteCall((void*)0x535A46, isObjectDebug);
		WriteCall((void*)0x535ACD, isObjectDebug);
		WriteCall((void*)0x535F56, isObjectDebug);
		WriteCall((void*)0x53631E, isObjectDebug);
		WriteCall((void*)0x536A9E, isObjectDebug);
		WriteCall((void*)0x536CCE, isObjectDebug);
		WriteCall((void*)0x536EEB, isObjectDebug);
		WriteCall((void*)0x53760B, isObjectDebug);
		WriteCall((void*)0x53781B, isObjectDebug);
		WriteCall((void*)0x537FCB, isObjectDebug);
		WriteCall((void*)0x538627, isObjectDebug);
		WriteCall((void*)0x538747, isObjectDebug);
		WriteCall((void*)0x538AD8, isObjectDebug);
		WriteCall((void*)0x538FDA, isObjectDebug);
		WriteCall((void*)0x539115, isObjectDebug);
		WriteCall((void*)0x53950C, isObjectDebug);
		WriteCall((void*)0x539BCC, isObjectDebug);
		WriteCall((void*)0x53A6C7, isObjectDebug);
		WriteCall((void*)0x53AFCC, isObjectDebug);
		WriteCall((void*)0x53B0ED, isObjectDebug);
		WriteCall((void*)0x53BBAC, isObjectDebug);
		WriteCall((void*)0x53C3FC, isObjectDebug);
		WriteCall((void*)0x53C67C, isObjectDebug);
		WriteCall((void*)0x53CA80, isObjectDebug);
		WriteCall((void*)0x53CF7C, isObjectDebug);
		WriteCall((void*)0x53D1D0, isObjectDebug);
		WriteCall((void*)0x53D71C, isObjectDebug);
		WriteCall((void*)0x53DEA1, isObjectDebug);
		WriteCall((void*)0x53E45C, isObjectDebug);
		WriteCall((void*)0x53E9B0, isObjectDebug);
		WriteCall((void*)0x53EC4C, isObjectDebug);
		WriteCall((void*)0x53F026, isObjectDebug);
		WriteCall((void*)0x53F4EC, isObjectDebug);
		WriteCall((void*)0x540750, isObjectDebug);
		WriteCall((void*)0x540B4C, isObjectDebug);
		WriteCall((void*)0x540EFC, isObjectDebug);
		WriteCall((void*)0x541400, isObjectDebug);
		WriteCall((void*)0x5431E6, isObjectDebug);
		WriteCall((void*)0x54357B, isObjectDebug);
		WriteCall((void*)0x54392E, isObjectDebug);
		WriteCall((void*)0x54491C, isObjectDebug);
		WriteCall((void*)0x5450BB, isObjectDebug);
		WriteCall((void*)0x54518B, isObjectDebug);
		WriteCall((void*)0x5455DA, isObjectDebug);
		WriteCall((void*)0x54568C, isObjectDebug);
		WriteCall((void*)0x5458EB, isObjectDebug);
		WriteCall((void*)0x548E43, isObjectDebug);
		WriteCall((void*)0x5499AB, isObjectDebug);
		WriteCall((void*)0x54E1EE, isObjectDebug);
		WriteCall((void*)0x54E368, isObjectDebug);
		WriteCall((void*)0x55A949, isObjectDebug);
		WriteCall((void*)0x55D27C, isObjectDebug);
		WriteCall((void*)0x563F06, isObjectDebug);
		WriteCall((void*)0x564724, isObjectDebug);
		WriteCall((void*)0x5648BF, isObjectDebug);
		WriteCall((void*)0x564A5A, isObjectDebug);
		WriteCall((void*)0x564AE7, isObjectDebug);
		WriteCall((void*)0x5652C1, isObjectDebug);
		WriteCall((void*)0x5657CA, isObjectDebug);
		WriteCall((void*)0x565B04, isObjectDebug);
		WriteCall((void*)0x595861, isObjectDebug);
		WriteCall((void*)0x598064, isObjectDebug);
		WriteCall((void*)0x598078, isObjectDebug);
		WriteCall((void*)0x5980E6, isObjectDebug);
		WriteCall((void*)0x5981B1, isObjectDebug);
		WriteCall((void*)0x5983D1, isObjectDebug);
		WriteCall((void*)0x5986FB, isObjectDebug);
		WriteCall((void*)0x59898F, isObjectDebug);
		WriteCall((void*)0x598A32, isObjectDebug);
		WriteCall((void*)0x598E9F, isObjectDebug);
		WriteCall((void*)0x598F9E, isObjectDebug);
		WriteCall((void*)0x59AB20, isObjectDebug);
		WriteCall((void*)0x59B2A7, isObjectDebug);
		WriteCall((void*)0x59B94F, isObjectDebug);
		WriteCall((void*)0x59BA6A, isObjectDebug);
		WriteCall((void*)0x59BAF3, isObjectDebug);
		WriteCall((void*)0x59BDBB, isObjectDebug);
		WriteCall((void*)0x59BED7, isObjectDebug);
		WriteCall((void*)0x59BFDF, isObjectDebug);
		WriteCall((void*)0x59C1B7, isObjectDebug);
		WriteCall((void*)0x59C326, isObjectDebug);
		WriteCall((void*)0x59C398, isObjectDebug);
		WriteCall((void*)0x59C48C, isObjectDebug);
		WriteCall((void*)0x59C573, isObjectDebug);
		WriteCall((void*)0x59C70C, isObjectDebug);
		WriteCall((void*)0x59C861, isObjectDebug);
		WriteCall((void*)0x59CB6D, isObjectDebug);
		WriteCall((void*)0x59CE53, isObjectDebug);
		WriteCall((void*)0x59CFD0, isObjectDebug);
		WriteCall((void*)0x59D0F0, isObjectDebug);
		WriteCall((void*)0x59D210, isObjectDebug);
		WriteCall((void*)0x59D370, isObjectDebug);
		WriteCall((void*)0x59D480, isObjectDebug);
		WriteCall((void*)0x59D5E0, isObjectDebug);
		WriteCall((void*)0x59D81A, isObjectDebug);
		WriteCall((void*)0x59D84A, isObjectDebug);
		WriteCall((void*)0x59D99A, isObjectDebug);
		WriteCall((void*)0x59DC4C, isObjectDebug);
		WriteCall((void*)0x59DF18, isObjectDebug);
		WriteCall((void*)0x59E190, isObjectDebug);
		WriteCall((void*)0x59E450, isObjectDebug);
		WriteCall((void*)0x59E680, isObjectDebug);
		WriteCall((void*)0x59E95C, isObjectDebug);
		WriteCall((void*)0x59EB1C, isObjectDebug);
		WriteCall((void*)0x59EC10, isObjectDebug);
		WriteCall((void*)0x59ED70, isObjectDebug);
		WriteCall((void*)0x59EEC0, isObjectDebug);
		WriteCall((void*)0x59F000, isObjectDebug);
		WriteCall((void*)0x59F252, isObjectDebug);
		WriteCall((void*)0x59F4F5, isObjectDebug);
		WriteCall((void*)0x59F640, isObjectDebug);
		WriteCall((void*)0x59F790, isObjectDebug);
		WriteCall((void*)0x59FA60, isObjectDebug);
		WriteCall((void*)0x59FD70, isObjectDebug);
		WriteCall((void*)0x59FF50, isObjectDebug);
		WriteCall((void*)0x5A0250, isObjectDebug);
		WriteCall((void*)0x5A0560, isObjectDebug);
		WriteCall((void*)0x5A0740, isObjectDebug);
		WriteCall((void*)0x5A08B0, isObjectDebug);
		WriteCall((void*)0x5A09F0, isObjectDebug);
		WriteCall((void*)0x5A0B30, isObjectDebug);
		WriteCall((void*)0x5A0C70, isObjectDebug);
		WriteCall((void*)0x5A16D1, isObjectDebug);
		WriteCall((void*)0x5A1D90, isObjectDebug);
		WriteCall((void*)0x5A1F7D, isObjectDebug);
		WriteCall((void*)0x5A215B, isObjectDebug);
		WriteCall((void*)0x5A2270, isObjectDebug);
		WriteCall((void*)0x5A24B1, isObjectDebug);
		WriteCall((void*)0x5A296D, isObjectDebug);
		WriteCall((void*)0x5A2DE0, isObjectDebug);
		WriteCall((void*)0x5A2F30, isObjectDebug);
		WriteCall((void*)0x5A3050, isObjectDebug);
		WriteCall((void*)0x5A3230, isObjectDebug);
		WriteCall((void*)0x5A33F0, isObjectDebug);
		WriteCall((void*)0x5A499B, isObjectDebug);
		WriteCall((void*)0x5A4DA9, isObjectDebug);
		WriteCall((void*)0x5A5121, isObjectDebug);
		WriteCall((void*)0x5A56AF, isObjectDebug);
		WriteCall((void*)0x5A575F, isObjectDebug);
		WriteCall((void*)0x5A589A, isObjectDebug);
		WriteCall((void*)0x5A5B8C, isObjectDebug);
		WriteCall((void*)0x5A6820, isObjectDebug);
		WriteCall((void*)0x5A6BCC, isObjectDebug);
		WriteCall((void*)0x5A6ED0, isObjectDebug);
		WriteCall((void*)0x5A70DC, isObjectDebug);
		WriteCall((void*)0x5A7B50, isObjectDebug);
		WriteCall((void*)0x5A87F0, isObjectDebug);
		WriteCall((void*)0x5A8CB3, isObjectDebug);
		WriteCall((void*)0x5A8EA9, isObjectDebug);
		WriteCall((void*)0x5A9020, isObjectDebug);
		WriteCall((void*)0x5A9149, isObjectDebug);
		WriteCall((void*)0x5A99AF, isObjectDebug);
		WriteCall((void*)0x5A9ADD, isObjectDebug);
		WriteCall((void*)0x5AFFB6, isObjectDebug);
		WriteCall((void*)0x5B050C, isObjectDebug);
		WriteCall((void*)0x5B077C, isObjectDebug);
		WriteCall((void*)0x5B0BBD, isObjectDebug);
		WriteCall((void*)0x5B2431, isObjectDebug);
		WriteCall((void*)0x5B2497, isObjectDebug);
		WriteCall((void*)0x5B256C, isObjectDebug);
		WriteCall((void*)0x5B2580, isObjectDebug);
		WriteCall((void*)0x5B26A0, isObjectDebug);
		WriteCall((void*)0x5B27D0, isObjectDebug);
		WriteCall((void*)0x5B2EE5, isObjectDebug);
		WriteCall((void*)0x5B3578, isObjectDebug);
		WriteCall((void*)0x5B403A, isObjectDebug); //
		WriteCall((void*)0x5B420B, isObjectDebug);
		WriteCall((void*)0x5B4288, isObjectDebug);
		WriteCall((void*)0x5B42E7, isObjectDebug);
		WriteCall((void*)0x5B45FA, isObjectDebug);
		WriteCall((void*)0x5B4622, isObjectDebug);
		WriteCall((void*)0x5B4860, isObjectDebug);
		WriteCall((void*)0x5B4B50, isObjectDebug);
		WriteCall((void*)0x5B4D2A, isObjectDebug);
		WriteCall((void*)0x5B520A, isObjectDebug);
		WriteCall((void*)0x5B5430, isObjectDebug);
		WriteCall((void*)0x5B6289, isObjectDebug);
		WriteCall((void*)0x5B64D1, isObjectDebug);
		WriteCall((void*)0x5B7231, isObjectDebug);
		WriteCall((void*)0x5B761C, isObjectDebug);
		WriteCall((void*)0x5B78C0, isObjectDebug);
		WriteCall((void*)0x5B7C0B, isObjectDebug);
		WriteCall((void*)0x5B7D90, isObjectDebug);
		WriteCall((void*)0x5B7ECB, isObjectDebug);
		WriteCall((void*)0x5B8615, isObjectDebug);
		WriteCall((void*)0x5B8646, isObjectDebug);
		WriteCall((void*)0x5B8F9C, isObjectDebug);
		WriteCall((void*)0x5B9057, isObjectDebug);
		WriteCall((void*)0x5B97F4, isObjectDebug);
		WriteCall((void*)0x5B9830, isObjectDebug);
		WriteCall((void*)0x5B9C5C, isObjectDebug);
		WriteCall((void*)0x5BA040, isObjectDebug);
		WriteCall((void*)0x5BA4B0, isObjectDebug);
		WriteCall((void*)0x5BA920, isObjectDebug);
		WriteCall((void*)0x5BAD10, isObjectDebug);
		WriteCall((void*)0x5BB180, isObjectDebug);
		WriteCall((void*)0x5BB5F0, isObjectDebug);
		WriteCall((void*)0x5BBA60, isObjectDebug);
		WriteCall((void*)0x5BBDDC, isObjectDebug);
		WriteCall((void*)0x5BC0BC, isObjectDebug);
		WriteCall((void*)0x5BC21C, isObjectDebug);
		WriteCall((void*)0x5BC50C, isObjectDebug);
		WriteCall((void*)0x5BC77C, isObjectDebug);
		WriteCall((void*)0x5BCA9C, isObjectDebug);
		WriteCall((void*)0x5BCDCC, isObjectDebug);
		WriteCall((void*)0x5BD12C, isObjectDebug);
		WriteCall((void*)0x5BD32C, isObjectDebug);
		WriteCall((void*)0x5BD40C, isObjectDebug);
		WriteCall((void*)0x5BD762, isObjectDebug);
		WriteCall((void*)0x5BD89C, isObjectDebug);
		WriteCall((void*)0x5BE67C, isObjectDebug);
		WriteCall((void*)0x5BE89C, isObjectDebug);
		WriteCall((void*)0x5BE9D0, isObjectDebug);
		WriteCall((void*)0x5BEBBC, isObjectDebug);
		WriteCall((void*)0x5BEDB0, isObjectDebug);
		WriteCall((void*)0x5BEFC0, isObjectDebug);
		WriteCall((void*)0x5BF9BC, isObjectDebug);
		WriteCall((void*)0x5C027C, isObjectDebug);
		WriteCall((void*)0x5C35D0, isObjectDebug);
		WriteCall((void*)0x5C36BE, isObjectDebug);
		WriteCall((void*)0x5C373F, isObjectDebug);
		WriteCall((void*)0x5C382C, isObjectDebug);
		WriteCall((void*)0x5C38A3, isObjectDebug);
		WriteCall((void*)0x5C392C, isObjectDebug);
		WriteCall((void*)0x5C4115, isObjectDebug);
		WriteCall((void*)0x5C4409, isObjectDebug);
		WriteCall((void*)0x5C453C, isObjectDebug);
		WriteCall((void*)0x5C47EA, isObjectDebug);
		WriteCall((void*)0x5C4844, isObjectDebug);
		WriteCall((void*)0x5C4FD3, isObjectDebug);
		WriteCall((void*)0x5C5423, isObjectDebug);
		WriteCall((void*)0x5C570E, isObjectDebug);
		WriteCall((void*)0x5C57C9, isObjectDebug);
		WriteCall((void*)0x5C5830, isObjectDebug);
		WriteCall((void*)0x5C5977, isObjectDebug);
		WriteCall((void*)0x5C5EAA, isObjectDebug);
		WriteCall((void*)0x5C63F1, isObjectDebug);
		WriteCall((void*)0x5C699A, isObjectDebug);
		WriteCall((void*)0x5C6EE1, isObjectDebug);
		WriteCall((void*)0x5C748A, isObjectDebug);
		WriteCall((void*)0x5C79DE, isObjectDebug);
		WriteCall((void*)0x5C803A, isObjectDebug);
		WriteCall((void*)0x5C8581, isObjectDebug);
		WriteCall((void*)0x5C8730, isObjectDebug);
		WriteCall((void*)0x5C8B63, isObjectDebug);
		WriteCall((void*)0x5C9344, isObjectDebug);
		WriteCall((void*)0x5C95DF, isObjectDebug);
		WriteCall((void*)0x5C9871, isObjectDebug);
		WriteCall((void*)0x5C9B06, isObjectDebug);
		WriteCall((void*)0x5C9D26, isObjectDebug);
		WriteCall((void*)0x5C9F65, isObjectDebug);
		WriteCall((void*)0x5CA0EB, isObjectDebug);
		WriteCall((void*)0x5CA3F1, isObjectDebug);
		WriteCall((void*)0x5CA4C1, isObjectDebug);
		WriteCall((void*)0x5CA69E, isObjectDebug);
		WriteCall((void*)0x5CA71E, isObjectDebug);
		WriteCall((void*)0x5CA7A0, isObjectDebug);
		WriteCall((void*)0x5CA86E, isObjectDebug);
		WriteCall((void*)0x5CA8D6, isObjectDebug);
		WriteCall((void*)0x5CA982, isObjectDebug);
		WriteCall((void*)0x5CA9E9, isObjectDebug);
		WriteCall((void*)0x5CAC7B, isObjectDebug);
		WriteCall((void*)0x5CAD01, isObjectDebug);
		WriteCall((void*)0x5CB173, isObjectDebug);
		WriteCall((void*)0x5CB2B3, isObjectDebug);
		WriteCall((void*)0x5CB471, isObjectDebug);
		WriteCall((void*)0x5CBB90, isObjectDebug);
		WriteCall((void*)0x5CC62A, isObjectDebug);
		WriteCall((void*)0x5CCA6B, isObjectDebug);
		WriteCall((void*)0x5CCB11, isObjectDebug);
		WriteCall((void*)0x5CCD2B, isObjectDebug);
		WriteCall((void*)0x5CCDC1, isObjectDebug);
		WriteCall((void*)0x5CCF17, isObjectDebug);
		WriteCall((void*)0x5CCF84, isObjectDebug);
		WriteCall((void*)0x5CD162, isObjectDebug);
		WriteCall((void*)0x5CD25F, isObjectDebug);
		WriteCall((void*)0x5CD580, isObjectDebug);
		WriteCall((void*)0x5CD9D9, isObjectDebug);
		WriteCall((void*)0x5CDA89, isObjectDebug);
		WriteCall((void*)0x5CDB84, isObjectDebug);
		WriteCall((void*)0x5CDC11, isObjectDebug);
		WriteCall((void*)0x5CDEA9, isObjectDebug);
		WriteCall((void*)0x5CE1FA, isObjectDebug);
		WriteCall((void*)0x5CE419, isObjectDebug);
		WriteCall((void*)0x5CE52A, isObjectDebug);
		WriteCall((void*)0x5CE69F, isObjectDebug);
		WriteCall((void*)0x5CE6F7, isObjectDebug);
		WriteCall((void*)0x5CE8D7, isObjectDebug);
		WriteCall((void*)0x5CE957, isObjectDebug);
		WriteCall((void*)0x5CEAF0, isObjectDebug);
		WriteCall((void*)0x5CEB7A, isObjectDebug);
		WriteCall((void*)0x5CED2B, isObjectDebug);
		WriteCall((void*)0x5CED7F, isObjectDebug);
		WriteCall((void*)0x5CEEA2, isObjectDebug);
		WriteCall((void*)0x5CEED5, isObjectDebug);
		WriteCall((void*)0x5CEF5B, isObjectDebug);
		WriteCall((void*)0x5CF114, isObjectDebug);
		WriteCall((void*)0x5CF5DF, isObjectDebug);
		WriteCall((void*)0x5CF65A, isObjectDebug);
		WriteCall((void*)0x5CF69E, isObjectDebug);
		WriteCall((void*)0x5CFA2F, isObjectDebug);
		WriteCall((void*)0x5D042A, isObjectDebug);
		WriteCall((void*)0x5D05F6, isObjectDebug);
		WriteCall((void*)0x5D06B7, isObjectDebug);
		WriteCall((void*)0x5D0726, isObjectDebug);
		WriteCall((void*)0x5D0847, isObjectDebug);
		WriteCall((void*)0x5D0A4A, isObjectDebug);
		WriteCall((void*)0x5D0BBC, isObjectDebug);
		WriteCall((void*)0x5D10E9, isObjectDebug);
		WriteCall((void*)0x5D146C, isObjectDebug);
		WriteCall((void*)0x5D1A21, isObjectDebug);
		WriteCall((void*)0x5D2225, isObjectDebug);
		WriteCall((void*)0x5D25FE, isObjectDebug);
		WriteCall((void*)0x5D27BB, isObjectDebug);
		WriteCall((void*)0x5D2B59, isObjectDebug);
		WriteCall((void*)0x5D2D6D, isObjectDebug);
		WriteCall((void*)0x5D3099, isObjectDebug);
		WriteCall((void*)0x5D336A, isObjectDebug);
		WriteCall((void*)0x5D3659, isObjectDebug);
		WriteCall((void*)0x5D392A, isObjectDebug);
		WriteCall((void*)0x5D39DC, isObjectDebug);
		WriteCall((void*)0x5D3A3C, isObjectDebug);
		WriteCall((void*)0x5D3B27, isObjectDebug);
		WriteCall((void*)0x5D3C3A, isObjectDebug);
		WriteCall((void*)0x5D3D75, isObjectDebug);
		WriteCall((void*)0x5D3DBA, isObjectDebug);
		WriteCall((void*)0x5D3FE5, isObjectDebug);
		WriteCall((void*)0x5D416B, isObjectDebug);
		WriteCall((void*)0x5D48D7, isObjectDebug);
		WriteCall((void*)0x5D4A67, isObjectDebug);
		WriteCall((void*)0x5D4BB7, isObjectDebug);
		WriteCall((void*)0x5D4CEA, isObjectDebug);
		WriteCall((void*)0x5D512E, isObjectDebug);
		WriteCall((void*)0x5DBA39, isObjectDebug);
		WriteCall((void*)0x5DBBA9, isObjectDebug);
		WriteCall((void*)0x5DD128, isObjectDebug);
		WriteCall((void*)0x5DD13C, isObjectDebug);
		WriteCall((void*)0x5DDC51, isObjectDebug);
		WriteCall((void*)0x5DDD6C, isObjectDebug);
		WriteCall((void*)0x5DDEC1, isObjectDebug);
		WriteCall((void*)0x5DDF2A, isObjectDebug);
		WriteCall((void*)0x5DE011, isObjectDebug);
		WriteCall((void*)0x5DE05A, isObjectDebug);
		WriteCall((void*)0x5DE132, isObjectDebug);
		WriteCall((void*)0x5DE1BB, isObjectDebug);
		WriteCall((void*)0x5DE2B1, isObjectDebug);
		WriteCall((void*)0x5DE489, isObjectDebug);
		WriteCall((void*)0x5DE61C, isObjectDebug);
		WriteCall((void*)0x5DE7E2, isObjectDebug);
		WriteCall((void*)0x5DEA02, isObjectDebug);
		WriteCall((void*)0x5DEB12, isObjectDebug);
		WriteCall((void*)0x5DEDC6, isObjectDebug);
		WriteCall((void*)0x5DEF42, isObjectDebug);
		WriteCall((void*)0x5DF007, isObjectDebug);
		WriteCall((void*)0x5DF1B2, isObjectDebug);
		WriteCall((void*)0x5E2640, isObjectDebug);
		WriteCall((void*)0x5E265F, isObjectDebug);
		WriteCall((void*)0x5E2B33, isObjectDebug);
		WriteCall((void*)0x5E2BBF, isObjectDebug);
		WriteCall((void*)0x5E2BE4, isObjectDebug);
		WriteCall((void*)0x5E3255, isObjectDebug);
		WriteCall((void*)0x5E32B5, isObjectDebug);
		WriteCall((void*)0x5E3337, isObjectDebug);
		WriteCall((void*)0x5E40A0, isObjectDebug);
		WriteCall((void*)0x5E61C7, isObjectDebug);
		WriteCall((void*)0x5E62ED, isObjectDebug);
		WriteCall((void*)0x5E64FC, isObjectDebug);
		WriteCall((void*)0x5E6AE7, isObjectDebug);
		WriteCall((void*)0x5E6B8B, isObjectDebug);
		WriteCall((void*)0x5E723F, isObjectDebug);
		WriteCall((void*)0x5E725A, isObjectDebug);
		WriteCall((void*)0x5E74BA, isObjectDebug);
		WriteCall((void*)0x5E74D7, isObjectDebug);
		WriteCall((void*)0x5E767E, isObjectDebug);
		WriteCall((void*)0x5E7973, isObjectDebug);
		WriteCall((void*)0x5E7B81, isObjectDebug);
		WriteCall((void*)0x5E8146, isObjectDebug);
		WriteCall((void*)0x5E8338, isObjectDebug);
		WriteCall((void*)0x5E84F9, isObjectDebug);
		WriteCall((void*)0x5E8E38, isObjectDebug);
		WriteCall((void*)0x5E9496, isObjectDebug);
		WriteCall((void*)0x5E9815, isObjectDebug);
		WriteCall((void*)0x5E9985, isObjectDebug);
		WriteCall((void*)0x5E9ACD, isObjectDebug);
		WriteCall((void*)0x5E9D56, isObjectDebug);
		WriteCall((void*)0x5EA022, isObjectDebug);
		WriteCall((void*)0x5EA5BD, isObjectDebug);
		WriteCall((void*)0x5EAA52, isObjectDebug);
		WriteCall((void*)0x5EAC0B, isObjectDebug);
		WriteCall((void*)0x5EAF5B, isObjectDebug);
		WriteCall((void*)0x5EB6CB, isObjectDebug);
		WriteCall((void*)0x5EB72A, isObjectDebug);
		WriteCall((void*)0x5EC378, isObjectDebug);
		WriteCall((void*)0x5EC3A6, isObjectDebug);
		WriteCall((void*)0x5EC46B, isObjectDebug);
		WriteCall((void*)0x5EDC78, isObjectDebug);
		WriteCall((void*)0x5EDD39, isObjectDebug);
		WriteCall((void*)0x5EF9E9, isObjectDebug);
		WriteCall((void*)0x5EFA57, isObjectDebug);
		WriteCall((void*)0x5EFB0A, isObjectDebug);
		WriteCall((void*)0x5F0058, isObjectDebug);
		WriteCall((void*)0x5F069F, isObjectDebug);
		WriteCall((void*)0x5F09BE, isObjectDebug);
		WriteCall((void*)0x5F0A4E, isObjectDebug);
		WriteCall((void*)0x5F0B56, isObjectDebug);
		WriteCall((void*)0x5F0CF5, isObjectDebug);
		WriteCall((void*)0x5F0E86, isObjectDebug);
		WriteCall((void*)0x5F0FD7, isObjectDebug);
		WriteCall((void*)0x5F13CB, isObjectDebug);
		WriteCall((void*)0x5F154A, isObjectDebug);
		WriteCall((void*)0x5F240F, isObjectDebug);
		WriteCall((void*)0x5F39AC, isObjectDebug);
		WriteCall((void*)0x5F401B, isObjectDebug);
		WriteCall((void*)0x5F4129, isObjectDebug);
		WriteCall((void*)0x5F432B, isObjectDebug);
		WriteCall((void*)0x5F46BF, isObjectDebug);
		WriteCall((void*)0x5F59AA, isObjectDebug);
		WriteCall((void*)0x5F6071, isObjectDebug);
		WriteCall((void*)0x5F6686, isObjectDebug);
		WriteCall((void*)0x5F7CEE, isObjectDebug);
		WriteCall((void*)0x5F84EA, isObjectDebug);
		WriteCall((void*)0x5F87B4, isObjectDebug);
		WriteCall((void*)0x5F8E0A, isObjectDebug);
		WriteCall((void*)0x5F90D3, isObjectDebug);
		WriteCall((void*)0x5F971A, isObjectDebug);
		WriteCall((void*)0x5F9928, isObjectDebug);
		WriteCall((void*)0x5F994B, isObjectDebug);
		WriteCall((void*)0x5F9BA8, isObjectDebug);
		WriteCall((void*)0x5F9BCB, isObjectDebug);
		WriteCall((void*)0x5FA0FC, isObjectDebug);
		WriteCall((void*)0x5FA35F, isObjectDebug);
		WriteCall((void*)0x5FA708, isObjectDebug);
		WriteCall((void*)0x5FA72B, isObjectDebug);
		WriteCall((void*)0x5FA9B7, isObjectDebug);
		WriteCall((void*)0x5FA9DB, isObjectDebug);
		WriteCall((void*)0x5FAEB9, isObjectDebug);
		WriteCall((void*)0x5FB13B, isObjectDebug);
		WriteCall((void*)0x5FB626, isObjectDebug);
		WriteCall((void*)0x5FB89B, isObjectDebug);
		WriteCall((void*)0x5FC165, isObjectDebug);
		WriteCall((void*)0x5FC5C5, isObjectDebug);
		WriteCall((void*)0x5FC7C4, isObjectDebug);
		WriteCall((void*)0x5FCC0B, isObjectDebug);
		WriteCall((void*)0x5FD3B4, isObjectDebug);
		WriteCall((void*)0x5FD7AB, isObjectDebug);
		WriteCall((void*)0x5FDC90, isObjectDebug);
		WriteCall((void*)0x5FDE96, isObjectDebug);
		WriteCall((void*)0x5FDF9A, isObjectDebug);
		WriteCall((void*)0x5FDFEA, isObjectDebug);
		WriteCall((void*)0x5FF75E, isObjectDebug);
		WriteCall((void*)0x5FF9FA, isObjectDebug);
		WriteCall((void*)0x5FFE76, isObjectDebug);
		WriteCall((void*)0x600B7A, isObjectDebug);
		WriteCall((void*)0x6039DA, isObjectDebug);
		WriteCall((void*)0x603C9A, isObjectDebug);
		WriteCall((void*)0x603F97, isObjectDebug);
		WriteCall((void*)0x60400E, isObjectDebug);
		WriteCall((void*)0x6042B7, isObjectDebug);
		WriteCall((void*)0x605B4F, isObjectDebug);
		WriteCall((void*)0x605E08, isObjectDebug);
		WriteCall((void*)0x605E58, isObjectDebug);
		WriteCall((void*)0x605EA8, isObjectDebug);
		WriteCall((void*)0x605F18, isObjectDebug);
		WriteCall((void*)0x605F68, isObjectDebug);
		WriteCall((void*)0x6062AE, isObjectDebug);
		WriteCall((void*)0x606351, isObjectDebug);
		WriteCall((void*)0x60670B, isObjectDebug);
		WriteCall((void*)0x606727, isObjectDebug);
		WriteCall((void*)0x606B3E, isObjectDebug);
		WriteCall((void*)0x606E8E, isObjectDebug);
		WriteCall((void*)0x606EE0, isObjectDebug);
		WriteCall((void*)0x6079B2, isObjectDebug);
		WriteCall((void*)0x607AA4, isObjectDebug);
		WriteCall((void*)0x607B37, isObjectDebug);
		WriteCall((void*)0x607B77, isObjectDebug);
		WriteCall((void*)0x607BB3, isObjectDebug);
		WriteCall((void*)0x607CBB, isObjectDebug);
		WriteCall((void*)0x608605, isObjectDebug);
		WriteCall((void*)0x608685, isObjectDebug);
		WriteCall((void*)0x608705, isObjectDebug);
		WriteCall((void*)0x60874A, isObjectDebug);
		WriteCall((void*)0x608778, isObjectDebug);
		WriteCall((void*)0x6089FE, isObjectDebug);
		WriteCall((void*)0x608A4D, isObjectDebug);
		WriteCall((void*)0x608F45, isObjectDebug);
		WriteCall((void*)0x609223, isObjectDebug);
		WriteCall((void*)0x60936E, isObjectDebug);
		WriteCall((void*)0x6094C8, isObjectDebug);
		WriteCall((void*)0x609CAE, isObjectDebug);
		WriteCall((void*)0x609F1C, isObjectDebug);
		WriteCall((void*)0x609FCA, isObjectDebug);
		WriteCall((void*)0x60A79E, isObjectDebug);
		WriteCall((void*)0x60AA0C, isObjectDebug);
		WriteCall((void*)0x60AABA, isObjectDebug);
		WriteCall((void*)0x60AB06, isObjectDebug);
		WriteCall((void*)0x60AD37, isObjectDebug);
		WriteCall((void*)0x60AD65, isObjectDebug);
		WriteCall((void*)0x60AE3C, isObjectDebug);
		WriteCall((void*)0x60AF9B, isObjectDebug);
		WriteCall((void*)0x60B197, isObjectDebug);
		WriteCall((void*)0x60C65F, isObjectDebug);
		WriteCall((void*)0x60C85D, isObjectDebug);
		WriteCall((void*)0x60CB42, isObjectDebug);
		WriteCall((void*)0x60CB74, isObjectDebug);
		WriteCall((void*)0x60CFD9, isObjectDebug);
		WriteCall((void*)0x60D057, isObjectDebug);
		WriteCall((void*)0x60D090, isObjectDebug);
		WriteCall((void*)0x60D0AC, isObjectDebug);
		WriteCall((void*)0x60D512, isObjectDebug);
		WriteCall((void*)0x60D841, isObjectDebug);
		WriteCall((void*)0x60E172, isObjectDebug);
		WriteCall((void*)0x60E1B1, isObjectDebug);
		WriteCall((void*)0x60E44B, isObjectDebug);
		WriteCall((void*)0x60E501, isObjectDebug);
		WriteCall((void*)0x60E6CE, isObjectDebug);
		WriteCall((void*)0x60EA87, isObjectDebug);
		WriteCall((void*)0x60EE1E, isObjectDebug);
		WriteCall((void*)0x61438A, isObjectDebug);
		WriteCall((void*)0x6147E3, isObjectDebug);
		WriteCall((void*)0x614800, isObjectDebug);
		WriteCall((void*)0x614875, isObjectDebug);
		WriteCall((void*)0x614885, isObjectDebug);
		WriteCall((void*)0x6148CD, isObjectDebug);
		WriteCall((void*)0x614901, isObjectDebug);
		WriteCall((void*)0x614E91, isObjectDebug);
		WriteCall((void*)0x61507B, isObjectDebug);
		WriteCall((void*)0x6151DC, isObjectDebug);
		WriteCall((void*)0x615221, isObjectDebug);
		WriteCall((void*)0x61539D, isObjectDebug);
		WriteCall((void*)0x6153DB, isObjectDebug);
		WriteCall((void*)0x615420, isObjectDebug);
		WriteCall((void*)0x615460, isObjectDebug);
		WriteCall((void*)0x6154AB, isObjectDebug);
		WriteCall((void*)0x6155FB, isObjectDebug);
		WriteCall((void*)0x615791, isObjectDebug);
		WriteCall((void*)0x6157E0, isObjectDebug);
		WriteCall((void*)0x615841, isObjectDebug);
		WriteCall((void*)0x615891, isObjectDebug);
		WriteCall((void*)0x6158E1, isObjectDebug);
		WriteCall((void*)0x615951, isObjectDebug);
		WriteCall((void*)0x6159A1, isObjectDebug);
		WriteCall((void*)0x615C30, isObjectDebug);
		WriteCall((void*)0x615E77, isObjectDebug);
		WriteCall((void*)0x616221, isObjectDebug);
		WriteCall((void*)0x6163E0, isObjectDebug);
		WriteCall((void*)0x616420, isObjectDebug);
		WriteCall((void*)0x616476, isObjectDebug);
		WriteCall((void*)0x6164C0, isObjectDebug);
		WriteCall((void*)0x6167A7, isObjectDebug);
		WriteCall((void*)0x6167F8, isObjectDebug);
		WriteCall((void*)0x616C67, isObjectDebug);
		WriteCall((void*)0x616CAE, isObjectDebug);
		WriteCall((void*)0x617182, isObjectDebug);
		WriteCall((void*)0x6171B9, isObjectDebug);
		WriteCall((void*)0x6172B2, isObjectDebug);
		WriteCall((void*)0x6172E3, isObjectDebug);
		WriteCall((void*)0x617352, isObjectDebug);
		WriteCall((void*)0x617383, isObjectDebug);
		WriteCall((void*)0x6173E1, isObjectDebug);
		WriteCall((void*)0x61778A, isObjectDebug);
		WriteCall((void*)0x6177D1, isObjectDebug);
		WriteCall((void*)0x61782B, isObjectDebug);
		WriteCall((void*)0x617CDA, isObjectDebug);
		WriteCall((void*)0x617F0A, isObjectDebug);
		WriteCall((void*)0x61806E, isObjectDebug);
		WriteCall((void*)0x61869A, isObjectDebug);
		WriteCall((void*)0x61881C, isObjectDebug);
		WriteCall((void*)0x618A7C, isObjectDebug);
		WriteCall((void*)0x618ACC, isObjectDebug);
		WriteCall((void*)0x61997E, isObjectDebug);
		WriteCall((void*)0x619EE9, isObjectDebug);
		WriteCall((void*)0x61A34F, isObjectDebug);
		WriteCall((void*)0x61A580, isObjectDebug);
		WriteCall((void*)0x61AD07, isObjectDebug);
		WriteCall((void*)0x61AE93, isObjectDebug);
		WriteCall((void*)0x61AF90, isObjectDebug);
		WriteCall((void*)0x61B456, isObjectDebug);
		WriteCall((void*)0x61BC05, isObjectDebug);
		WriteCall((void*)0x61BDE4, isObjectDebug);
		WriteCall((void*)0x61BEF4, isObjectDebug);
		WriteCall((void*)0x61BFB3, isObjectDebug);
		WriteCall((void*)0x61C17A, isObjectDebug);
		WriteCall((void*)0x61C8AB, isObjectDebug);
		WriteCall((void*)0x61DC9A, isObjectDebug);
		WriteCall((void*)0x61E610, isObjectDebug);
		WriteCall((void*)0x61E7E0, isObjectDebug);
		WriteCall((void*)0x61EF00, isObjectDebug);
		WriteCall((void*)0x61F6E1, isObjectDebug);
		WriteCall((void*)0x620824, isObjectDebug);
		WriteCall((void*)0x620B10, isObjectDebug);
		WriteCall((void*)0x621077, isObjectDebug);
		WriteCall((void*)0x621690, isObjectDebug);
		WriteCall((void*)0x62345D, isObjectDebug);
		WriteCall((void*)0x623BBA, isObjectDebug);
		WriteCall((void*)0x6240A1, isObjectDebug);
		WriteCall((void*)0x624828, isObjectDebug);
		WriteCall((void*)0x6268E6, isObjectDebug);
		WriteCall((void*)0x62A284, isObjectDebug);
		WriteCall((void*)0x62A7B0, isObjectDebug);
		WriteCall((void*)0x630E7A, isObjectDebug);
		WriteCall((void*)0x630F3D, isObjectDebug);
		WriteCall((void*)0x630F7A, isObjectDebug);
		WriteCall((void*)0x630FC7, isObjectDebug);
		WriteCall((void*)0x631047, isObjectDebug);
		WriteCall((void*)0x6310C7, isObjectDebug);
		WriteCall((void*)0x631486, isObjectDebug);
		WriteCall((void*)0x632038, isObjectDebug);
		WriteCall((void*)0x632066, isObjectDebug);
		WriteCall((void*)0x6334BA, isObjectDebug);
		WriteCall((void*)0x633EF6, isObjectDebug);
		WriteCall((void*)0x633F7D, isObjectDebug);
		WriteCall((void*)0x634458, isObjectDebug);
		WriteCall((void*)0x6347BE, isObjectDebug);
		WriteCall((void*)0x63587A, isObjectDebug);
		WriteCall((void*)0x635A4B, isObjectDebug);
		WriteCall((void*)0x635D6B, isObjectDebug);
		WriteCall((void*)0x635F29, isObjectDebug);
		WriteCall((void*)0x635FDB, isObjectDebug);
		WriteCall((void*)0x636102, isObjectDebug);
		WriteCall((void*)0x63614C, isObjectDebug);
		WriteCall((void*)0x636235, isObjectDebug);
		WriteCall((void*)0x636439, isObjectDebug);
		WriteCall((void*)0x63646E, isObjectDebug);
		WriteCall((void*)0x636597, isObjectDebug);
		WriteCall((void*)0x6365D1, isObjectDebug);
		WriteCall((void*)0x636958, isObjectDebug);
		WriteCall((void*)0x636B51, isObjectDebug);
		WriteCall((void*)0x63707D, isObjectDebug);
		WriteCall((void*)0x6370E3, isObjectDebug);
		WriteCall((void*)0x63736B, isObjectDebug);
		WriteCall((void*)0x63758A, isObjectDebug);
		WriteCall((void*)0x63787E, isObjectDebug);
		WriteCall((void*)0x637CEB, isObjectDebug);
		WriteCall((void*)0x637E47, isObjectDebug);
		WriteCall((void*)0x637EEB, isObjectDebug);
		WriteCall((void*)0x638002, isObjectDebug);
		WriteCall((void*)0x63805B, isObjectDebug);
		WriteCall((void*)0x638227, isObjectDebug);
		WriteCall((void*)0x638307, isObjectDebug);
		WriteCall((void*)0x63838B, isObjectDebug);
		WriteCall((void*)0x6387BB, isObjectDebug);
		WriteCall((void*)0x638827, isObjectDebug);
		WriteCall((void*)0x638957, isObjectDebug);
		WriteCall((void*)0x63899A, isObjectDebug);
		WriteCall((void*)0x638BA7, isObjectDebug);
		WriteCall((void*)0x638C19, isObjectDebug);
		WriteCall((void*)0x638FE9, isObjectDebug);
		WriteCall((void*)0x639141, isObjectDebug);
		WriteCall((void*)0x6391AB, isObjectDebug);
		WriteCall((void*)0x639207, isObjectDebug);
		WriteCall((void*)0x639315, isObjectDebug);
		WriteCall((void*)0x6393A4, isObjectDebug);
		WriteCall((void*)0x639415, isObjectDebug);
		WriteCall((void*)0x639697, isObjectDebug);
		WriteCall((void*)0x639705, isObjectDebug);
		WriteCall((void*)0x63981B, isObjectDebug);
		WriteCall((void*)0x639935, isObjectDebug);
		WriteCall((void*)0x639982, isObjectDebug);
		WriteCall((void*)0x639BD6, isObjectDebug);
		WriteCall((void*)0x639E83, isObjectDebug);
		WriteCall((void*)0x63A11A, isObjectDebug);
		WriteCall((void*)0x63A1A1, isObjectDebug);
		WriteCall((void*)0x63A3B6, isObjectDebug);
		WriteCall((void*)0x63A3EA, isObjectDebug);
		WriteCall((void*)0x63A44A, isObjectDebug);
		WriteCall((void*)0x63A506, isObjectDebug);
		WriteCall((void*)0x63A563, isObjectDebug);
		WriteCall((void*)0x63A6CA, isObjectDebug);
		WriteCall((void*)0x63A7AA, isObjectDebug);
		WriteCall((void*)0x63A9D7, isObjectDebug);
		WriteCall((void*)0x63AA28, isObjectDebug);
		WriteCall((void*)0x63AAFA, isObjectDebug);
		WriteCall((void*)0x63B117, isObjectDebug);
		WriteCall((void*)0x63B180, isObjectDebug);
		WriteCall((void*)0x63B429, isObjectDebug);
		WriteCall((void*)0x63B6B9, isObjectDebug);
		WriteCall((void*)0x63B9F7, isObjectDebug);
		WriteCall((void*)0x63BA33, isObjectDebug);
		WriteCall((void*)0x63BDB0, isObjectDebug);
		WriteCall((void*)0x63BE2C, isObjectDebug);
		WriteCall((void*)0x63C04F, isObjectDebug);
		WriteCall((void*)0x63C3CD, isObjectDebug);
		WriteCall((void*)0x63C717, isObjectDebug); //1
		WriteCall((void*)0x63D207, isObjectDebug);
		WriteCall((void*)0x63D24D, isObjectDebug);
		WriteCall((void*)0x63D29D, isObjectDebug);
		WriteCall((void*)0x63D2ED, isObjectDebug);
		WriteCall((void*)0x63D33D, isObjectDebug);
		WriteCall((void*)0x63D47B, isObjectDebug);
		WriteCall((void*)0x63D58A, isObjectDebug);
		WriteCall((void*)0x63D5DE, isObjectDebug);
		WriteCall((void*)0x63D9EA, isObjectDebug);
		WriteCall((void*)0x63DA8D, isObjectDebug);
		WriteCall((void*)0x63DCA6, isObjectDebug);
		WriteCall((void*)0x63DF5D, isObjectDebug);
		WriteCall((void*)0x63E083, isObjectDebug);
		WriteCall((void*)0x63E0F2, isObjectDebug);
		WriteCall((void*)0x63E43A, isObjectDebug);
		WriteCall((void*)0x63E6FA, isObjectDebug);
		WriteCall((void*)0x63E9AA, isObjectDebug);
		WriteCall((void*)0x63E9EA, isObjectDebug);
		WriteCall((void*)0x63ED6A, isObjectDebug);
		WriteCall((void*)0x63F26D, isObjectDebug);
		WriteCall((void*)0x63F364, isObjectDebug);
		WriteCall((void*)0x63F72A, isObjectDebug);
		WriteCall((void*)0x63FB0A, isObjectDebug);
		WriteCall((void*)0x63FB87, isObjectDebug);
		WriteCall((void*)0x63FC77, isObjectDebug);
		WriteCall((void*)0x640447, isObjectDebug);
		WriteCall((void*)0x6406E1, isObjectDebug);
		WriteCall((void*)0x64085A, isObjectDebug);
		WriteCall((void*)0x6FE65D, isObjectDebug);
		WriteCall((void*)0x70115D, isObjectDebug);
		WriteCall((void*)0x7160AC, isObjectDebug);
		WriteCall((void*)0x716CD0, isObjectDebug);
		WriteCall((void*)0x71B8AE, isObjectDebug);
		WriteCall((void*)0x71BBB6, isObjectDebug);
		WriteCall((void*)0x71BEA6, isObjectDebug);
		WriteCall((void*)0x71C016, isObjectDebug);
		WriteCall((void*)0x71C10B, isObjectDebug);
		WriteCall((void*)0x71C1A8, isObjectDebug);
		WriteCall((void*)0x71C2BF, isObjectDebug);
		WriteCall((void*)0x71C564, isObjectDebug);
		WriteCall((void*)0x71C744, isObjectDebug);
		WriteCall((void*)0x71C924, isObjectDebug);
		WriteCall((void*)0x71CBF3, isObjectDebug);
		WriteCall((void*)0x71CCC4, isObjectDebug);
		WriteCall((void*)0x71CE1A, isObjectDebug);
		WriteCall((void*)0x71CFA6, isObjectDebug);
		WriteCall((void*)0x71D054, isObjectDebug);
		WriteCall((void*)0x71D0F8, isObjectDebug);
		WriteCall((void*)0x72976B, isObjectDebug);
		WriteCall((void*)0x729971, isObjectDebug);
		WriteCall((void*)0x72BD1E, isObjectDebug);
		WriteCall((void*)0x74C876, isObjectDebug);
		WriteCall((void*)0x74C87D, isObjectDebug);
		WriteCall((void*)0x74C883, isObjectDebug);
		WriteCall((void*)0x74C93C, isObjectDebug);
		WriteCall((void*)0x74CAB5, isObjectDebug);
		WriteCall((void*)0x74CF50, isObjectDebug);
		WriteCall((void*)0x74D40F, isObjectDebug);
		WriteCall((void*)0x74D471, isObjectDebug);
		WriteCall((void*)0x74D4E9, isObjectDebug);
		WriteCall((void*)0x75A4BA, isObjectDebug);
		WriteCall((void*)0x75A684, isObjectDebug);
		WriteCall((void*)0x79AA25, isObjectDebug);
		WriteCall((void*)0x79B38F, isObjectDebug);
		WriteCall((void*)0x79B3AD, isObjectDebug);
		WriteCall((void*)0x79B43B, isObjectDebug);
		WriteCall((void*)0x79B44B, isObjectDebug);
		WriteCall((void*)0x79B494, isObjectDebug);
		WriteCall((void*)0x79B4DA, isObjectDebug);
		WriteCall((void*)0x79B914, isObjectDebug);
		WriteCall((void*)0x79C4A7, isObjectDebug);
		WriteCall((void*)0x79CA14, isObjectDebug);
		WriteCall((void*)0x79CC10, isObjectDebug);
		WriteCall((void*)0x79CE51, isObjectDebug);
		WriteCall((void*)0x79D465, isObjectDebug);
		WriteCall((void*)0x79D9E5, isObjectDebug);
		WriteCall((void*)0x79F7F6, isObjectDebug);
		WriteCall((void*)0x7A162A, isObjectDebug);
		WriteCall((void*)0x7A1ACC, isObjectDebug);
		WriteCall((void*)0x7A1CDA, isObjectDebug);
		WriteCall((void*)0x7A26FA, isObjectDebug);
		WriteCall((void*)0x7A2F9B, isObjectDebug);
		WriteCall((void*)0x7A30EA, isObjectDebug);
		WriteCall((void*)0x7A35D3, isObjectDebug);
		WriteCall((void*)0x7A37C2, isObjectDebug);
		WriteCall((void*)0x7A3A10, isObjectDebug);
		WriteCall((void*)0x7A3B6E, isObjectDebug);
		WriteCall((void*)0x7A3D93, isObjectDebug);
		WriteCall((void*)0x7A3E61, isObjectDebug);
		WriteCall((void*)0x7A3F22, isObjectDebug);
		WriteCall((void*)0x7A4271, isObjectDebug);
		WriteCall((void*)0x7A43FE, isObjectDebug);
		WriteCall((void*)0x7A446E, isObjectDebug);
		WriteCall((void*)0x7A4559, isObjectDebug);
		WriteCall((void*)0x7A4CC7, isObjectDebug);
		WriteCall((void*)0x7A4EE9, isObjectDebug);
		WriteCall((void*)0x7A89D8, isObjectDebug);
		WriteCall((void*)0x7A8DB2, isObjectDebug);
		WriteCall((void*)0x7A8FA9, isObjectDebug);
		WriteCall((void*)0x7A914B, isObjectDebug);
		WriteCall((void*)0x7A9B80, isObjectDebug);
		WriteCall((void*)0x7ABC52, isObjectDebug);
		WriteCall((void*)0x7AC1A2, isObjectDebug);
		WriteCall((void*)0x7AC7C5, isObjectDebug);
		WriteCall((void*)0x7AC825, isObjectDebug);
		WriteCall((void*)0x7AC885, isObjectDebug);
		WriteCall((void*)0x7AC8E5, isObjectDebug);
		WriteCall((void*)0x7B04BB, isObjectDebug);
		WriteCall((void*)0x7B0B37, isObjectDebug);
		WriteCall((void*)0x7B0C8A, isObjectDebug);
		WriteCall((void*)0x7B0E1B, isObjectDebug);
		WriteCall((void*)0x7B0E2F, isObjectDebug);
		WriteCall((void*)0x7B0E4E, isObjectDebug);
		WriteCall((void*)0x7B1158, isObjectDebug);
		WriteCall((void*)0x7B13DE, isObjectDebug);
		WriteJump((void*)0x7B14F0, isObjectDebug);
	/*	WriteCall((void*)0x7F0FE7, isObjectDebug);
		WriteData((BYTE*)0x7F0FE7, (BYTE)0x0);
		WriteCall((void*)0x7F103B, isObjectDebug);
		WriteData((BYTE*)0x7F103B, (BYTE)0x0);
		WriteCall((void*)0x7F122F, isObjectDebug);
		WriteData((BYTE*)0x7F122F, (BYTE)0x0);
		WriteCall((void*)0x7F124F, isObjectDebug);
		WriteData((BYTE*)0x7F124F, (BYTE)0x0);
		WriteCall((void*)0x7F1253, isObjectDebug);
		WriteData((BYTE*)0x7F1253, (BYTE)0x0);
		WriteCall((void*)0x7F1283, isObjectDebug);
		WriteData((BYTE*)0x7F1283, (BYTE)0x0);
		WriteCall((void*)0x7F1347, isObjectDebug);
		WriteData((BYTE*)0x7F1347, (BYTE)0x0);
		WriteCall((void*)0x7F139B, isObjectDebug);
		WriteData((BYTE*)0x7F139B, (BYTE)0x0);// I don't know what this is and it crashes FMVs, so dummied.*/
	}

	int isObjectDebug(task* tp) {
		//Check if tp is a valid pointer and if it's the selected object in the editor.
		if ((int)tp > 0x400000) {
			if (tp->twp->id == -1) {
				return 1;
			}
			else {
				return 0;
			}
		} else {
			return 0;
		}
	}

	extern "C" __declspec(dllexport) void __cdecl OnFrame()
	{
		if (inEditMode && showDisplay) {
			DisplayDebugString(1, "MEME MAKER (by Speeps)");
			DisplayDebugStringFormatted(2, "Mode = %d", mode);
			DisplayDebugStringFormatted(3, "Movement Speed = %0.2f", movementSpeed);

			switch (mode) {
			case 0:
				displayPlayerInfo(playerNo);
				break;
			case 1:
				displayCameraInfo();
				break;
			case 2:
				writeHeadDisplay(playerNo);
				break;
			case 3:
				displayEventAnimInfo();
				break;
			case 4:
				displayNPCModeInfo(playerNo);
				break;
			case 5:
				displayBossSpawnerInfo();
				break;
			case 6:
				displayObjectSpawnerInfo();
				break;
			}
		}

		//Stupid hacky way to remap the default cam zoom from Y to X when X is held.
		//Because R rotation uses the value of Buttons_Y for its speed for some reason.
		if (ControllerPointers[0]->HeldButtons & Buttons_X) {
			WriteData<1>((int*)0x462F4B, 0x04);
			WriteData<1>((int*)0x462F50, 0x74);
		}
		else {
			WriteData<1>((int*)0x462F4B, 0x02);
			WriteData<1>((int*)0x462F50, 0x7D);
		}

	}

	__declspec(dllexport) void __cdecl OnInput()
	{
		if (GameState == 15) { //Action check will crash the game on the main menu.
			for (int i = 0; i < GetPlayerCount(); i++) {
				if (isEditorEnabled() && ControllerPointers[i]->PressedButtons & Buttons_Y && ControllerPointers[i]->NotHeldButtons & Buttons_Z) {
					playerNo = i;
					if (!inEditMode) editorStart();
					else editorStop();
				}
				doHeadChecks(playerNo);
				checkNPCMode(playerNo);
			}

			//Create Custom Subtitle
			if (ControllerPointers[0]->PressedButtons & Buttons_C) {
				std::ifstream subFile(subtitleFilePath);
				if (subFile.is_open())
				{
					std::getline(subFile, subUpper);
					std::getline(subFile, subLower);
					std::getline(subFile, subVoice);
					subFile.close();
				}

				if (subVoice.length() != 0) PlayVoice(atoi(subVoice.c_str()));
				if (!(subUpper.length() == 0 && subLower.length() == 0)) {
					snprintf(customSubtitle, LengthOfArray(customSubtitle), "\a%s \n%s", subUpper.c_str(), subLower.c_str());
					DisplayHintText(customMessage, 180);
				}
			}
		}
		else if (GameState != 16 || CutsceneMode) { //Kill everything if not in normal gameplay.
			if (inEditMode) editorStop();
			resetCamera();
			stopHeadEdits(playerNo);
		}

		//Always Active
		if (inEditMode) {
			if (checkFontSize()) SetDebugFontSize((unsigned short)(((float)HorizontalResolution / 640.0f) * 8)); //Helps prevent mod conflicts
			if (EntityData1Ptrs[0]->CharID & Characters_Gamma) TimeMinutes = 2; //Stop Gamma dying
			CharObj2Ptrs[0]->UnderwaterTime = 0; //Stop 1P from drowning
			EntityData1Ptrs[playerNo]->Action = 99; //Compatibility with CharacterSelect
			DisablePause(); //Text Box will enable pause menu afterwards so this needs to run globally.
			checkCameraMode();
			doSpeedModifier();

			//Get analog data
			analogData.leftX = (double)(ControllerPointers[playerNo]->LeftStickX << 8);
			analogData.leftY = (double)(ControllerPointers[playerNo]->LeftStickY << 8);
			analogData.rightX = (double)(ControllerPointers[playerNo]->RightStickX << 8);
			analogData.rightY = (double)(ControllerPointers[playerNo]->RightStickY << 8);

			switch (mode) {
			case 0:
				doBasicAnimation(playerNo);
				doPlayerMovement(playerNo, movementSpeed, analogData);
				break;
			case 1:
				doFreeCam(playerNo, movementSpeed, analogData);
				break;
			case 2:
				doHeadFunctions(playerNo, movementSpeed, faceFilePath, analogData);
				break;
			case 3:
				doEventAnimFunction(playerNo, analogData);
				doPlayerMovement(playerNo, movementSpeed, analogData);
				break;
			case 4:
				doNPCModeDisplay(playerNo);
				doPlayerMovement(playerNo, movementSpeed, analogData);
				break;
			case 5:
				doBossSpawner(playerNo);
				doPlayerMovement(playerNo, movementSpeed, analogData);
				break;
			case 6:
				doObjectSpawner(playerNo);
				doPlayerMovement(playerNo, movementSpeed, analogData);
				break;
			}

			if (mode == 6 && inEditMode) {
				DebugMode = true;
				if (CharObj2Ptrs[playerNo]->PhysicsData.YOff < 29000.0f) {
					CharObj2Ptrs[playerNo]->PhysicsData.YOff = 30000.0f;
					CharObj2Ptrs[playerNo]->PhysicsData.CollisionSize = 9000;
				}
			}
			else {
				if (CharObj2Ptrs[playerNo]->PhysicsData.YOff > 29000.0f) {
					CharObj2Ptrs[playerNo]->PhysicsData = PhysicsArray[EntityData1Ptrs[playerNo]->CharID];
				}
				DestroyTask(selection);
				selection = 0;
				DebugMode = false;
			}

			//Change mode with B Button
			if (ControllerPointers[playerNo]->PressedButtons & Buttons_B) {
				if (mode < 6) mode++;
				else mode = 0;
			}

			//Toggle display with Start
			if (ControllerPointers[playerNo]->PressedButtons & Buttons_Start) showDisplay = !showDisplay;
		}
	}

	bool checkFontSize() {
		for (int i = 0; i < GetPlayerCount(); i++) if (ControllerPointers[i]->HeldButtons & Buttons_Z) return false;
		return true;
	}

	void editorStart() {
		//Disable Ball
		if (CharObj2Ptrs[playerNo]->ObjectHeld) EntityData1Ptrs[playerNo]->Status = Status_HoldObject;
		else EntityData1Ptrs[playerNo]->Status = 0;
		ControlEnabled = true;

		//Check for Free Camera Mode
		if ((BYTE)camera_flags == 7) wasFreeCam = true;
		else wasFreeCam = false;
		inEditMode = true;

		//Reset everything.
		setRunningAnim(playerNo);
		movementSpeed = 5.0;
		showDisplay = true;

		WriteData<1>((int*)(0x7B43B4), 0x75); //Stop Tikal's Debug movement interfering.
		WriteData<1>((int*)(0x7B5288), 0x75); //Stop Eggman's Debug movement interfering.
		WriteData<1>((int*)(0x7B529C), 0x75); //^

		WriteData<1>((int*)0x413DA4, 0xFF); //Disable the HUD so the timer doesn't overlap the display.
		WriteData<1>((int*)0x436308, 0x8C); //Stop the first person camera from activating.
		WriteData<1>((int*)0x44AEFB, 0x8F); //Disable Death Zones
		WriteData<1>((int*)0x437760, 0x75); //Stop normal cameras trying to take control
		WriteData<1>((int*)0x496F8C, 0x75); //Make Sonic invincible
	}

	void editorStop() {
		if (wasFreeCam) WriteData<1>((int*)0x3B2CBA8, 0x07); //Change Camera mode back to Free Camera if the player was in that mode.
		inEditMode = false;
		DestroyTask(selection);
		selection = 0;
		DebugMode = false;
		CharObj2Ptrs[playerNo]->PhysicsData = PhysicsArray[EntityData1Ptrs[playerNo]->CharID];

		//CharObj2Ptrs[playerNo]->AnimationThing.Index = 0;

		switch (EntityData1Ptrs[playerNo]->CharID) {
		case Characters_Sonic:
			if (CharObj2Ptrs[playerNo]->AnimationThing.Index >= 102 && CharObj2Ptrs[playerNo]->AnimationThing.Index <= 124) {
				EntityData1Ptrs[playerNo]->Action = 62;
			} else if (CharObj2Ptrs[playerNo]->AnimationThing.Index == 125) {
				EntityData1Ptrs[playerNo]->Action = 69;
				if (CharObj2Ptrs[playerNo]->ObjectHeld) EntityData1Ptrs[playerNo]->Status = Status_HoldObject;
				else EntityData1Ptrs[playerNo]->Status = EntityData1Ptrs[playerNo]->Status & ~0x2200 | 0x500;
				CharObj2Ptrs[playerNo]->SpindashSpeed = 5.0;
			} else {
				EntityData1Ptrs[playerNo]->Action = 1;
				if (CharObj2Ptrs[playerNo]->ObjectHeld) EntityData1Ptrs[playerNo]->Status = Status_HoldObject;
				else EntityData1Ptrs[playerNo]->Status = 0;
			}
			break;
		case Characters_Tails:
			if (CharObj2Ptrs[playerNo]->AnimationThing.Index >= 107 && CharObj2Ptrs[playerNo]->AnimationThing.Index <= 127) {
				EntityData1Ptrs[playerNo]->Action = 48;
			} else if (CharObj2Ptrs[playerNo]->AnimationThing.Index == 128) {
				EntityData1Ptrs[playerNo]->Action = 55;
				if (CharObj2Ptrs[playerNo]->ObjectHeld) EntityData1Ptrs[playerNo]->Status = Status_HoldObject;
				else EntityData1Ptrs[playerNo]->Status = EntityData1Ptrs[playerNo]->Status & ~0x2200 | 0x500;
				CharObj2Ptrs[playerNo]->SpindashSpeed = 5.0;
			} else {
				EntityData1Ptrs[playerNo]->Action = 1;
				if (CharObj2Ptrs[playerNo]->ObjectHeld) EntityData1Ptrs[playerNo]->Status = Status_HoldObject;
				else EntityData1Ptrs[playerNo]->Status = 0;
			}
			break;
		default:
			EntityData1Ptrs[playerNo]->Action = 1;
		}

		if (getCamMode() == 0) resetCamera();

		//Enable Debug for Eggman and Tikal
		WriteData<1>((int*)(0x7B43B4), 0x74);
		WriteData<1>((int*)(0x7B5288), 0x74);
		WriteData<1>((int*)(0x7B529C), 0x74);

		WriteData<1>((int*)0x44AEFB, 0x84); //Enable Death Zones
		WriteData<1>((int*)0x496F8C, 0x74); //Remove invincibility
		if (!isNPC(playerNo)) WriteData<1>((int*)0x413DA4, 0x00); //Enable the HUD
		EnablePause();
	}

	bool isEditorEnabled() {
		if (EntityData1Ptrs[playerNo]->Action == 45) return false;
		if (CurrentLevel == LevelIDs_SkyChase1 || CurrentLevel == LevelIDs_SkyChase2) return false;
		return true;
	}

	void doSpeedModifier() {
		if (ControllerPointers[playerNo]->HeldButtons & Buttons_X && mode != 3) {
			if (analogData.leftX > 30072.0) {
				if (movementSpeed < 16.0) movementSpeed = movementSpeed + 0.1;
				if (movementSpeed >= 16.0 && movementSpeed < 1000.0) movementSpeed = movementSpeed + 1.0;
			}
			if (analogData.leftX < -30072.0) {
				if (movementSpeed > 0.1 && movementSpeed < 16.0) movementSpeed = movementSpeed - 0.1;
				if (movementSpeed >= 16.0) movementSpeed = movementSpeed - 1.0;
			}
		}
	}
}