#include "SADXModLoader.h"
#include <math.h>
#include "Animations.h"
#include "Functions.h"
#include "NPCMode.h"
#include "EventAnimationPlayer.h"

#include <iostream>
#include <fstream>
#include <string>
extern "C"
{
	int headControlsMode[8];
	bool inEditMode = false;
	bool lookingAtPosition[8];
	int runningAnim = 0;
	int runningFace[8];
	BYTE faceValue[8];
	BYTE mode = 0;
	bool wasFreeCam;
	int cameraMode = 0;
	bool headFrozen[8];
	std::string sub1 = "";
	std::string sub2 = "";
	std::string subtitleFilePath;
	char customSubtitle[128];
	const char* const hintMessage[] = { customSubtitle, NULL };
	BYTE exitMode;
	void* headPointer[8];
	bool eyeSelection;
	bool showDisplay;
	long double LeftX;
	long double LeftY;
	long double RightX;
	long double RightY;
	int levelCheck = 99;
	int actCheck = 99;
	int playerNo;
	bool characterHasBeenSpawned[8];
	double movementSpeed = 5.0;
	void doAnimationFunctions();
	void doMovementFunctions();
	void doCameraFunctions();
	void doFreeCamFunctions();
	void doHeadFunctions();
	void* getHeadData(int characterID);
	void* getHeadDataS();
	void* getHeadDataT();
	void* getHeadDataK();
	void* getHeadDataTi();
	void* getHeadDataA();
	void* getHeadDataB();
	int getAnimLimit();
	bool getCanHeadFreeze();
	bool getCanUseEditMode();
	PhysicsData backup[8];
	void startUp();
	void shutDown();
	bool checkFontSize();
	bool anyHeadsFrozen();


	//Chaos 6: Chaos6_Animations 
	//Perfect Chaos: PerfectChaos_AnimList
	HMODULE SUPERSONICMOD = GetModuleHandle(L"sadx-super-sonic");
	double getRawInput(bool isRight, bool isY);
	void doSpeedModifier();
	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions) {
		subtitleFilePath = std::string(path) + "\\text.txt";
		for (int i = 0; i < 8; i++) {
			backup[i] = PhysicsArray[i];
			headFrozen[i] = false;
			runningFace[i] = 0;
			faceValue[i] = 0;
			lookingAtPosition[i] = false;
			headControlsMode[i] = 0;
			headPointer[i] = 0;
		}

		//Record pointer to player's head.
		WriteCall((void*)0x49587D, getHeadDataS); //Sonic
		WriteCall((void*)0x46146D, getHeadDataT); //Tails
		WriteCall((void*)0x477D4D, getHeadDataK); //Knuckles
		WriteCall((void*)0x7B407D, getHeadDataTi); //Tikal
		WriteCall((void*)0x48883D, getHeadDataA); //Amy
		WriteCall((void*)0x4909BD, getHeadDataB); //Big
	}

	extern "C" __declspec(dllexport) void __cdecl OnFrame()
	{
		if (inEditMode && showDisplay) {
			DisplayDebugString(1, "MEME MAKER V1.0");
			DisplayDebugStringFormatted(2, "Mode = %d", mode);
			DisplayDebugStringFormatted(3, "Movement Speed = %f", movementSpeed);

			switch (mode) {
			case 0:
				DisplayDebugStringFormatted(4, "Current Animation = %d", CharObj2Ptrs[playerNo]->AnimationThing.Index);
				DisplayDebugStringFormatted(6, "PosX = %f", EntityData1Ptrs[playerNo]->Position.x);
				DisplayDebugStringFormatted(7, "PosY = %f", EntityData1Ptrs[playerNo]->Position.y);
				DisplayDebugStringFormatted(8, "PosZ = %f", EntityData1Ptrs[playerNo]->Position.z);
				DisplayDebugStringFormatted(10, "RotX = %d", EntityData1Ptrs[playerNo]->Rotation.x);
				DisplayDebugStringFormatted(11, "RotY = %d", EntityData1Ptrs[playerNo]->Rotation.y);
				DisplayDebugStringFormatted(12, "RotZ = %d", EntityData1Ptrs[playerNo]->Rotation.z);
				break;
			case 1:
				switch (cameraMode) {
				case 0: DisplayDebugString(4, "Camera Mode: NORMAL"); break;
				case 1: DisplayDebugString(4, "Camera Mode: FREE MOVEMENT"); break;
				case 2: DisplayDebugString(4, "Camera Mode: FOLLOW FROM FIXED POINT"); break;
				case 3: DisplayDebugString(4, "Camera Mode: NO VANILLA CAMERAS"); break;
				}

				DisplayDebugStringFormatted(6, "CamPosX = %f", Camera_Data1->Position.x);
				DisplayDebugStringFormatted(7, "CamPosY = %f", Camera_Data1->Position.y);
				DisplayDebugStringFormatted(8, "CamPosZ = %f", Camera_Data1->Position.z);
				DisplayDebugStringFormatted(10, "CamRotX = %d", Camera_Data1->Rotation.x);
				DisplayDebugStringFormatted(11, "CamRotY = %d", Camera_Data1->Rotation.y);
				DisplayDebugStringFormatted(12, "CamRotZ = %d", Camera_Data1->Rotation.z);
				break;
			case 2:
				DisplayDebugStringFormatted(4, "Head Editor Mode: %d", headControlsMode[playerNo]);
				switch (headControlsMode[playerNo]) {
				case 0:
					if (headFrozen[playerNo]) {
						DisplayDebugString(5, "Custom Head Controls Enabled, Press A to reset.");
						if (faceValue[GetCharacterID(playerNo)] > 19)DisplayDebugStringFormatted(6, "Current Face = %d (Values higher than 19 are animations activated with X)", faceValue[GetCharacterID(playerNo)]);
						else DisplayDebugStringFormatted(6, "Current Face = %d", faceValue[GetCharacterID(playerNo)]);
						
						if (eyeSelection) DisplayDebugString(7, "Selected Eye = Left");
						else DisplayDebugString(8, "Selected Eye = Right");
					}
					else {
						if (getCanHeadFreeze) DisplayDebugString(5, "Custom Head Controls Disabled, Press A to activate.");
						else DisplayDebugString(5, "Eggman, Gamma and Super Sonic do not have this data.");
						if (EntityData1Ptrs[playerNo]->CharID == Characters_Big) DisplayDebugString(7, "NOTE: Big cannot move his head, he has only limited eye movement.");
					} break;
				case 1:
					DisplayDebugString(5, "Press A to focus the character's head on the camera's current position.");
					DisplayDebugString(6, "To reset, go to mode 0.");
					break;
				case 2:
					DisplayDebugString(5, "Auto Follow: Camera"); break;
				}

				if (headControlsMode[playerNo] > 2) DisplayDebugStringFormatted (6, "Auto Follow: Player %d", headControlsMode[playerNo] - 2);
				break;


			case 3:
				DisplayDebugString(4, "Event Animation Player");

				switch (getEventObject()) {
				case 0: DisplayDebugString(5, "Object: Player Character"); break;
				case 1: DisplayDebugString(5, "Object: ZERO"); break;
				case 2: DisplayDebugString(5, "Object: SS POLICE"); break;
				case 3: DisplayDebugString(5, "Object: CHAOS 0"); break;
				case 4: DisplayDebugString(5, "Object: CHAOS 6"); break;
				case 5: DisplayDebugString(5, "Object: PERFECT CHAOS"); break;
				case 6: DisplayDebugString(5, "Object: PACHACAMAC"); break;
				case 7: DisplayDebugString(5, "Object: EGG MOBILE EGGMAN"); break;
				case 8: DisplayDebugString(5, "Object: TIKAL'S CHAO"); break;
				case 9: DisplayDebugString(5, "Object: CHAOS EMERALD (GREEN)"); break;
				case 10: DisplayDebugString(5, "Object: CHAOS EMERALD (BLUE)"); break;
				case 11: DisplayDebugString(5, "Object: CHAOS EMERALD (PURPLE)"); break;
				case 12: DisplayDebugString(5, "Object: CHAOS EMERALD (RED)"); break;
				case 13: DisplayDebugString(5, "Object: CHAOS EMERALD (SKY)"); break;
				case 14: DisplayDebugString(5, "Object: CHAOS EMERALD (WHITE)"); break;
				case 15: DisplayDebugString(5, "Object: CHAOS EMERALD (YELLOW)"); break;
				case 16: DisplayDebugString(5, "Object: CHAOS EMERALD (BLACK)"); break;
				case 17: DisplayDebugString(5, "Object: SS POLICE CAR"); break;
				case 18: DisplayDebugString(5, "Object: NPC MALE 1"); break;
				case 19: DisplayDebugString(5, "Object: NPC MALE 2"); break;
				case 20: DisplayDebugString(5, "Object: NPC MALE 3"); break;
				case 21: DisplayDebugString(5, "Object: NPC MALE 4"); break;
				case 22: DisplayDebugString(5, "Object: NPC FEMALE 1"); break;
				case 23: DisplayDebugString(5, "Object: NPC FEMALE 2"); break;
				case 24: DisplayDebugString(5, "Object: NPC FEMALE 3"); break;
				case 25: DisplayDebugString(5, "Object: NPC FEMALE 4"); break;
				case 26: DisplayDebugString(5, "Object: NPC GIRL 1"); break;
				case 27: DisplayDebugString(5, "Object: NPC GIRL 2"); break;
				case 28: DisplayDebugString(5, "Object: NPC GIRL 3"); break;
				case 29: DisplayDebugString(5, "Object: NPC GIRL 4"); break;
				case 30: DisplayDebugString(5, "Object: NPC FAT 1"); break;
				case 31: DisplayDebugString(5, "Object: NPC FAT 2"); break;
				case 32: DisplayDebugString(5, "Object: NPC BOY 1"); break;
				case 33: DisplayDebugString(5, "Object: NPC FAT 3"); break;
				case 34: DisplayDebugString(5, "Object: NPC FAT 4"); break;
				case 35: DisplayDebugString(5, "Object: NPC BOY 2"); break;
				case 36: DisplayDebugString(5, "Object: NPC BOY 3"); break;
				case 37: DisplayDebugString(5, "Object: NPC BOY 4"); break;
				case 38: DisplayDebugString(5, "Object: NPC HOTEL FEMALE 1"); break;
				case 39: DisplayDebugString(5, "Object: NPC HOTEL FEMALE 2"); break;
				case 40: DisplayDebugString(5, "Object: NPC HOTEL FEMALE 3"); break;
				case 41: DisplayDebugString(5, "Object: NPC BURGER SHOP FEMALE"); break;
				case 42: DisplayDebugString(5, "Object: NPC TRAIN STATION WORKER"); break;
				case 43: DisplayDebugString(5, "Object: NPC BURGER SHOP MALE"); break;
				}
				DisplayDebugStringFormatted(6, "Index: %d", getEventIndex()); 
				DisplayDebugStringFormatted(7, "Animation Speed: %f", getEventSpeed());
				DisplayDebugString(8, "Press A to Play.");
				DisplayDebugString(9, "Press X+A to Loop."); 
				break;
			case 4:
				DisplayDebugString(4, "NPC Mode");
				DisplayDebugString(5, "Press A to toggle.");
				DisplayDebugStringFormatted(6, "Currently selected NPC: %d", getNPCIndex(playerNo));
			}
		}


		//Stupid hacky way to remap the default cam zoom from Y to X.
		//Because some prat made R rotation read Buttons_Y.
		if (ControllerPointers[0]->HeldButtons & Buttons_X) {
			WriteData<1>((int*)0x462F4B, 0x04);
			WriteData<1>((int*)0x462F50, 0x74);
		} else {
			WriteData<1>((int*)0x462F4B, 0x02);
			WriteData<1>((int*)0x462F50, 0x7D);
		}

		//Only keep the face if Sonic's head is frozen or it might break cutscenes.
		
		
		for (int i = 0; i < GetPlayerCount(); i++) {
			if (headFrozen[i]) {
				if (faceValue[GetCharacterID(i)] <= 19) {
					runningFace[GetCharacterID(i)] = faceValue[GetCharacterID(i)] | (faceValue[GetCharacterID(i)] << 8) | (faceValue[GetCharacterID(i)] << 16);
					WriteData((int*)((char*)EntityData1Ptrs[i]->field_3C + 0x5C), runningFace[GetCharacterID(i)]);


					WriteData<1>((int*)(0x418873), 0x75); //Prevent auto blink override
					WriteData<1>((int*)(0x4188BE), 0x01); //Prevent auto blink override
				}
				else {
					WriteData<1>((int*)(0x418873), 0x74);
					WriteData<1>((int*)(0x4188BE), 0x02);
				}
			}
		}

		
	}




	__declspec(dllexport) void __cdecl OnInput()
	{
		if (GameState == 15) { //Action check will crash the game on the main menu.
			
			for (int i = 0; i < GetPlayerCount(); i++) {
				if (ControllerPointers[i]->PressedButtons & Buttons_Y && getCanUseEditMode && ControllerPointers[i]->NotHeldButtons & Buttons_Z) {
					playerNo = i;
					if (!inEditMode) startUp();
					else shutDown();
				}
			
				if (headControlsMode[i] == 2) EV_LookPoint(EV_GetPlayer(i),
					Camera_Data1->Position.x, Camera_Data1->Position.y, Camera_Data1->Position.z);

				if (headControlsMode[i] > 2) {
					EV_LookPoint(EV_GetPlayer(playerNo),
					EntityData1Ptrs[headControlsMode[i] - 3]->Position.x,
					EntityData1Ptrs[headControlsMode[i] - 3]->Position.y,
					EntityData1Ptrs[headControlsMode[i] - 3]->Position.z);
				}

				
			}


		}

		if ((GameState != 15 && GameState != 16) || CutsceneMode) {
			if (inEditMode) shutDown(); //If the game tries to load an area with this running it'll crash.

			if (GameState == 9 || CutsceneMode) {
				WriteData((unsigned int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 44), 0xFFFFE000);
				WriteData((unsigned int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 52), 0xFFFFE800);
				WriteData((unsigned int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 60), 0xFFFFE500);
				WriteData((unsigned int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 68), 0xFFFFE300);
				WriteData((unsigned int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 76), 0xFFFFEE00);
				WriteData((unsigned int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 84), 0xFFFFE300);
			}
			
			cameraMode = 0;
			for (int i = 0; i < 8; i++) {
				characterHasBeenSpawned[i] = false;
				headFrozen[i] = false;
			}
			WriteData<1>((int*)(0x418873), 0x74);
			WriteData<1>((int*)(0x4403BA), 0x08);
			WriteData<1>((int*)(0x4403F9), 0x0C);
			WriteData<1>((int*)(0x4188BE), 0x02);
		}

		if (ControllerPointers[0]->PressedButtons & Buttons_C) {
			std::ifstream subFile(subtitleFilePath);
			if (subFile.is_open())
			{
				std::getline(subFile, sub1);
				std::getline(subFile, sub2);
				subFile.close();
			}
			snprintf(customSubtitle, LengthOfArray(customSubtitle), "\a%s \n%s", sub1.c_str(), sub2.c_str());
			DisplayHintText(hintMessage, 180);
		}

		if (inEditMode) {
			//Global Code

			//Disable Pause and stop text becoming huge when the DLC timer is enabled.

			if (checkFontSize) { //Make sure character select's menu isn't open and reset the size when needed.
				SetDebugFontSize(((float)HorizontalResolution / 640.0f) * 8); //DLC mods will cause text to become huge otherwise.
			}


			if(EntityData1Ptrs[0]->CharID & Characters_Gamma) TimeMinutes = 2; //Stop Gamma dying
			CharObj2Ptrs[0]->UnderwaterTime = 0; //Stop 1P from drowning

			CharObj2Ptrs[playerNo]->AnimationThing.Index = runningAnim; //Stop the game moving to the next animation in certain cases.
			EntityData1Ptrs[playerNo]->Action = 99; //Compatibility with CharacterSelect
			DisablePause(); //Text Box will enable pause menu afterwards so this needs to run globally.
			doCameraFunctions();
			doSpeedModifier();

			switch (mode) {
			case 0:
				doAnimationFunctions();
				doMovementFunctions();
				break;
			case 1:
				doFreeCamFunctions();
				break;
			case 2:
				doHeadFunctions();
				break;
			case 3:
				doEventAnimFunction(playerNo);
				doMovementFunctions();
				break;
			case 4:
				doNPCModeDisplay(playerNo);
				break;
			}
			
			if (ControllerPointers[playerNo]->PressedButtons & Buttons_B) {
				if (mode < 4) mode++;
				else mode = 0;
			}

			LeftX = getRawInput(false, false);
			LeftY = getRawInput(false, true);
			RightX = getRawInput(true, false);
			RightY = getRawInput(true, true);

			if (ControllerPointers[playerNo]->PressedButtons & Buttons_Start) showDisplay = !showDisplay;
		}
	}

	bool checkFontSize() {
		for (int i = 0; i < GetPlayerCount(); i++)
		{
			if (ControllerPointers[i]->HeldButtons & Buttons_Z) return false;
		}

		return true;
	}

	void startUp() {
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

		//Set Action to 99 to disable the main controls.
		CharObj2Ptrs[playerNo]->AnimationThing.Index = 0;
		inEditMode = true;

		//Reset everything.
		runningAnim = 0;
		runningFace[playerNo] = 0 | (0 << 8) | (0 << 16);
		movementSpeed = 5.0;
		CharObj2Ptrs[playerNo]->AnimationThing.AnimData->AnimationSpeed = 0.5;
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

	void shutDown() {
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


		if (cameraMode == 0) {
			WriteData<1>((int*)0x437760, 0x74); //Enable normal cameras
			WriteData<1>((int*)0x436308, 0x8F); //Enable first person cam
		}

		//Enable Debug for Eggman and Tikal
		WriteData<1>((int*)(0x7B43B4), 0x74);
		WriteData<1>((int*)(0x7B5288), 0x74);
		WriteData<1>((int*)(0x7B529C), 0x74);

		WriteData<1>((int*)0x44AEFB, 0x84); //Enable Death Zones
		WriteData<1>((int*)0x496F8C, 0x74); //Remove invincibility
		if (!isNPC(playerNo)) WriteData<1>((int*)0x413DA4, 0x00); //Enable the HUD
		EnablePause();
	}


	bool getCanUseEditMode() {
		if (EntityData1Ptrs[playerNo]->Action == 45) return false;
		if (CurrentLevel == LevelIDs_SkyChase1 | LevelIDs_SkyChase2) return false;
		return true;
	}

	void doSpeedModifier() {
		if (ControllerPointers[playerNo]->HeldButtons & Buttons_X && mode != 3) {
			if (LeftX > 30072.0)
				if (movementSpeed < 19.9) movementSpeed = movementSpeed + 0.1;
			if (LeftX < -30072.0) 
				if (movementSpeed >= 0.1) movementSpeed = movementSpeed - 0.1;
		}
	}

	void doCameraFunctions() {
		//Always Default Cam
		WriteData<1>((int*)0x3B2CADE, 0x00);
		WriteData<1>((int*)0x3B2CAE0, 0x00);
		WriteData((int*)0x3B2CAE3, 0x462E9002);
		WriteData((int*)0x3B2CAE7, 0x467D8000);

		//Force Auto Cam because it breaks otherwise.
		WriteData<1>((int*)0x3B2CBA8, 0x04);

		switch (cameraMode) {
		case 0: WriteData<1>((int*)0x3B2CADF, 0x01); break;
		case 1: WriteData<1>((int*)0x3B2CADF, 0x00); break;
		case 2: WriteData<1>((int*)0x3B2CADF, 0x02); break;
		case 3: WriteData<1>((int*)0x3B2CADF, 0x01); break;
		}

	}

	double getRawInput(bool isRight, bool isY) {
		int rightXRaw;
		int rightYRaw;
		int leftXRaw;
		int leftYRaw;

		if (isRight) {
			if (isY) {
				rightYRaw = ControllerPointers[playerNo]->RightStickY << 8;
				return (double)rightYRaw;
			} else {
				rightXRaw = ControllerPointers[playerNo]->RightStickX;
				return (double)(rightXRaw << 8);
			}
		} else {
			if (isY) {
				leftYRaw = ControllerPointers[playerNo]->LeftStickY << 8;
				return (double)leftYRaw;
			} else {
				leftXRaw = ControllerPointers[playerNo]->LeftStickX;
				return (double)(leftXRaw << 8);
			}
		}
	}

	void doFreeCamFunctions() {
		switch (ControllerPointers[playerNo]->PressedButtons) {
		case Buttons_Up: cameraMode = 0; break;
		case Buttons_Left: cameraMode = 1; break;
		case Buttons_Right: cameraMode = 2; break;
		case Buttons_Down: cameraMode = 3; break;
		default: break;
		}

		if (cameraMode == 1) {
			if (ControllerPointers[playerNo]->HeldButtons & Buttons_X) {
				//Y Position
				if (LeftY > 3072.0 || LeftY < -3072.0) Camera_Data1->Position.y = Camera_Data1->Position.y - LeftY / squareroot((LeftY * LeftY)) * (movementSpeed / 2);

				//Y Rotation
				if (RightX > 3072.0)  Camera_Data1->Rotation.z = (int)(Camera_Data1->Rotation.z - (movementSpeed * 100));
				if (RightX < -3072.0) Camera_Data1->Rotation.z = (int)(Camera_Data1->Rotation.z + (movementSpeed * 100));

			}
			else {
				if (LeftX > 3072.0 || LeftX < -3072.0 || LeftY > 3072.0 || LeftY < -3072.0) {
					Angle v10 = -Camera_Data1->Rotation.y - (unsigned __int64)(atan2(LeftY, LeftX) * 65536.0 * -0.1591549762031479);
					Camera_Data1->Position.x = njCos(v10) * (movementSpeed / 2) + Camera_Data1->Position.x;
					Camera_Data1->Position.z = njSin(v10) * (movementSpeed / 2) + Camera_Data1->Position.z;
				}

				if (RightY > 3072.0)  Camera_Data1->Rotation.x = (int)(Camera_Data1->Rotation.x - (movementSpeed * 100));
				if (RightY < -3072.0) Camera_Data1->Rotation.x = (int)(Camera_Data1->Rotation.x + (movementSpeed * 100));
				if (RightX > 3072.0)  Camera_Data1->Rotation.y = (int)(Camera_Data1->Rotation.y - (movementSpeed * 100));
				if (RightX < -3072.0) Camera_Data1->Rotation.y = (int)(Camera_Data1->Rotation.y + (movementSpeed * 100));
			}
		}
	}

	void* getHeadDataS() { return getHeadData(Characters_Sonic); }
	void* getHeadDataT() { return getHeadData(Characters_Tails); }
	void* getHeadDataK() { return getHeadData(Characters_Knuckles); }
	void* getHeadDataTi() { return getHeadData(Characters_Tikal); }
	void* getHeadDataA() { return getHeadData(Characters_Amy); }
	void* getHeadDataB() { return getHeadData(Characters_Big); }
	void* getHeadData(int characterID) {
		if (GameState == 21) return AllocateMemory(136);

		if (!(levelCheck == CurrentLevel && actCheck == CurrentAct)) {
			reset:
			for (int i = 0; i < 7; i++) characterHasBeenSpawned[i] = false;
			levelCheck = CurrentLevel;
			actCheck = CurrentAct;

			spawnNewChar:
			if (!characterHasBeenSpawned[characterID]) {
				characterHasBeenSpawned[characterID] = true;
				headPointer[characterID] = AllocateMemory(136);
				return headPointer[characterID];
			}
			else return AllocateMemory(136);
		} else {
			if (!characterHasBeenSpawned[characterID]) goto spawnNewChar;
			else return AllocateMemory(136);
		}
	}

	void doHeadFunctions() {
		if (headControlsMode[playerNo] > (GetPlayerCount() + 2)) {
			headControlsMode[playerNo] = 0;
			goto StopLooking;
		}

		
		if (faceValue[GetCharacterID(playerNo)] > 19 && ControllerPointers[playerNo]->HeldButtons & Buttons_X) {
			EV_SetFace(EV_GetPlayer(playerNo), (char*)faceAnimations[faceValue[GetCharacterID(playerNo)] - 20]);
		}

		switch (ControllerPointers[playerNo]->PressedButtons) {
		case Buttons_Left:
			if (!headFrozen[playerNo]) {
				if (headControlsMode[playerNo] > 0) {
					headControlsMode[playerNo]--;
				}
				if (headControlsMode[playerNo] < 2) EV_LookFree(EV_GetPlayer(playerNo));
				lookingAtPosition[playerNo] = false;
			}
			break;
		case Buttons_Right:
			if (!headFrozen[playerNo]) {
				if (headControlsMode[playerNo] < (GetPlayerCount() + 2)) headControlsMode[playerNo]++;
				lookingAtPosition[playerNo] = false;
				headFrozen[playerNo] = false;
				goto CloseHead;
			}

			break;
		case Buttons_A:
			switch (headControlsMode[playerNo]) {
			case 0:
				if (getCanHeadFreeze && !CutsceneMode) {
					headFrozen[playerNo] = !headFrozen[playerNo];
					if (headFrozen[playerNo]) {
						for (int i = 44; i < 85; i += 8) {
							WriteData((int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + i), 0);
						}
					}
					else {
					CloseHead:
						WriteData((unsigned int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 44), 0xFFFFE000);
						WriteData((unsigned int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 52), 0xFFFFE800);
						WriteData((unsigned int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 60), 0xFFFFE500);
						WriteData((unsigned int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 68), 0xFFFFE300);
						WriteData((unsigned int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 76), 0xFFFFEE00);
						WriteData((unsigned int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 84), 0xFFFFE300);
					}
				}
				break;
			case 1:
				if (!lookingAtPosition[playerNo]) {
					EV_LookPoint(EV_GetPlayer(playerNo), Camera_Data1->Position.x, Camera_Data1->Position.y, Camera_Data1->Position.z);
					lookingAtPosition[playerNo] = true;
				}
				else {
				StopLooking:
					EV_LookFree(EV_GetPlayer(playerNo));
					lookingAtPosition[playerNo] = false;
				}
				break;
			case 2:
				break;
			}
		default:
			break;
		}

		if (headFrozen[playerNo]) {
			WriteData<1>((int*)(0x4403BA), 0x10); //Remove rotation limits.
			WriteData<1>((int*)(0x4403F9), 0x10);

			switch (ControllerPointers[playerNo]->PressedButtons) {
			case Buttons_Up: //Increment Face
				if (faceValue[GetCharacterID(playerNo)] < 52 ||
					(faceValue[GetCharacterID(playerNo)] < 8 && EntityData1Ptrs[playerNo]->CharID == Characters_Big)) faceValue[GetCharacterID(playerNo)]++;
				else faceValue[GetCharacterID(playerNo)] = 0;
				break;
			case Buttons_Down: //Decrement Face
				if (faceValue[GetCharacterID(playerNo)] > 0) faceValue[GetCharacterID(playerNo)]--;
				else if (EntityData1Ptrs[playerNo]->CharID == Characters_Big) faceValue[GetCharacterID(playerNo)] = 7;
				else faceValue[GetCharacterID(playerNo)] = 52;
				break;
			default:
				break;
			}
			
			if (movementSpeed != 0.0) { //Head won't move below 1.0
				if (RightX > 3072.0)  WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 45), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 45) - (movementSpeed + 1.0)));
				if (RightX < -3072.0) WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 45), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 45) + (movementSpeed + 1.0)));

				if (RightY > 3072.0)  WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 53), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 53) - (movementSpeed + 1.0)));
				if (RightY < -3072.0) WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 53), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 53) + (movementSpeed + 1.0)));

				if (ControllerPointers[playerNo]->PressedButtons & Buttons_Right) eyeSelection = !eyeSelection;
				if (ControllerPointers[playerNo]->PressedButtons & Buttons_Left)  eyeSelection = !eyeSelection;

				//Eyes Y (Both use the same address)
				if (ControllerPointers[playerNo]->NotHeldButtons & Buttons_X) {
					if (LeftY > 3072.0)  WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 85), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 85) + (movementSpeed + 1.0)));
					if (LeftY < -3072.0) WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 85), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 85) - (movementSpeed + 1.0)));
				
				
				if (eyeSelection) { //Eyes X
					if (LeftX > 3072.0)  WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 61), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 61) - (movementSpeed + 1.0)));
					if (LeftX < -3072.0) WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 61), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 61) + (movementSpeed + 1.0)));
				}
				else {
					if (LeftX > 3072.0)  WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 77), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 77) - (movementSpeed + 1.0)));
					if (LeftX < -3072.0) WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 77), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 77) + (movementSpeed + 1.0)));
				}
				}
			}

		} else {
			if (!anyHeadsFrozen) {
				WriteData<1>((int*)(0x418873), 0x74);
				WriteData<1>((int*)(0x4403BA), 0x08);
				WriteData<1>((int*)(0x4403F9), 0x0C);
				WriteData<1>((int*)(0x4188BE), 0x02);
			}

			if (faceValue[GetCharacterID(playerNo)] <= 19) {
				runningFace[playerNo] = 0 | (0 << 8) | (0 << 16);
				WriteData((int*)((char*)EntityData1Ptrs[playerNo]->field_3C + 0x5C), runningFace);
				faceValue[GetCharacterID(playerNo)] = 0;
			}
			
		}

	}


	bool anyHeadsFrozen() {
		for (int i = 0; i < GetPlayerCount(); i++) {
			if (headFrozen[i]) return true;
		}
		return false;
	}

	bool getCanHeadFreeze() {
		if (EntityData1Ptrs[playerNo]->CharID == Characters_Eggman | Characters_Gamma) return false;
		if (EntityData1Ptrs[playerNo]->CharID == Characters_Sonic && CharObj2Ptrs[playerNo]->AnimationThing.Index > 133 && CharObj2Ptrs[playerNo]->AnimationThing.Index < 146) return false;
		return true;
	}

	void doAnimationFunctions() {
		switch (ControllerPointers[playerNo]->PressedButtons) {
		case Buttons_Right:
			if (runningAnim == getAnimLimit()) runningAnim = 0; else runningAnim += 1; 
			break;
		case Buttons_Left:
			if (runningAnim > 0) runningAnim -= 1; //
			break;
		case Buttons_Up:
			if (runningAnim > (getAnimLimit() - 10)) runningAnim = 0; else runningAnim += 10;
			break;
		case Buttons_Down:
			if (runningAnim > 10) runningAnim -= 10; else runningAnim = 0;
			break;
		default:
			break;
		}
	}

	int getAnimLimit() {
		switch (EntityData1Ptrs[playerNo]->CharID) {
		case 0: if (CurrentLevel == LevelIDs_PerfectChaos || SUPERSONICMOD) return 146; else return 133;
		case 1: return 23;
		case 2: return 135;
		case 3: return 104;
		case 4: return 8;
		case 5: return 101;
		case 6: return 90;
		case 7: return 104;
		default: break;
		}
	}

	void doMovementFunctions() {
		//AKA Shamelessly copy the debug movement code lol

		if (ControllerPointers[playerNo]->HeldButtons & Buttons_X) {
			//Y Position
			if (LeftY > 25072.0 || LeftY < -25072.0) EntityData1Ptrs[playerNo]->Position.y = EntityData1Ptrs[playerNo]->Position.y - LeftY / squareroot((LeftY * LeftY)) * movementSpeed;
			
			//Y Rotation
			if (RightX > 25072.0) EntityData1Ptrs[playerNo]->Rotation.y = (int)(EntityData1Ptrs[playerNo]->Rotation.y - (movementSpeed * 100));
			if (RightX < -25072.0) EntityData1Ptrs[playerNo]->Rotation.y = (int)(EntityData1Ptrs[playerNo]->Rotation.y + (movementSpeed * 100));
		}else{
			if (LeftX > 3072.0 || LeftX < -3072.0 || LeftY > 3072.0 || LeftY < -3072.0){
				Angle v10 = -Camera_Data1->Rotation.y - (unsigned __int64)(atan2(LeftY, LeftX) * 65536.0 * -0.1591549762031479);
				EntityData1Ptrs[playerNo]->Position.x = njCos(v10) * movementSpeed + EntityData1Ptrs[playerNo]->Position.x;
				EntityData1Ptrs[playerNo]->Position.z = njSin(v10) * movementSpeed + EntityData1Ptrs[playerNo]->Position.z;
			}

			if (RightX > 3072.0 || RightX < -3072.0 || RightY > 3072.0 || RightY < -3072.0) {
				Angle v10 = -Camera_Data1->Rotation.y - (unsigned __int64)(atan2(RightY, RightX) * 65536.0 * -0.1591549762031479);
					EntityData1Ptrs[playerNo]->Rotation.x = (signed int)(njSin(v10) * (movementSpeed * 100) + EntityData1Ptrs[playerNo]->Rotation.x);
					EntityData1Ptrs[playerNo]->Rotation.z = (signed int)(njCos(v10) * (movementSpeed * -100) + EntityData1Ptrs[playerNo]->Rotation.z);

			}
		}
	}

}