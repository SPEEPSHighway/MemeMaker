#include "SADXModLoader.h"
#include "Functions.h"
#include "NPCMode.h"

#include <iostream>
#include <fstream>
#include <string>

bool halfface = false;
bool headFrozen[8] = { false };
std::string customFace;
int runningFace[8] = { 0 };
BYTE faceValue[8] = { 0 };
void* headPointer[8] = { 0 };
bool eyeSelection;
int levelCheck = 99;
int actCheck = 99;
bool characterHasBeenSpawned[8] = { 0 };
int headControlsMode[8] = { 0 };
bool lookingAtPosition[8] = { 0 };
FACETBL facetbl2[faceTable_Length];


void* getHeadData(int characterID) {
	if (GameState == 21) return AllocateMemory(136);

	if (!(levelCheck == CurrentLevel && actCheck == CurrentAct)) {
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
	}
	else {
		if (!characterHasBeenSpawned[characterID]) goto spawnNewChar;
		else return AllocateMemory(136);
	}
}

void initFaceTableBackup() {
	for (int i = 0; i < faceTable_Length; i++) facetbl2[i].nbFrame = faceTable[i].nbFrame;
}

void* getHeadDataS() { return getHeadData(Characters_Sonic); }
void* getHeadDataT() { return getHeadData(Characters_Tails); }
void* getHeadDataK() { return getHeadData(Characters_Knuckles); }
void* getHeadDataTi() { return getHeadData(Characters_Tikal); }
void* getHeadDataA() { return getHeadData(Characters_Amy); }
void* getHeadDataB() { return getHeadData(Characters_Big); }

void* getHeadPointer(int characterID) {
	return headPointer[characterID];
}

bool getEye() {
	return eyeSelection;
}

bool anyHeadsFrozen() {
	for (int i = 0; i < GetPlayerCount(); i++) {
		if (headFrozen[i]) return true;
	}
	return false;
}

bool getCanHeadFreeze(int playerNo) {
	if (EntityData1Ptrs[playerNo]->CharID == Characters_Eggman || EntityData1Ptrs[playerNo]->CharID == Characters_Gamma) return false;
	if (EntityData1Ptrs[playerNo]->CharID == Characters_Sonic && CharObj2Ptrs[playerNo]->AnimationThing.Index > 133 && CharObj2Ptrs[playerNo]->AnimationThing.Index < 146) return false;
	return true;
}

void doHeadChecks(int playerNo) {
	for (int i = 0; i < GetPlayerCount(); i++) {
		if (headControlsMode[i] == 2) EV_LookPoint(EV_GetPlayer(i),
			Camera_Data1->Position.x, Camera_Data1->Position.y, Camera_Data1->Position.z);

		if (headControlsMode[i] > 2) {
			EV_LookPoint(EV_GetPlayer(i),
				EntityData1Ptrs[headControlsMode[i] - 3]->Position.x,
				EntityData1Ptrs[headControlsMode[i] - 3]->Position.y,
				EntityData1Ptrs[headControlsMode[i] - 3]->Position.z);
		}

		if (faceValue[GetCharacterID(i)] <= 19 && faceValue[GetCharacterID(i)] > 0) {
			runningFace[GetCharacterID(i)] = faceValue[GetCharacterID(i)] | (faceValue[GetCharacterID(i)] << 8) | (faceValue[GetCharacterID(i)] << 16);
			WriteData((int*)((char*)EntityData1Ptrs[i]->field_3C + 0x5C), runningFace[GetCharacterID(i)]);
			//PFaceChange(0, faceValue[GetCharacterID(i)]);
		}

		if (faceValue[GetCharacterID(i)] > 19) {
			PFaceChange(i, faceValue[GetCharacterID(i)] - 20);
		}
	}
}

void stopHeadEdits(int playerNo) {
	if (GameState == 9 || CutsceneMode) {
		WriteData((unsigned int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 44), 0xFFFFE000);
		WriteData((unsigned int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 52), 0xFFFFE800);
		WriteData((unsigned int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 60), 0xFFFFE500);
		WriteData((unsigned int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 68), 0xFFFFE300);
		WriteData((unsigned int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 76), 0xFFFFEE00);
		WriteData((unsigned int*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 84), 0xFFFFE300);
	}

	WriteData<1>((int*)(0x418873), 0x74);
	WriteData<1>((int*)(0x4403BA), 0x08);
	WriteData<1>((int*)(0x4403F9), 0x0C);
	WriteData<1>((int*)(0x4188BE), 0x02);

	for (int i = 0; i < 8; i++) {
		characterHasBeenSpawned[i] = false;
		headFrozen[i] = false;
	}
}

void writeHeadDisplay(int playerNo) {
	DisplayDebugStringFormatted(4, "Head Editor Mode: %d", headControlsMode[playerNo]);
	switch (headControlsMode[playerNo]) {
	case 0:
		if (headFrozen[playerNo]) {
			DisplayDebugString(5, "Custom Head Controls Enabled, Press A to reset.");
			DisplayDebugStringFormatted(6, "Current Face = %d", faceValue[GetCharacterID(playerNo)]);
			if (getEye()) DisplayDebugString(7, "Selected Eye = Left");
			else DisplayDebugString(7, "Selected Eye = Right");
		}
		else {
			if (getCanHeadFreeze(playerNo)) DisplayDebugString(5, "Custom Head Controls Disabled, Press A to activate.");
			else DisplayDebugString(5, "Eggman, Gamma and Super Sonic do not have this data.");

			if (halfface) DisplayDebugString(6, "Press D-Down to return the speed of face animations to normal."); 
			else DisplayDebugString(6, "Press D-Up to slow down faces and match event speed.");

			if (EntityData1Ptrs[playerNo]->CharID == Characters_Big) DisplayDebugString(7, "NOTE: Big cannot move his head, has limited eye movement and broken faces.");
		} break;
	case 1:
		DisplayDebugString(5, "Press A to focus the character's head on the camera's current position.");
		DisplayDebugString(6, "To reset, go to mode 0.");
		break;
	case 2:
		DisplayDebugString(5, "Auto Follow: Camera");
		break;
	}
	if (headControlsMode[playerNo] > 2) DisplayDebugStringFormatted(6, "Auto Follow: Player %d", headControlsMode[playerNo] - 2);
	if (getCanHeadFreeze(playerNo))DisplayDebugString(8, "Press X to read from face.txt.");
}


void doHeadFunctions(int playerNo, double moveSpeed, std::string faceFilePath, AnalogData analogData) {

	if (headControlsMode[playerNo] > (GetPlayerCount() + 2)) {
		headControlsMode[playerNo] = 0;
		goto StopLooking;
	}

	//Read from file to get face animation
	if (ControllerPointers[playerNo]->HeldButtons & Buttons_X) {
		std::ifstream subFile(faceFilePath);
		if (subFile.is_open())
		{
			std::getline(subFile, customFace);
			subFile.close();
		}
		EV_ClrFace(EV_GetPlayer(playerNo));
		if(customFace.length() > 0) EV_SetFace(EV_GetPlayer(playerNo), (char*)customFace.c_str());
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
	case Buttons_Up:
		if (!headFrozen[playerNo]) {
			halfface = true;
			for (int i = 0; i < faceTable_Length; i++) faceTable[i].nbFrame = facetbl2[i].nbFrame * 2;
		}
		break;
	case Buttons_Down:
		if (!headFrozen[playerNo]) {
			halfface = false;
			for (int i = 0; i < faceTable_Length; i++) faceTable[i].nbFrame = facetbl2[i].nbFrame;
		}
		break;
	case Buttons_A:
		switch (headControlsMode[playerNo]) {
		case 0:
			if (getCanHeadFreeze(playerNo) && !CutsceneMode) {
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
			if (faceValue[GetCharacterID(playerNo)] < 50 ||
				(faceValue[GetCharacterID(playerNo)] < 8 && EntityData1Ptrs[playerNo]->CharID == Characters_Big)) faceValue[GetCharacterID(playerNo)]++;
			else faceValue[GetCharacterID(playerNo)] = 0;
			EV_ClrFace(EV_GetPlayer(playerNo));
			break;
		case Buttons_Down: //Decrement Face
			if (faceValue[GetCharacterID(playerNo)] > 0) faceValue[GetCharacterID(playerNo)]--;
			else if (EntityData1Ptrs[playerNo]->CharID == Characters_Big) faceValue[GetCharacterID(playerNo)] = 7;
			else faceValue[GetCharacterID(playerNo)] = 50;
			EV_ClrFace(EV_GetPlayer(playerNo));
			break;
		default:
			break;
		}

		if (moveSpeed != 0.0) { //Head won't move below 1.0
			if (analogData.rightX > 3072.0)  WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 45), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 45) - (moveSpeed + 1.0)));
			if (analogData.rightX < -3072.0) WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 45), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 45) + (moveSpeed + 1.0)));

			if (analogData.rightY > 3072.0)  WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 53), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 53) - (moveSpeed + 1.0)));
			if (analogData.rightY < -3072.0) WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 53), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 53) + (moveSpeed + 1.0)));

			if (ControllerPointers[playerNo]->PressedButtons & Buttons_Right) eyeSelection = !eyeSelection;
			if (ControllerPointers[playerNo]->PressedButtons & Buttons_Left)  eyeSelection = !eyeSelection;

			//Eyes Y (Both use the same address)
			if (ControllerPointers[playerNo]->NotHeldButtons & Buttons_X) {
				if (analogData.leftY > 3072.0)  WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 85), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 85) + (moveSpeed + 1.0)));
				if (analogData.leftY < -3072.0) WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 85), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 85) - (moveSpeed + 1.0)));


				if (eyeSelection) { //Eyes X
					if (analogData.leftX > 3072.0)  WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 61), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 61) - (moveSpeed + 1.0)));
					if (analogData.leftX < -3072.0) WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 61), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 61) + (moveSpeed + 1.0)));
				}
				else {
					if (analogData.leftX > 3072.0)  WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 77), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 77) - (moveSpeed + 1.0)));
					if (analogData.leftX < -3072.0) WriteData((BYTE*)((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 77), (BYTE)(*((char*)headPointer[EntityData1Ptrs[playerNo]->CharID] + 77) + (moveSpeed + 1.0)));
				}
			}
		}
	}
	else {
		if (!anyHeadsFrozen()) {
			WriteData<1>((int*)(0x418873), 0x74);
			WriteData<1>((int*)(0x4403BA), 0x08);
			WriteData<1>((int*)(0x4403F9), 0x0C);
			WriteData<1>((int*)(0x4188BE), 0x02);
		}

	}

}