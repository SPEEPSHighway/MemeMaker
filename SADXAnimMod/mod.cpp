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
	void annoyingWriteCallMass();
	bool checkFontSize();
	int dtest(task* tp);

	//Perfect Chaos: PerfectChaos_AnimList
	void doSpeedModifier();
	bool noDefaultDebugMove();
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
		annoyingWriteCallMass();
		WriteJump((void*)0x49AA1E, (void*)0x49AA2C);


		initFaceTableBackup();
	}

	bool noDefaultDebugMove() {
		return false;
	}

	void annoyingWriteCallMass() {
		WriteCall((void*)0x40195D, dtest);
		WriteCall((void*)0x40C0EE, dtest);
		WriteCall((void*)0x46AE6A, dtest);
		WriteCall((void*)0x46B13A, dtest);
		WriteCall((void*)0x46B1D1, dtest);
		WriteCall((void*)0x49D347, dtest);
		WriteCall((void*)0x49D397, dtest);
		WriteCall((void*)0x49D416, dtest);
		WriteCall((void*)0x49D467, dtest);
		WriteCall((void*)0x49D87D, dtest);
		WriteCall((void*)0x49D9F1, dtest);
		WriteCall((void*)0x49DBA7, dtest);
		WriteCall((void*)0x49DD03, dtest);
		WriteCall((void*)0x49DE36, dtest);
		WriteCall((void*)0x49DEB6, dtest);
		WriteCall((void*)0x49DF36, dtest);
		WriteCall((void*)0x49DFB6, dtest);
		WriteCall((void*)0x49E036, dtest);
		WriteCall((void*)0x49E1AD, dtest);
		WriteCall((void*)0x4A2583, dtest);
		WriteCall((void*)0x4A2B07, dtest);
		WriteCall((void*)0x4A670D, dtest);
		WriteCall((void*)0x4A7D2B, dtest);
		WriteCall((void*)0x4AB7CB, dtest);
		WriteCall((void*)0x4AE9B8, dtest);
		WriteCall((void*)0x4AEB78, dtest);
		WriteCall((void*)0x4AFFC6, dtest);
		WriteCall((void*)0x4B0118, dtest);
		WriteCall((void*)0x4B02D8, dtest);
		WriteCall((void*)0x4B2A68, dtest);
		WriteCall((void*)0x4B4980, dtest);
		WriteCall((void*)0x4B8D2B, dtest);
		WriteCall((void*)0x4B8DCE, dtest);
		WriteCall((void*)0x4BF654, dtest);
		WriteCall((void*)0x4C082C, dtest);
		WriteCall((void*)0x4C8FBB, dtest);
		WriteCall((void*)0x4C90E5, dtest);
		WriteCall((void*)0x4CA2B5, dtest);
		WriteCall((void*)0x4CA3DE, dtest);
		WriteCall((void*)0x4CA53A, dtest);
		WriteCall((void*)0x4CA66A, dtest);
		WriteCall((void*)0x4CA7DA, dtest);
		WriteCall((void*)0x4CA90A, dtest);
		WriteCall((void*)0x4CB9D8, dtest);
		WriteCall((void*)0x4CBACB, dtest);
		WriteCall((void*)0x4CBC7D, dtest);
		WriteCall((void*)0x4D451C, dtest);
		WriteCall((void*)0x4D4ABE, dtest);
		WriteCall((void*)0x4DBD20, dtest);
		WriteCall((void*)0x4DBFC1, dtest);
		WriteCall((void*)0x4DC42D, dtest);
		WriteCall((void*)0x4DF438, dtest);
		WriteCall((void*)0x4DF44C, dtest);
		WriteCall((void*)0x4DF803, dtest);
		WriteCall((void*)0x4DF856, dtest);
		WriteCall((void*)0x4DFF7C, dtest);
		WriteCall((void*)0x4DFF92, dtest);
		WriteCall((void*)0x4E0017, dtest);
		WriteCall((void*)0x4E06C1, dtest);
		WriteCall((void*)0x4E13A5, dtest);
		WriteCall((void*)0x4E1410, dtest);
		WriteCall((void*)0x4E177F, dtest);
		WriteCall((void*)0x4E1AF4, dtest);
		WriteCall((void*)0x4E1B60, dtest);
		WriteCall((void*)0x4E1BA0, dtest);
		WriteCall((void*)0x4E1BE0, dtest);
		WriteCall((void*)0x4E1C20, dtest);
		WriteCall((void*)0x4E1C80, dtest);
		WriteCall((void*)0x4E1CC0, dtest);
		WriteCall((void*)0x4E1FD4, dtest);
		WriteCall((void*)0x4E2050, dtest);
		WriteCall((void*)0x4E2090, dtest);
		WriteCall((void*)0x4E262E, dtest);
		WriteCall((void*)0x4E28B9, dtest);
		WriteCall((void*)0x4E2997, dtest);
		WriteCall((void*)0x4E2ACC, dtest);
		WriteCall((void*)0x4E2BE2, dtest);
		WriteCall((void*)0x4E2BF8, dtest);
		WriteCall((void*)0x4E2C50, dtest);
		WriteCall((void*)0x4E2C90, dtest);
		WriteCall((void*)0x4E2D35, dtest);
		WriteCall((void*)0x4E33F2, dtest);
		WriteCall((void*)0x4E3408, dtest);
		WriteCall((void*)0x4E3D5A, dtest);
		WriteCall((void*)0x4E4030, dtest);
		WriteCall((void*)0x4E4D21, dtest);
		WriteCall((void*)0x4E527F, dtest);
		WriteCall((void*)0x4E557C, dtest);
		WriteCall((void*)0x4E5701, dtest);
		WriteCall((void*)0x4E5853, dtest);
		WriteCall((void*)0x4E7F5F, dtest);
		WriteCall((void*)0x4EA289, dtest);
		WriteCall((void*)0x4EB2CA, dtest);
		WriteCall((void*)0x4EB730, dtest);
		WriteCall((void*)0x4EBE04, dtest);
		WriteCall((void*)0x4EBED4, dtest);
		WriteCall((void*)0x4EBFEF, dtest);
		WriteCall((void*)0x4EC06E, dtest);
		WriteCall((void*)0x4ED028, dtest);
		WriteCall((void*)0x4ED03C, dtest);
		WriteCall((void*)0x4ED1DD, dtest);
		WriteCall((void*)0x4ED20A, dtest);
		WriteCall((void*)0x4EDA6D, dtest);
		WriteCall((void*)0x4EDE11, dtest);
		WriteCall((void*)0x4EDF29, dtest);
		WriteCall((void*)0x4EE77E, dtest);
		WriteCall((void*)0x4EEA15, dtest);
		WriteCall((void*)0x4EEA66, dtest);
		WriteCall((void*)0x4EF1E8, dtest);
		WriteCall((void*)0x4EF2B3, dtest);
		WriteCall((void*)0x4EF6C5, dtest);
		WriteCall((void*)0x4EFC4C, dtest);
		WriteCall((void*)0x4F0527, dtest);
		WriteCall((void*)0x4F0572, dtest);
		WriteCall((void*)0x4F0DF8, dtest);
		WriteCall((void*)0x4F1115, dtest);
		WriteCall((void*)0x4F1C02, dtest);
		WriteCall((void*)0x4F1C42, dtest);
		WriteCall((void*)0x4F26EA, dtest);
		WriteCall((void*)0x4F2D82, dtest);
		WriteCall((void*)0x4F2DC2, dtest);
		WriteCall((void*)0x4F385A, dtest);
		WriteCall((void*)0x4F3F04, dtest);
		WriteCall((void*)0x4F3FD2, dtest);
		WriteCall((void*)0x4F401A, dtest);
		WriteCall((void*)0x4F4728, dtest);
		WriteCall((void*)0x4F4788, dtest);
		WriteCall((void*)0x4F483F, dtest);
		WriteCall((void*)0x4F4C3E, dtest);
		WriteCall((void*)0x4F4D9E, dtest);
		WriteCall((void*)0x4F5737, dtest);
		WriteCall((void*)0x4F57C8, dtest);
		WriteCall((void*)0x4F9FFE, dtest);
		WriteCall((void*)0x4FA012, dtest);
		WriteCall((void*)0x4FA344, dtest);
		WriteCall((void*)0x4FA358, dtest);
		WriteCall((void*)0x4FA4C1, dtest);
		WriteCall((void*)0x4FA856, dtest);
		WriteCall((void*)0x4FACF4, dtest);
		WriteCall((void*)0x4FAD31, dtest);
		WriteCall((void*)0x4FADE9, dtest);
		WriteCall((void*)0x4FAE75, dtest);
		WriteCall((void*)0x4FAEBB, dtest);
		WriteCall((void*)0x4FB027, dtest);
		WriteCall((void*)0x4FB061, dtest);
		WriteCall((void*)0x4FB137, dtest);
		WriteCall((void*)0x4FB166, dtest);
		WriteCall((void*)0x4FB1DE, dtest);
		WriteCall((void*)0x4FB5BF, dtest);
		WriteCall((void*)0x4FB607, dtest);
		WriteCall((void*)0x4FB7A0, dtest);
		WriteCall((void*)0x4FB810, dtest);
		WriteCall((void*)0x4FBA1C, dtest);
		WriteCall((void*)0x4FBBA3, dtest);
		WriteCall((void*)0x4FBDD3, dtest);
		WriteCall((void*)0x4FC4FC, dtest);
		WriteCall((void*)0x4FC58C, dtest);
		WriteCall((void*)0x4FC9F7, dtest);
		WriteCall((void*)0x4FD0BF, dtest);
		WriteCall((void*)0x4FD7CF, dtest);
		WriteCall((void*)0x4FDA34, dtest);
		WriteCall((void*)0x4FDA53, dtest);
		WriteCall((void*)0x4FDAB8, dtest);
		WriteCall((void*)0x4FE022, dtest);
		WriteCall((void*)0x4FE808, dtest);
		WriteCall((void*)0x4FED53, dtest);
		WriteCall((void*)0x4FF0A3, dtest);
		WriteCall((void*)0x4FF4DE, dtest);
		WriteCall((void*)0x4FF622, dtest);
		WriteCall((void*)0x4FFB91, dtest);
		WriteCall((void*)0x4FFC61, dtest);
		WriteCall((void*)0x4FFDD0, dtest);
		WriteCall((void*)0x4FFE10, dtest);
		WriteCall((void*)0x4FFE50, dtest);
		WriteCall((void*)0x4FFE90, dtest);
		WriteCall((void*)0x500249, dtest);
		WriteCall((void*)0x50026E, dtest);
		WriteCall((void*)0x500796, dtest);
		WriteCall((void*)0x500BBA, dtest);
		WriteCall((void*)0x500BFC, dtest);
		WriteCall((void*)0x500EF1, dtest);
		WriteCall((void*)0x500F1B, dtest);
		WriteCall((void*)0x500F81, dtest);
		WriteCall((void*)0x500FB3, dtest);
		WriteCall((void*)0x501021, dtest);
		WriteCall((void*)0x501053, dtest);
		WriteCall((void*)0x5010C1, dtest);
		WriteCall((void*)0x5010F3, dtest);
		WriteCall((void*)0x501C45, dtest);
		WriteCall((void*)0x5020E7, dtest);
		WriteCall((void*)0x502EAD, dtest);
		WriteCall((void*)0x51C897, dtest);
		WriteCall((void*)0x51C9AE, dtest);
		WriteCall((void*)0x51CB77, dtest);
		WriteCall((void*)0x51CC3E, dtest);
		WriteCall((void*)0x51CFCC, dtest);
		WriteCall((void*)0x51D00C, dtest);
		WriteCall((void*)0x51D43E, dtest);
		WriteCall((void*)0x51D6B0, dtest);
		WriteCall((void*)0x51D77C, dtest);
		WriteCall((void*)0x51D7B1, dtest);
		WriteCall((void*)0x51D877, dtest);
		WriteCall((void*)0x51D899, dtest);
		WriteCall((void*)0x51D965, dtest);
		WriteCall((void*)0x51E18A, dtest);
		WriteCall((void*)0x51E33C, dtest);
		WriteCall((void*)0x51E4C8, dtest);
		WriteCall((void*)0x51E4FD, dtest);
		WriteCall((void*)0x51E6E7, dtest);
		WriteCall((void*)0x51E763, dtest);
		WriteCall((void*)0x51E93F, dtest);
		WriteCall((void*)0x51E9A9, dtest);
		WriteCall((void*)0x51EAAC, dtest);
		WriteCall((void*)0x51EB5A, dtest);
		WriteCall((void*)0x51EF4A, dtest);
		WriteCall((void*)0x51F56A, dtest);
		WriteCall((void*)0x51F6DA, dtest);
		WriteCall((void*)0x51FA17, dtest);
		WriteCall((void*)0x51FBE7, dtest);
		WriteCall((void*)0x51FE8A, dtest);
		WriteCall((void*)0x51FFCC, dtest);
		WriteCall((void*)0x520098, dtest);
		WriteCall((void*)0x520272, dtest);
		WriteCall((void*)0x521B3E, dtest);
		WriteCall((void*)0x5228AA, dtest);
		WriteCall((void*)0x522A99, dtest);
		WriteCall((void*)0x522ACE, dtest);
		WriteCall((void*)0x522E88, dtest);
		WriteCall((void*)0x522F6A, dtest);
		WriteCall((void*)0x523173, dtest);
		WriteCall((void*)0x52319E, dtest);
		WriteCall((void*)0x5231A4, dtest);
		WriteCall((void*)0x523488, dtest);
		WriteCall((void*)0x523668, dtest);
		WriteCall((void*)0x523EFE, dtest);
		WriteCall((void*)0x52416D, dtest);
		WriteCall((void*)0x5242D0, dtest);
		WriteCall((void*)0x5245DE, dtest);
		WriteCall((void*)0x52468B, dtest);
		WriteCall((void*)0x524727, dtest);
		WriteCall((void*)0x524780, dtest);
		WriteCall((void*)0x5247AD, dtest);
		WriteCall((void*)0x52488A, dtest);
		WriteCall((void*)0x5248DF, dtest);
		WriteCall((void*)0x524E51, dtest);
		WriteCall((void*)0x524E90, dtest);
		WriteCall((void*)0x52507C, dtest);
		WriteCall((void*)0x526175, dtest);
		WriteCall((void*)0x526666, dtest);
		WriteCall((void*)0x52679D, dtest);
		WriteCall((void*)0x5267D2, dtest);
		WriteCall((void*)0x5268FF, dtest);
		WriteCall((void*)0x52694A, dtest);
		WriteCall((void*)0x526A94, dtest);
		WriteCall((void*)0x526ACF, dtest);
		WriteCall((void*)0x526B83, dtest);
		WriteCall((void*)0x528A06, dtest);
		WriteCall((void*)0x528E37, dtest);
		WriteCall((void*)0x52B13C, dtest);
		WriteCall((void*)0x52B1F6, dtest);
		WriteCall((void*)0x52B60D, dtest);
		WriteCall((void*)0x52B738, dtest);
		WriteCall((void*)0x52BA00, dtest);
		WriteCall((void*)0x52C728, dtest);
		WriteCall((void*)0x52CB82, dtest);
		WriteCall((void*)0x52D524, dtest);
		WriteCall((void*)0x52D72C, dtest);
		WriteCall((void*)0x5323B1, dtest);
		WriteCall((void*)0x53244B, dtest);
		WriteCall((void*)0x53365A, dtest);
		WriteCall((void*)0x53388A, dtest);
		WriteCall((void*)0x5340BA, dtest);
		WriteCall((void*)0x5341EA, dtest);
		WriteCall((void*)0x53433A, dtest);
		WriteCall((void*)0x53496A, dtest);
		WriteCall((void*)0x534C3A, dtest);
		WriteCall((void*)0x53508A, dtest);
		WriteCall((void*)0x535474, dtest);
		WriteCall((void*)0x535526, dtest);
		WriteCall((void*)0x535A46, dtest);
		WriteCall((void*)0x535ACD, dtest);
		WriteCall((void*)0x535F56, dtest);
		WriteCall((void*)0x53631E, dtest);
		WriteCall((void*)0x536A9E, dtest);
		WriteCall((void*)0x536CCE, dtest);
		WriteCall((void*)0x536EEB, dtest);
		WriteCall((void*)0x53760B, dtest);
		WriteCall((void*)0x53781B, dtest);
		WriteCall((void*)0x537FCB, dtest);
		WriteCall((void*)0x538627, dtest);
		WriteCall((void*)0x538747, dtest);
		WriteCall((void*)0x538AD8, dtest);
		WriteCall((void*)0x538FDA, dtest);
		WriteCall((void*)0x539115, dtest);
		WriteCall((void*)0x53950C, dtest);
		WriteCall((void*)0x539BCC, dtest);
		WriteCall((void*)0x53A6C7, dtest);
		WriteCall((void*)0x53AFCC, dtest);
		WriteCall((void*)0x53B0ED, dtest);
		WriteCall((void*)0x53BBAC, dtest);
		WriteCall((void*)0x53C3FC, dtest);
		WriteCall((void*)0x53C67C, dtest);
		WriteCall((void*)0x53CA80, dtest);
		WriteCall((void*)0x53CF7C, dtest);
		WriteCall((void*)0x53D1D0, dtest);
		WriteCall((void*)0x53D71C, dtest);
		WriteCall((void*)0x53DEA1, dtest);
		WriteCall((void*)0x53E45C, dtest);
		WriteCall((void*)0x53E9B0, dtest);
		WriteCall((void*)0x53EC4C, dtest);
		WriteCall((void*)0x53F026, dtest);
		WriteCall((void*)0x53F4EC, dtest);
		WriteCall((void*)0x540750, dtest);
		WriteCall((void*)0x540B4C, dtest);
		WriteCall((void*)0x540EFC, dtest);
		WriteCall((void*)0x541400, dtest);
		WriteCall((void*)0x5431E6, dtest);
		WriteCall((void*)0x54357B, dtest);
		WriteCall((void*)0x54392E, dtest);
		WriteCall((void*)0x54491C, dtest);
		WriteCall((void*)0x5450BB, dtest);
		WriteCall((void*)0x54518B, dtest);
		WriteCall((void*)0x5455DA, dtest);
		WriteCall((void*)0x54568C, dtest);
		WriteCall((void*)0x5458EB, dtest);
		WriteCall((void*)0x548E43, dtest);
		WriteCall((void*)0x5499AB, dtest);
		WriteCall((void*)0x54E1EE, dtest);
		WriteCall((void*)0x54E368, dtest);
		WriteCall((void*)0x55A949, dtest);
		WriteCall((void*)0x55D27C, dtest);
		WriteCall((void*)0x563F06, dtest);
		WriteCall((void*)0x564724, dtest);
		WriteCall((void*)0x5648BF, dtest);
		WriteCall((void*)0x564A5A, dtest);
		WriteCall((void*)0x564AE7, dtest);
		WriteCall((void*)0x5652C1, dtest);
		WriteCall((void*)0x5657CA, dtest);
		WriteCall((void*)0x565B04, dtest);
		WriteCall((void*)0x595861, dtest);
		WriteCall((void*)0x598064, dtest);
		WriteCall((void*)0x598078, dtest);
		WriteCall((void*)0x5980E6, dtest);
		WriteCall((void*)0x5981B1, dtest);
		WriteCall((void*)0x5983D1, dtest);
		WriteCall((void*)0x5986FB, dtest);
		WriteCall((void*)0x59898F, dtest);
		WriteCall((void*)0x598A32, dtest);
		WriteCall((void*)0x598E9F, dtest);
		WriteCall((void*)0x598F9E, dtest);
		WriteCall((void*)0x59AB20, dtest);
		WriteCall((void*)0x59B2A7, dtest);
		WriteCall((void*)0x59B94F, dtest);
		WriteCall((void*)0x59BA6A, dtest);
		WriteCall((void*)0x59BAF3, dtest);
		WriteCall((void*)0x59BDBB, dtest);
		WriteCall((void*)0x59BED7, dtest);
		WriteCall((void*)0x59BFDF, dtest);
		WriteCall((void*)0x59C1B7, dtest);
		WriteCall((void*)0x59C326, dtest);
		WriteCall((void*)0x59C398, dtest);
		WriteCall((void*)0x59C48C, dtest);
		WriteCall((void*)0x59C573, dtest);
		WriteCall((void*)0x59C70C, dtest);
		WriteCall((void*)0x59C861, dtest);
		WriteCall((void*)0x59CB6D, dtest);
		WriteCall((void*)0x59CE53, dtest);
		WriteCall((void*)0x59CFD0, dtest);
		WriteCall((void*)0x59D0F0, dtest);
		WriteCall((void*)0x59D210, dtest);
		WriteCall((void*)0x59D370, dtest);
		WriteCall((void*)0x59D480, dtest);
		WriteCall((void*)0x59D5E0, dtest);
		WriteCall((void*)0x59D81A, dtest);
		WriteCall((void*)0x59D84A, dtest);
		WriteCall((void*)0x59D99A, dtest);
		WriteCall((void*)0x59DC4C, dtest);
		WriteCall((void*)0x59DF18, dtest);
		WriteCall((void*)0x59E190, dtest);
		WriteCall((void*)0x59E450, dtest);
		WriteCall((void*)0x59E680, dtest);
		WriteCall((void*)0x59E95C, dtest);
		WriteCall((void*)0x59EB1C, dtest);
		WriteCall((void*)0x59EC10, dtest);
		WriteCall((void*)0x59ED70, dtest);
		WriteCall((void*)0x59EEC0, dtest);
		WriteCall((void*)0x59F000, dtest);
		WriteCall((void*)0x59F252, dtest);
		WriteCall((void*)0x59F4F5, dtest);
		WriteCall((void*)0x59F640, dtest);
		WriteCall((void*)0x59F790, dtest);
		WriteCall((void*)0x59FA60, dtest);
		WriteCall((void*)0x59FD70, dtest);
		WriteCall((void*)0x59FF50, dtest);
		WriteCall((void*)0x5A0250, dtest);
		WriteCall((void*)0x5A0560, dtest);
		WriteCall((void*)0x5A0740, dtest);
		WriteCall((void*)0x5A08B0, dtest);
		WriteCall((void*)0x5A09F0, dtest);
		WriteCall((void*)0x5A0B30, dtest);
		WriteCall((void*)0x5A0C70, dtest);
		WriteCall((void*)0x5A16D1, dtest);
		WriteCall((void*)0x5A1D90, dtest);
		WriteCall((void*)0x5A1F7D, dtest);
		WriteCall((void*)0x5A215B, dtest);
		WriteCall((void*)0x5A2270, dtest);
		WriteCall((void*)0x5A24B1, dtest);
		WriteCall((void*)0x5A296D, dtest);
		WriteCall((void*)0x5A2DE0, dtest);
		WriteCall((void*)0x5A2F30, dtest);
		WriteCall((void*)0x5A3050, dtest);
		WriteCall((void*)0x5A3230, dtest);
		WriteCall((void*)0x5A33F0, dtest);
		WriteCall((void*)0x5A499B, dtest);
		WriteCall((void*)0x5A4DA9, dtest);
		WriteCall((void*)0x5A5121, dtest);
		WriteCall((void*)0x5A56AF, dtest);
		WriteCall((void*)0x5A575F, dtest);
		WriteCall((void*)0x5A589A, dtest);
		WriteCall((void*)0x5A5B8C, dtest);
		WriteCall((void*)0x5A6820, dtest);
		WriteCall((void*)0x5A6BCC, dtest);
		WriteCall((void*)0x5A6ED0, dtest);
		WriteCall((void*)0x5A70DC, dtest);
		WriteCall((void*)0x5A7B50, dtest);
		WriteCall((void*)0x5A87F0, dtest);
		WriteCall((void*)0x5A8CB3, dtest);
		WriteCall((void*)0x5A8EA9, dtest);
		WriteCall((void*)0x5A9020, dtest);
		WriteCall((void*)0x5A9149, dtest);
		WriteCall((void*)0x5A99AF, dtest);
		WriteCall((void*)0x5A9ADD, dtest);
		WriteCall((void*)0x5AFFB6, dtest);
		WriteCall((void*)0x5B050C, dtest);
		WriteCall((void*)0x5B077C, dtest);
		WriteCall((void*)0x5B0BBD, dtest);
		WriteCall((void*)0x5B2431, dtest);
		WriteCall((void*)0x5B2497, dtest);
		WriteCall((void*)0x5B256C, dtest);
		WriteCall((void*)0x5B2580, dtest);
		WriteCall((void*)0x5B26A0, dtest);
		WriteCall((void*)0x5B27D0, dtest);
		WriteCall((void*)0x5B2EE5, dtest);
		WriteCall((void*)0x5B3578, dtest);
		WriteCall((void*)0x5B403A, dtest);
		WriteCall((void*)0x5B420B, dtest);
		WriteCall((void*)0x5B4288, dtest);
		WriteCall((void*)0x5B42E7, dtest);
		WriteCall((void*)0x5B45FA, dtest);
		WriteCall((void*)0x5B4622, dtest);
		WriteCall((void*)0x5B4860, dtest);
		WriteCall((void*)0x5B4B50, dtest);
		WriteCall((void*)0x5B4D2A, dtest);
		WriteCall((void*)0x5B520A, dtest);
		WriteCall((void*)0x5B5430, dtest);
		WriteCall((void*)0x5B6289, dtest);
		WriteCall((void*)0x5B64D1, dtest);
		WriteCall((void*)0x5B7231, dtest);
		WriteCall((void*)0x5B761C, dtest);
		WriteCall((void*)0x5B78C0, dtest);
		WriteCall((void*)0x5B7C0B, dtest);
		WriteCall((void*)0x5B7D90, dtest);
		WriteCall((void*)0x5B7ECB, dtest);
		WriteCall((void*)0x5B8615, dtest);
		WriteCall((void*)0x5B8646, dtest);
		WriteCall((void*)0x5B8F9C, dtest);
		WriteCall((void*)0x5B9057, dtest);
		WriteCall((void*)0x5B97F4, dtest);
		WriteCall((void*)0x5B9830, dtest);
		WriteCall((void*)0x5B9C5C, dtest);
		WriteCall((void*)0x5BA040, dtest);
		WriteCall((void*)0x5BA4B0, dtest);
		WriteCall((void*)0x5BA920, dtest);
		WriteCall((void*)0x5BAD10, dtest);
		WriteCall((void*)0x5BB180, dtest);
		WriteCall((void*)0x5BB5F0, dtest);
		WriteCall((void*)0x5BBA60, dtest);
		WriteCall((void*)0x5BBDDC, dtest);
		WriteCall((void*)0x5BC0BC, dtest);
		WriteCall((void*)0x5BC21C, dtest);
		WriteCall((void*)0x5BC50C, dtest);
		WriteCall((void*)0x5BC77C, dtest);
		WriteCall((void*)0x5BCA9C, dtest);
		WriteCall((void*)0x5BCDCC, dtest);
		WriteCall((void*)0x5BD12C, dtest);
		WriteCall((void*)0x5BD32C, dtest);
		WriteCall((void*)0x5BD40C, dtest);
		WriteCall((void*)0x5BD762, dtest);
		WriteCall((void*)0x5BD89C, dtest);
		WriteCall((void*)0x5BE67C, dtest);
		WriteCall((void*)0x5BE89C, dtest);
		WriteCall((void*)0x5BE9D0, dtest);
		WriteCall((void*)0x5BEBBC, dtest);
		WriteCall((void*)0x5BEDB0, dtest);
		WriteCall((void*)0x5BEFC0, dtest);
		WriteCall((void*)0x5BF9BC, dtest);
		WriteCall((void*)0x5C027C, dtest);
		WriteCall((void*)0x5C35D0, dtest);
		WriteCall((void*)0x5C36BE, dtest);
		WriteCall((void*)0x5C373F, dtest);
		WriteCall((void*)0x5C382C, dtest);
		WriteCall((void*)0x5C38A3, dtest);
		WriteCall((void*)0x5C392C, dtest);
		WriteCall((void*)0x5C4115, dtest);
		WriteCall((void*)0x5C4409, dtest);
		WriteCall((void*)0x5C453C, dtest);
		WriteCall((void*)0x5C47EA, dtest);
		WriteCall((void*)0x5C4844, dtest);
		WriteCall((void*)0x5C4FD3, dtest);
		WriteCall((void*)0x5C5423, dtest);
		WriteCall((void*)0x5C570E, dtest);
		WriteCall((void*)0x5C57C9, dtest);
		WriteCall((void*)0x5C5830, dtest);
		WriteCall((void*)0x5C5977, dtest);
		WriteCall((void*)0x5C5EAA, dtest);
		WriteCall((void*)0x5C63F1, dtest);
		WriteCall((void*)0x5C699A, dtest);
		WriteCall((void*)0x5C6EE1, dtest);
		WriteCall((void*)0x5C748A, dtest);
		WriteCall((void*)0x5C79DE, dtest);
		WriteCall((void*)0x5C803A, dtest);
		WriteCall((void*)0x5C8581, dtest);
		WriteCall((void*)0x5C8730, dtest);
		WriteCall((void*)0x5C8B63, dtest);
		WriteCall((void*)0x5C9344, dtest);
		WriteCall((void*)0x5C95DF, dtest);
		WriteCall((void*)0x5C9871, dtest);
		WriteCall((void*)0x5C9B06, dtest);
		WriteCall((void*)0x5C9D26, dtest);
		WriteCall((void*)0x5C9F65, dtest);
		WriteCall((void*)0x5CA0EB, dtest);
		WriteCall((void*)0x5CA3F1, dtest);
		WriteCall((void*)0x5CA4C1, dtest);
		WriteCall((void*)0x5CA69E, dtest);
		WriteCall((void*)0x5CA71E, dtest);
		WriteCall((void*)0x5CA7A0, dtest);
		WriteCall((void*)0x5CA86E, dtest);
		WriteCall((void*)0x5CA8D6, dtest);
		WriteCall((void*)0x5CA982, dtest);
		WriteCall((void*)0x5CA9E9, dtest);
		WriteCall((void*)0x5CAC7B, dtest);
		WriteCall((void*)0x5CAD01, dtest);
		WriteCall((void*)0x5CB173, dtest);
		WriteCall((void*)0x5CB2B3, dtest);
		WriteCall((void*)0x5CB471, dtest);
		WriteCall((void*)0x5CBB90, dtest);
		WriteCall((void*)0x5CC62A, dtest);
		WriteCall((void*)0x5CCA6B, dtest);
		WriteCall((void*)0x5CCB11, dtest);
		WriteCall((void*)0x5CCD2B, dtest);
		WriteCall((void*)0x5CCDC1, dtest);
		WriteCall((void*)0x5CCF17, dtest);
		WriteCall((void*)0x5CCF84, dtest);
		WriteCall((void*)0x5CD162, dtest);
		WriteCall((void*)0x5CD25F, dtest);
		WriteCall((void*)0x5CD580, dtest);
		WriteCall((void*)0x5CD9D9, dtest);
		WriteCall((void*)0x5CDA89, dtest);
		WriteCall((void*)0x5CDB84, dtest);
		WriteCall((void*)0x5CDC11, dtest);
		WriteCall((void*)0x5CDEA9, dtest);
		WriteCall((void*)0x5CE1FA, dtest);
		WriteCall((void*)0x5CE419, dtest);
		WriteCall((void*)0x5CE52A, dtest);
		WriteCall((void*)0x5CE69F, dtest);
		WriteCall((void*)0x5CE6F7, dtest);
		WriteCall((void*)0x5CE8D7, dtest);
		WriteCall((void*)0x5CE957, dtest);
		WriteCall((void*)0x5CEAF0, dtest);
		WriteCall((void*)0x5CEB7A, dtest);
		WriteCall((void*)0x5CED2B, dtest);
		WriteCall((void*)0x5CED7F, dtest);
		WriteCall((void*)0x5CEEA2, dtest);
		WriteCall((void*)0x5CEED5, dtest);
		WriteCall((void*)0x5CEF5B, dtest);
		WriteCall((void*)0x5CF114, dtest);
		WriteCall((void*)0x5CF5DF, dtest);
		WriteCall((void*)0x5CF65A, dtest);
		WriteCall((void*)0x5CF69E, dtest);
		WriteCall((void*)0x5CFA2F, dtest);
		WriteCall((void*)0x5D042A, dtest);
		WriteCall((void*)0x5D05F6, dtest);
		WriteCall((void*)0x5D06B7, dtest);
		WriteCall((void*)0x5D0726, dtest);
		WriteCall((void*)0x5D0847, dtest);
		WriteCall((void*)0x5D0A4A, dtest);
		WriteCall((void*)0x5D0BBC, dtest);
		WriteCall((void*)0x5D10E9, dtest);
		WriteCall((void*)0x5D146C, dtest);
		WriteCall((void*)0x5D1A21, dtest);
		WriteCall((void*)0x5D2225, dtest);
		WriteCall((void*)0x5D25FE, dtest);
		WriteCall((void*)0x5D27BB, dtest);
		WriteCall((void*)0x5D2B59, dtest);
		WriteCall((void*)0x5D2D6D, dtest);
		WriteCall((void*)0x5D3099, dtest);
		WriteCall((void*)0x5D336A, dtest);
		WriteCall((void*)0x5D3659, dtest);
		WriteCall((void*)0x5D392A, dtest);
		WriteCall((void*)0x5D39DC, dtest);
		WriteCall((void*)0x5D3A3C, dtest);
		WriteCall((void*)0x5D3B27, dtest);
		WriteCall((void*)0x5D3C3A, dtest);
		WriteCall((void*)0x5D3D75, dtest);
		WriteCall((void*)0x5D3DBA, dtest);
		WriteCall((void*)0x5D3FE5, dtest);
		WriteCall((void*)0x5D416B, dtest);
		WriteCall((void*)0x5D48D7, dtest);
		WriteCall((void*)0x5D4A67, dtest);
		WriteCall((void*)0x5D4BB7, dtest);
		WriteCall((void*)0x5D4CEA, dtest);
		WriteCall((void*)0x5D512E, dtest);
		WriteCall((void*)0x5DBA39, dtest);
		WriteCall((void*)0x5DBBA9, dtest);
		WriteCall((void*)0x5DD128, dtest);
		WriteCall((void*)0x5DD13C, dtest);
		WriteCall((void*)0x5DDC51, dtest);
		WriteCall((void*)0x5DDD6C, dtest);
		WriteCall((void*)0x5DDEC1, dtest);
		WriteCall((void*)0x5DDF2A, dtest);
		WriteCall((void*)0x5DE011, dtest);
		WriteCall((void*)0x5DE05A, dtest);
		WriteCall((void*)0x5DE132, dtest);
		WriteCall((void*)0x5DE1BB, dtest);
		WriteCall((void*)0x5DE2B1, dtest);
		WriteCall((void*)0x5DE489, dtest);
		WriteCall((void*)0x5DE61C, dtest);
		WriteCall((void*)0x5DE7E2, dtest);
		WriteCall((void*)0x5DEA02, dtest);
		WriteCall((void*)0x5DEB12, dtest);
		WriteCall((void*)0x5DEDC6, dtest);
		WriteCall((void*)0x5DEF42, dtest);
		WriteCall((void*)0x5DF007, dtest);
		WriteCall((void*)0x5DF1B2, dtest);
		WriteCall((void*)0x5E2640, dtest);
		WriteCall((void*)0x5E265F, dtest);
		WriteCall((void*)0x5E2B33, dtest);
		WriteCall((void*)0x5E2BBF, dtest);
		WriteCall((void*)0x5E2BE4, dtest);
		WriteCall((void*)0x5E3255, dtest);
		WriteCall((void*)0x5E32B5, dtest);
		WriteCall((void*)0x5E3337, dtest);
		WriteCall((void*)0x5E40A0, dtest);
		WriteCall((void*)0x5E61C7, dtest);
		WriteCall((void*)0x5E62ED, dtest);
		WriteCall((void*)0x5E64FC, dtest);
		WriteCall((void*)0x5E6AE7, dtest);
		WriteCall((void*)0x5E6B8B, dtest);
		WriteCall((void*)0x5E723F, dtest);
		WriteCall((void*)0x5E725A, dtest);
		WriteCall((void*)0x5E74BA, dtest);
		WriteCall((void*)0x5E74D7, dtest);
		WriteCall((void*)0x5E767E, dtest);
		WriteCall((void*)0x5E7973, dtest);
		WriteCall((void*)0x5E7B81, dtest);
		WriteCall((void*)0x5E8146, dtest);
		WriteCall((void*)0x5E8338, dtest);
		WriteCall((void*)0x5E84F9, dtest);
		WriteCall((void*)0x5E8E38, dtest);
		WriteCall((void*)0x5E9496, dtest);
		WriteCall((void*)0x5E9815, dtest);
		WriteCall((void*)0x5E9985, dtest);
		WriteCall((void*)0x5E9ACD, dtest);
		WriteCall((void*)0x5E9D56, dtest);
		WriteCall((void*)0x5EA022, dtest);
		WriteCall((void*)0x5EA5BD, dtest);
		WriteCall((void*)0x5EAA52, dtest);
		WriteCall((void*)0x5EAC0B, dtest);
		WriteCall((void*)0x5EAF5B, dtest);
		WriteCall((void*)0x5EB6CB, dtest);
		WriteCall((void*)0x5EB72A, dtest);
		WriteCall((void*)0x5EC378, dtest);
		WriteCall((void*)0x5EC3A6, dtest);
		WriteCall((void*)0x5EC46B, dtest);
		WriteCall((void*)0x5EDC78, dtest);
		WriteCall((void*)0x5EDD39, dtest);
		WriteCall((void*)0x5EF9E9, dtest);
		WriteCall((void*)0x5EFA57, dtest);
		WriteCall((void*)0x5EFB0A, dtest);
		WriteCall((void*)0x5F0058, dtest);
		WriteCall((void*)0x5F069F, dtest);
		WriteCall((void*)0x5F09BE, dtest);
		WriteCall((void*)0x5F0A4E, dtest);
		WriteCall((void*)0x5F0B56, dtest);
		WriteCall((void*)0x5F0CF5, dtest);
		WriteCall((void*)0x5F0E86, dtest);
		WriteCall((void*)0x5F0FD7, dtest);
		WriteCall((void*)0x5F13CB, dtest);
		WriteCall((void*)0x5F154A, dtest);
		WriteCall((void*)0x5F240F, dtest);
		WriteCall((void*)0x5F39AC, dtest);
		WriteCall((void*)0x5F401B, dtest);
		WriteCall((void*)0x5F4129, dtest);
		WriteCall((void*)0x5F432B, dtest);
		WriteCall((void*)0x5F46BF, dtest);
		WriteCall((void*)0x5F59AA, dtest);
		WriteCall((void*)0x5F6071, dtest);
		WriteCall((void*)0x5F6686, dtest);
		WriteCall((void*)0x5F7CEE, dtest);
		WriteCall((void*)0x5F84EA, dtest);
		WriteCall((void*)0x5F87B4, dtest);
		WriteCall((void*)0x5F8E0A, dtest);
		WriteCall((void*)0x5F90D3, dtest);
		WriteCall((void*)0x5F971A, dtest);
		WriteCall((void*)0x5F9928, dtest);
		WriteCall((void*)0x5F994B, dtest);
		WriteCall((void*)0x5F9BA8, dtest);
		WriteCall((void*)0x5F9BCB, dtest);
		WriteCall((void*)0x5FA0FC, dtest);
		WriteCall((void*)0x5FA35F, dtest);
		WriteCall((void*)0x5FA708, dtest);
		WriteCall((void*)0x5FA72B, dtest);
		WriteCall((void*)0x5FA9B7, dtest);
		WriteCall((void*)0x5FA9DB, dtest);
		WriteCall((void*)0x5FAEB9, dtest);
		WriteCall((void*)0x5FB13B, dtest);
		WriteCall((void*)0x5FB626, dtest);
		WriteCall((void*)0x5FB89B, dtest);
		WriteCall((void*)0x5FC165, dtest);
		WriteCall((void*)0x5FC5C5, dtest);
		WriteCall((void*)0x5FC7C4, dtest);
		WriteCall((void*)0x5FCC0B, dtest);
		WriteCall((void*)0x5FD3B4, dtest);
		WriteCall((void*)0x5FD7AB, dtest);
		WriteCall((void*)0x5FDC90, dtest);
		WriteCall((void*)0x5FDE96, dtest);
		WriteCall((void*)0x5FDF9A, dtest);
		WriteCall((void*)0x5FDFEA, dtest);
		WriteCall((void*)0x5FF75E, dtest);
		WriteCall((void*)0x5FF9FA, dtest);
		WriteCall((void*)0x5FFE76, dtest);
		WriteCall((void*)0x600B7A, dtest);
		WriteCall((void*)0x6039DA, dtest);
		WriteCall((void*)0x603C9A, dtest);
		WriteCall((void*)0x603F97, dtest);
		WriteCall((void*)0x60400E, dtest);
		WriteCall((void*)0x6042B7, dtest);
		WriteCall((void*)0x605B4F, dtest);
		WriteCall((void*)0x605E08, dtest);
		WriteCall((void*)0x605E58, dtest);
		WriteCall((void*)0x605EA8, dtest);
		WriteCall((void*)0x605F18, dtest);
		WriteCall((void*)0x605F68, dtest);
		WriteCall((void*)0x6062AE, dtest);
		WriteCall((void*)0x606351, dtest);
		WriteCall((void*)0x60670B, dtest);
		WriteCall((void*)0x606727, dtest);
		WriteCall((void*)0x606B3E, dtest);
		WriteCall((void*)0x606E8E, dtest);
		WriteCall((void*)0x606EE0, dtest);
		WriteCall((void*)0x6079B2, dtest);
		WriteCall((void*)0x607AA4, dtest);
		WriteCall((void*)0x607B37, dtest);
		WriteCall((void*)0x607B77, dtest);
		WriteCall((void*)0x607BB3, dtest);
		WriteCall((void*)0x607CBB, dtest);
		WriteCall((void*)0x608605, dtest);
		WriteCall((void*)0x608685, dtest);
		WriteCall((void*)0x608705, dtest);
		WriteCall((void*)0x60874A, dtest);
		WriteCall((void*)0x608778, dtest);
		WriteCall((void*)0x6089FE, dtest);
		WriteCall((void*)0x608A4D, dtest);
		WriteCall((void*)0x608F45, dtest);
		WriteCall((void*)0x609223, dtest);
		WriteCall((void*)0x60936E, dtest);
		WriteCall((void*)0x6094C8, dtest);
		WriteCall((void*)0x609CAE, dtest);
		WriteCall((void*)0x609F1C, dtest);
		WriteCall((void*)0x609FCA, dtest);
		WriteCall((void*)0x60A79E, dtest);
		WriteCall((void*)0x60AA0C, dtest);
		WriteCall((void*)0x60AABA, dtest);
		WriteCall((void*)0x60AB06, dtest);
		WriteCall((void*)0x60AD37, dtest);
		WriteCall((void*)0x60AD65, dtest);
		WriteCall((void*)0x60AE3C, dtest);
		WriteCall((void*)0x60AF9B, dtest);
		WriteCall((void*)0x60B197, dtest);
		WriteCall((void*)0x60C65F, dtest);
		WriteCall((void*)0x60C85D, dtest);
		WriteCall((void*)0x60CB42, dtest);
		WriteCall((void*)0x60CB74, dtest);
		WriteCall((void*)0x60CFD9, dtest);
		WriteCall((void*)0x60D057, dtest);
		WriteCall((void*)0x60D090, dtest);
		WriteCall((void*)0x60D0AC, dtest);
		WriteCall((void*)0x60D512, dtest);
		WriteCall((void*)0x60D841, dtest);
		WriteCall((void*)0x60E172, dtest);
		WriteCall((void*)0x60E1B1, dtest);
		WriteCall((void*)0x60E44B, dtest);
		WriteCall((void*)0x60E501, dtest);
		WriteCall((void*)0x60E6CE, dtest);
		WriteCall((void*)0x60EA87, dtest);
		WriteCall((void*)0x60EE1E, dtest);
		WriteCall((void*)0x61438A, dtest);
		WriteCall((void*)0x6147E3, dtest);
		WriteCall((void*)0x614800, dtest);
		WriteCall((void*)0x614875, dtest);
		WriteCall((void*)0x614885, dtest);
		WriteCall((void*)0x6148CD, dtest);
		WriteCall((void*)0x614901, dtest);
		WriteCall((void*)0x614E91, dtest);
		WriteCall((void*)0x61507B, dtest);
		WriteCall((void*)0x6151DC, dtest);
		WriteCall((void*)0x615221, dtest);
		WriteCall((void*)0x61539D, dtest);
		WriteCall((void*)0x6153DB, dtest);
		WriteCall((void*)0x615420, dtest);
		WriteCall((void*)0x615460, dtest);
		WriteCall((void*)0x6154AB, dtest);
		WriteCall((void*)0x6155FB, dtest);
		WriteCall((void*)0x615791, dtest);
		WriteCall((void*)0x6157E0, dtest);
		WriteCall((void*)0x615841, dtest);
		WriteCall((void*)0x615891, dtest);
		WriteCall((void*)0x6158E1, dtest);
		WriteCall((void*)0x615951, dtest);
		WriteCall((void*)0x6159A1, dtest);
		WriteCall((void*)0x615C30, dtest);
		WriteCall((void*)0x615E77, dtest);
		WriteCall((void*)0x616221, dtest);
		WriteCall((void*)0x6163E0, dtest);
		WriteCall((void*)0x616420, dtest);
		WriteCall((void*)0x616476, dtest);
		WriteCall((void*)0x6164C0, dtest);
		WriteCall((void*)0x6167A7, dtest);
		WriteCall((void*)0x6167F8, dtest);
		WriteCall((void*)0x616C67, dtest);
		WriteCall((void*)0x616CAE, dtest);
		WriteCall((void*)0x617182, dtest);
		WriteCall((void*)0x6171B9, dtest);
		WriteCall((void*)0x6172B2, dtest);
		WriteCall((void*)0x6172E3, dtest);
		WriteCall((void*)0x617352, dtest);
		WriteCall((void*)0x617383, dtest);
		WriteCall((void*)0x6173E1, dtest);
		WriteCall((void*)0x61778A, dtest);
		WriteCall((void*)0x6177D1, dtest);
		WriteCall((void*)0x61782B, dtest);
		WriteCall((void*)0x617CDA, dtest);
		WriteCall((void*)0x617F0A, dtest);
		WriteCall((void*)0x61806E, dtest);
		WriteCall((void*)0x61869A, dtest);
		WriteCall((void*)0x61881C, dtest);
		WriteCall((void*)0x618A7C, dtest);
		WriteCall((void*)0x618ACC, dtest);
		WriteCall((void*)0x61997E, dtest);
		WriteCall((void*)0x619EE9, dtest);
		WriteCall((void*)0x61A34F, dtest);
		WriteCall((void*)0x61A580, dtest);
		WriteCall((void*)0x61AD07, dtest);
		WriteCall((void*)0x61AE93, dtest);
		WriteCall((void*)0x61AF90, dtest);
		WriteCall((void*)0x61B456, dtest);
		WriteCall((void*)0x61BC05, dtest);
		WriteCall((void*)0x61BDE4, dtest);
		WriteCall((void*)0x61BEF4, dtest);
		WriteCall((void*)0x61BFB3, dtest);
		WriteCall((void*)0x61C17A, dtest);
		WriteCall((void*)0x61C8AB, dtest);
		WriteCall((void*)0x61DC9A, dtest);
		WriteCall((void*)0x61E610, dtest);
		WriteCall((void*)0x61E7E0, dtest);
		WriteCall((void*)0x61EF00, dtest);
		WriteCall((void*)0x61F6E1, dtest);
		WriteCall((void*)0x620824, dtest);
		WriteCall((void*)0x620B10, dtest);
		WriteCall((void*)0x621077, dtest);
		WriteCall((void*)0x621690, dtest);
		WriteCall((void*)0x62345D, dtest);
		WriteCall((void*)0x623BBA, dtest);
		WriteCall((void*)0x6240A1, dtest);
		WriteCall((void*)0x624828, dtest);
		WriteCall((void*)0x6268E6, dtest);
		WriteCall((void*)0x62A284, dtest);
		WriteCall((void*)0x62A7B0, dtest);
		WriteCall((void*)0x630E7A, dtest);
		WriteCall((void*)0x630F3D, dtest);
		WriteCall((void*)0x630F7A, dtest);
		WriteCall((void*)0x630FC7, dtest);
		WriteCall((void*)0x631047, dtest);
		WriteCall((void*)0x6310C7, dtest);
		WriteCall((void*)0x631486, dtest);
		WriteCall((void*)0x632038, dtest);
		WriteCall((void*)0x632066, dtest);
		WriteCall((void*)0x6334BA, dtest);
		WriteCall((void*)0x633EF6, dtest);
		WriteCall((void*)0x633F7D, dtest);
		WriteCall((void*)0x634458, dtest);
		WriteCall((void*)0x6347BE, dtest);
		WriteCall((void*)0x63587A, dtest);
		WriteCall((void*)0x635A4B, dtest);
		WriteCall((void*)0x635D6B, dtest);
		WriteCall((void*)0x635F29, dtest);
		WriteCall((void*)0x635FDB, dtest);
		WriteCall((void*)0x636102, dtest);
		WriteCall((void*)0x63614C, dtest);
		WriteCall((void*)0x636235, dtest);
		WriteCall((void*)0x636439, dtest);
		WriteCall((void*)0x63646E, dtest);
		WriteCall((void*)0x636597, dtest);
		WriteCall((void*)0x6365D1, dtest);
		WriteCall((void*)0x636958, dtest);
		WriteCall((void*)0x636B51, dtest);
		WriteCall((void*)0x63707D, dtest);
		WriteCall((void*)0x6370E3, dtest);
		WriteCall((void*)0x63736B, dtest);
		WriteCall((void*)0x63758A, dtest);
		WriteCall((void*)0x63787E, dtest);
		WriteCall((void*)0x637CEB, dtest);
		WriteCall((void*)0x637E47, dtest);
		WriteCall((void*)0x637EEB, dtest);
		WriteCall((void*)0x638002, dtest);
		WriteCall((void*)0x63805B, dtest);
		WriteCall((void*)0x638227, dtest);
		WriteCall((void*)0x638307, dtest);
		WriteCall((void*)0x63838B, dtest);
		WriteCall((void*)0x6387BB, dtest);
		WriteCall((void*)0x638827, dtest);
		WriteCall((void*)0x638957, dtest);
		WriteCall((void*)0x63899A, dtest);
		WriteCall((void*)0x638BA7, dtest);
		WriteCall((void*)0x638C19, dtest);
		WriteCall((void*)0x638FE9, dtest);
		WriteCall((void*)0x639141, dtest);
		WriteCall((void*)0x6391AB, dtest);
		WriteCall((void*)0x639207, dtest);
		WriteCall((void*)0x639315, dtest);
		WriteCall((void*)0x6393A4, dtest);
		WriteCall((void*)0x639415, dtest);
		WriteCall((void*)0x639697, dtest);
		WriteCall((void*)0x639705, dtest);
		WriteCall((void*)0x63981B, dtest);
		WriteCall((void*)0x639935, dtest);
		WriteCall((void*)0x639982, dtest);
		WriteCall((void*)0x639BD6, dtest);
		WriteCall((void*)0x639E83, dtest);
		WriteCall((void*)0x63A11A, dtest);
		WriteCall((void*)0x63A1A1, dtest);
		WriteCall((void*)0x63A3B6, dtest);
		WriteCall((void*)0x63A3EA, dtest);
		WriteCall((void*)0x63A44A, dtest);
		WriteCall((void*)0x63A506, dtest);
		WriteCall((void*)0x63A563, dtest);
		WriteCall((void*)0x63A6CA, dtest);
		WriteCall((void*)0x63A7AA, dtest);
		WriteCall((void*)0x63A9D7, dtest);
		WriteCall((void*)0x63AA28, dtest);
		WriteCall((void*)0x63AAFA, dtest);
		WriteCall((void*)0x63B117, dtest);
		WriteCall((void*)0x63B180, dtest);
		WriteCall((void*)0x63B429, dtest);
		WriteCall((void*)0x63B6B9, dtest);
		WriteCall((void*)0x63B9F7, dtest);
		WriteCall((void*)0x63BA33, dtest);
		WriteCall((void*)0x63BDB0, dtest);
		WriteCall((void*)0x63BE2C, dtest);
		WriteCall((void*)0x63C04F, dtest);
		WriteCall((void*)0x63C3CD, dtest);
		WriteCall((void*)0x63C717, dtest);
		WriteCall((void*)0x63D207, dtest);
		WriteCall((void*)0x63D24D, dtest);
		WriteCall((void*)0x63D29D, dtest);
		WriteCall((void*)0x63D2ED, dtest);
		WriteCall((void*)0x63D33D, dtest);
		WriteCall((void*)0x63D47B, dtest);
		WriteCall((void*)0x63D58A, dtest);
		WriteCall((void*)0x63D5DE, dtest);
		WriteCall((void*)0x63D9EA, dtest);
		WriteCall((void*)0x63DA8D, dtest);
		WriteCall((void*)0x63DCA6, dtest);
		WriteCall((void*)0x63DF5D, dtest);
		WriteCall((void*)0x63E083, dtest);
		WriteCall((void*)0x63E0F2, dtest);
		WriteCall((void*)0x63E43A, dtest);
		WriteCall((void*)0x63E6FA, dtest);
		WriteCall((void*)0x63E9AA, dtest);
		WriteCall((void*)0x63E9EA, dtest);
		WriteCall((void*)0x63ED6A, dtest);
		WriteCall((void*)0x63F26D, dtest);
		WriteCall((void*)0x63F364, dtest);
		WriteCall((void*)0x63F72A, dtest);
		WriteCall((void*)0x63FB0A, dtest);
		WriteCall((void*)0x63FB87, dtest);
		WriteCall((void*)0x63FC77, dtest);
		WriteCall((void*)0x640447, dtest);
		WriteCall((void*)0x6406E1, dtest);
		WriteCall((void*)0x64085A, dtest);
		WriteCall((void*)0x6FE65D, dtest);
		WriteCall((void*)0x70115D, dtest);
		WriteCall((void*)0x7160AC, dtest);
		WriteCall((void*)0x716CD0, dtest);
		WriteCall((void*)0x71B8AE, dtest);
		WriteCall((void*)0x71BBB6, dtest);
		WriteCall((void*)0x71BEA6, dtest);
		WriteCall((void*)0x71C016, dtest);
		WriteCall((void*)0x71C10B, dtest);
		WriteCall((void*)0x71C1A8, dtest);
		WriteCall((void*)0x71C2BF, dtest);
		WriteCall((void*)0x71C564, dtest);
		WriteCall((void*)0x71C744, dtest);
		WriteCall((void*)0x71C924, dtest);
		WriteCall((void*)0x71CBF3, dtest);
		WriteCall((void*)0x71CCC4, dtest);
		WriteCall((void*)0x71CE1A, dtest);
		WriteCall((void*)0x71CFA6, dtest);
		WriteCall((void*)0x71D054, dtest);
		WriteCall((void*)0x71D0F8, dtest);
		WriteCall((void*)0x72976B, dtest);
		WriteCall((void*)0x729971, dtest);
		WriteCall((void*)0x72BD1E, dtest);
		WriteCall((void*)0x74C876, dtest);
		WriteCall((void*)0x74C87D, dtest);
		WriteCall((void*)0x74C883, dtest);
		WriteCall((void*)0x74C93C, dtest);
		WriteCall((void*)0x74CAB5, dtest);
		WriteCall((void*)0x74CF50, dtest);
		WriteCall((void*)0x74D40F, dtest);
		WriteCall((void*)0x74D471, dtest);
		WriteCall((void*)0x74D4E9, dtest);
		WriteCall((void*)0x75A4BA, dtest);
		WriteCall((void*)0x75A684, dtest);
		WriteCall((void*)0x79AA25, dtest);
		WriteCall((void*)0x79B38F, dtest);
		WriteCall((void*)0x79B3AD, dtest);
		WriteCall((void*)0x79B43B, dtest);
		WriteCall((void*)0x79B44B, dtest);
		WriteCall((void*)0x79B494, dtest);
		WriteCall((void*)0x79B4DA, dtest);
		WriteCall((void*)0x79B914, dtest);
		WriteCall((void*)0x79C4A7, dtest);
		WriteCall((void*)0x79CA14, dtest);
		WriteCall((void*)0x79CC10, dtest);
		WriteCall((void*)0x79CE51, dtest);
		WriteCall((void*)0x79D465, dtest);
		WriteCall((void*)0x79D9E5, dtest);
		WriteCall((void*)0x79F7F6, dtest);
		WriteCall((void*)0x7A162A, dtest);
		WriteCall((void*)0x7A1ACC, dtest);
		WriteCall((void*)0x7A1CDA, dtest);
		WriteCall((void*)0x7A26FA, dtest);
		WriteCall((void*)0x7A2F9B, dtest);
		WriteCall((void*)0x7A30EA, dtest);
		WriteCall((void*)0x7A35D3, dtest);
		WriteCall((void*)0x7A37C2, dtest);
		WriteCall((void*)0x7A3A10, dtest);
		WriteCall((void*)0x7A3B6E, dtest);
		WriteCall((void*)0x7A3D93, dtest);
		WriteCall((void*)0x7A3E61, dtest);
		WriteCall((void*)0x7A3F22, dtest);
		WriteCall((void*)0x7A4271, dtest);
		WriteCall((void*)0x7A43FE, dtest);
		WriteCall((void*)0x7A446E, dtest);
		WriteCall((void*)0x7A4559, dtest);
		WriteCall((void*)0x7A4CC7, dtest);
		WriteCall((void*)0x7A4EE9, dtest);
		WriteCall((void*)0x7A89D8, dtest);
		WriteCall((void*)0x7A8DB2, dtest);
		WriteCall((void*)0x7A8FA9, dtest);
		WriteCall((void*)0x7A914B, dtest);
		WriteCall((void*)0x7A9B80, dtest);
		WriteCall((void*)0x7ABC52, dtest);
		WriteCall((void*)0x7AC1A2, dtest);
		WriteCall((void*)0x7AC7C5, dtest);
		WriteCall((void*)0x7AC825, dtest);
		WriteCall((void*)0x7AC885, dtest);
		WriteCall((void*)0x7AC8E5, dtest);
		WriteCall((void*)0x7B04BB, dtest);
		WriteCall((void*)0x7B0B37, dtest);
		WriteCall((void*)0x7B0C8A, dtest);
		WriteCall((void*)0x7B0E1B, dtest);
		WriteCall((void*)0x7B0E2F, dtest);
		WriteCall((void*)0x7B0E4E, dtest);
		WriteCall((void*)0x7B1158, dtest);
		WriteCall((void*)0x7B13DE, dtest);
		WriteJump((void*)0x7B14F0, dtest);
		WriteCall((void*)0x7F0FE7, dtest);
		WriteData((BYTE*)0x7F0FE7, (BYTE)0x0);
		WriteCall((void*)0x7F103B, dtest);
		WriteData((BYTE*)0x7F103B, (BYTE)0x0);
		WriteCall((void*)0x7F122F, dtest);
		WriteData((BYTE*)0x7F122F, (BYTE)0x0);
		WriteCall((void*)0x7F124F, dtest);
		WriteData((BYTE*)0x7F124F, (BYTE)0x0);
		WriteCall((void*)0x7F1253, dtest);
		WriteData((BYTE*)0x7F1253, (BYTE)0x0);
		WriteCall((void*)0x7F1283, dtest);
		WriteData((BYTE*)0x7F1283, (BYTE)0x0);
		WriteCall((void*)0x7F1347, dtest);
		WriteData((BYTE*)0x7F1347, (BYTE)0x0);
		WriteCall((void*)0x7F139B, dtest);
		WriteData((BYTE*)0x7F139B, (BYTE)0x0);
	}


	int dtest(task* tp) {
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
			DisplayDebugString(1, "MEME MAKER");
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
					CharObj2Ptrs[playerNo]->PhysicsData.CollisionSize = 0;
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