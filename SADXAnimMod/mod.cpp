#include "SADXModLoader.h"
#include <math.h>
#include "Functions.h"
#include "NPCMode.h"
#include "EventAnimationPlayer.h"
#include "HeadEditor.h"
#include "FreeCamera.h"
#include "FreeMove.h"

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
	bool checkFontSize();


	//Chaos 6: Chaos6_Animations 
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

		initFaceTableBackup();
	}

	extern "C" __declspec(dllexport) void __cdecl OnFrame()
	{
		if (inEditMode && showDisplay) {
			DisplayDebugString(1, "MEME MAKER V1.0");
			DisplayDebugStringFormatted(2, "Mode = %d", mode);
			DisplayDebugStringFormatted(3, "Movement Speed = %f", movementSpeed);

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
				break;
			}

			//Change mode with B Button
			if (ControllerPointers[playerNo]->PressedButtons & Buttons_B) {
				if (mode < 4) mode++;
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

		//Check if Sonic or Tails are in Board or Pinball mode.
		exitMode = 0;
		switch (EntityData1Ptrs[playerNo]->CharID) {
		case 0:
			if (EntityData1Ptrs[playerNo]->Action >= 62 && EntityData1Ptrs[playerNo]->Action <= 68) exitMode = 1;
			if (EntityData1Ptrs[playerNo]->Action >= 69 && EntityData1Ptrs[playerNo]->Action <= 70) exitMode = 2;
			break;
		case 2:
			if (EntityData1Ptrs[playerNo]->Action >= 48 && EntityData1Ptrs[playerNo]->Action <= 54) exitMode = 1;
			if (EntityData1Ptrs[playerNo]->Action >= 55 && EntityData1Ptrs[playerNo]->Action <= 56) exitMode = 2;
			break;
		default:
			break;
		}

		//Check for Free Camera Mode
		if ((BYTE)camera_flags == 7) wasFreeCam = true;
		else wasFreeCam = false;
		inEditMode = true;

		//Reset everything.
		resetRunningAnim();
		movementSpeed = 5.0;
		CharObj2Ptrs[playerNo]->AnimationThing.Index = 0;
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
		CharObj2Ptrs[playerNo]->AnimationThing.Index = 0;

		switch (exitMode) {
		case 0:
			EntityData1Ptrs[playerNo]->Action = 1;
			break;
		case 1:
			switch (EntityData1Ptrs[playerNo]->CharID) {
			case 0:
				CharObj2Ptrs[playerNo]->AnimationThing.Index = 112;
				EntityData1Ptrs[playerNo]->Action = 62;
				break;
			case 2:
				CharObj2Ptrs[playerNo]->AnimationThing.Index = 116;
				EntityData1Ptrs[playerNo]->Action = 48;
				break;
			default:
				break;
			}
			break;
		case 2:
			switch (EntityData1Ptrs[playerNo]->CharID) {
			case 0:
				CharObj2Ptrs[playerNo]->AnimationThing.Index = 125;
				EntityData1Ptrs[playerNo]->Action = 69;
				break;
			case 2:
				CharObj2Ptrs[playerNo]->AnimationThing.Index = 128;
				EntityData1Ptrs[playerNo]->Action = 55;
				break;
			default:
				break;
			}
			break;
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
			if (analogData.leftX > 30072.0)
				if (movementSpeed < 19.9) movementSpeed = movementSpeed + 0.1;
			if (analogData.leftX < -30072.0)
				if (movementSpeed >= 0.1) movementSpeed = movementSpeed - 0.1;
		}
	}
}