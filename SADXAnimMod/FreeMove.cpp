#include "SADXModLoader.h"
#include <math.h>
#include "Functions.h"

int runningAnim = 0;
HMODULE SUPERSONICMOD = GetModuleHandle(L"sadx-super-sonic");

int getAnimLimit(int playerNo) {
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

void resetRunningAnim() {
	runningAnim = 0;
}

void displayPlayerInfo(int playerNo) {
	DisplayDebugStringFormatted(4, "Current Animation = %d", CharObj2Ptrs[playerNo]->AnimationThing.Index);
	DisplayDebugStringFormatted(6, "PosX = %f", EntityData1Ptrs[playerNo]->Position.x);
	DisplayDebugStringFormatted(7, "PosY = %f", EntityData1Ptrs[playerNo]->Position.y);
	DisplayDebugStringFormatted(8, "PosZ = %f", EntityData1Ptrs[playerNo]->Position.z);
	DisplayDebugStringFormatted(10, "RotX = %d", EntityData1Ptrs[playerNo]->Rotation.x);
	DisplayDebugStringFormatted(11, "RotY = %d", 0x4000 - EntityData1Ptrs[playerNo]->Rotation.y);
	DisplayDebugStringFormatted(12, "RotZ = %d", EntityData1Ptrs[playerNo]->Rotation.z);
}

void doBasicAnimation(int playerNo) {
	CharObj2Ptrs[playerNo]->AnimationThing.Index = runningAnim; //Stop the game moving to the next animation in certain cases.

	switch (ControllerPointers[playerNo]->PressedButtons) {
	case Buttons_Right:
		if (runningAnim == getAnimLimit(playerNo)) runningAnim = 0; else runningAnim += 1;
		break;
	case Buttons_Left:
		if (runningAnim > 0) runningAnim -= 1; //
		break;
	case Buttons_Up:
		if (runningAnim > (getAnimLimit(playerNo) - 10)) runningAnim = 0; else runningAnim += 10;
		break;
	case Buttons_Down:
		if (runningAnim > 10) runningAnim -= 10; else runningAnim = 0;
		break;
	default:
		break;
	}
}

void doPlayerMovement(int playerNo, float movementSpeed, AnalogData analogData) {
	//AKA Shamelessly copy the debug movement code lol

	if (ControllerPointers[playerNo]->HeldButtons & Buttons_X) {
		//Y Position
		if (analogData.leftY > 25072.0 || analogData.leftY < -25072.0) EntityData1Ptrs[playerNo]->Position.y =
			EntityData1Ptrs[playerNo]->Position.y - analogData.leftY / squareroot((analogData.leftY * analogData.leftY)) * movementSpeed;

		//Y Rotation
		if (analogData.rightX > 25072.0) EntityData1Ptrs[playerNo]->Rotation.y = (int)(EntityData1Ptrs[playerNo]->Rotation.y - (movementSpeed * 100));
		if (analogData.rightX < -25072.0) EntityData1Ptrs[playerNo]->Rotation.y = (int)(EntityData1Ptrs[playerNo]->Rotation.y + (movementSpeed * 100));
	}
	else {
		if (analogData.leftX > 3072.0 || analogData.leftX < -3072.0 || analogData.leftY > 3072.0 || analogData.leftY < -3072.0) {
			Angle v10 = -Camera_Data1->Rotation.y - (unsigned __int64)(atan2(analogData.leftY, analogData.leftX) * 65536.0 * -0.1591549762031479);
			EntityData1Ptrs[playerNo]->Position.x = njCos(v10) * movementSpeed + EntityData1Ptrs[playerNo]->Position.x;
			EntityData1Ptrs[playerNo]->Position.z = njSin(v10) * movementSpeed + EntityData1Ptrs[playerNo]->Position.z;
		}

		if (analogData.rightX > 3072.0 || analogData.rightX < -3072.0 || analogData.rightY > 3072.0 || analogData.rightY < -3072.0) {
			Angle v10 = -Camera_Data1->Rotation.y - (unsigned __int64)(atan2(analogData.rightY, analogData.rightX) * 65536.0 * -0.1591549762031479);
			EntityData1Ptrs[playerNo]->Rotation.x = (signed int)(njSin(v10) * (movementSpeed * 100) + EntityData1Ptrs[playerNo]->Rotation.x);
			EntityData1Ptrs[playerNo]->Rotation.z = (signed int)(njCos(v10) * (movementSpeed * -100) + EntityData1Ptrs[playerNo]->Rotation.z);

		}
	}
}
