#include "SADXModLoader.h"
#include <math.h>
#include "Functions.h"
#include <string>

int cameraMode = 0;

static const std::string cameraModeList[] = {
	"NORMAL", "FREE MOVEMENT", "FOLLOW FROM FIXED POINT", "NO VANILLA CAMERAS"
};

void displayCameraInfo() {
	DisplayDebugStringFormatted(4, "Camera Mode: %s", cameraModeList[cameraMode].c_str());
	DisplayDebugStringFormatted(6, "CamPosX = %f", Camera_Data1->Position.x);
	DisplayDebugStringFormatted(7, "CamPosY = %f", Camera_Data1->Position.y);
	DisplayDebugStringFormatted(8, "CamPosZ = %f", Camera_Data1->Position.z);
	DisplayDebugStringFormatted(10, "CamRotX = %d", Camera_Data1->Rotation.x);
	DisplayDebugStringFormatted(11, "CamRotY = %d", Camera_Data1->Rotation.y);
	DisplayDebugStringFormatted(12, "CamRotZ = %d", Camera_Data1->Rotation.z);
}

void resetCamera() {
	WriteData<1>((int*)0x437760, 0x74); //Enable normal cameras
	WriteData<1>((int*)0x436308, 0x8F); //Enable first person cam
	cameraMode = 0;
}

int getCamMode() {
	return cameraMode;
}

void checkCameraMode() {
	//Always Default Cam
	if (cameraMode != 0) {
		CameraType[0] = 0;
		CameraType[2] = 0;
		WriteData((int*)0x3B2CAE3, 0x462E9002);
		WriteData((int*)0x3B2CAE7, 0x467D8000);

		//Force Auto Cam because it breaks otherwise.
		WriteData<1>((int*)0x3B2CBA8, 4); //camera_flags
	}

	switch (cameraMode) {
	case 0: CameraType[1] =  2; break;
	case 1: CameraType[1] =  0; break;
	case 2: CameraType[1] =  2; break;
	case 3: CameraType[1] =  1; break;
	}

}

void doFreeCam(int playerNo, float movementSpeed, AnalogData analogData) {
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
			if (analogData.leftY > 3072.0 || analogData.leftY < -3072.0) Camera_Data1->Position.y = Camera_Data1->Position.y - analogData.leftY / squareroot((analogData.leftY * analogData.leftY)) * (movementSpeed / 2);

			//Y Rotation
			if (analogData.rightX > 3072.0)  Camera_Data1->Rotation.z = (int)(Camera_Data1->Rotation.z - (movementSpeed * 100));
			if (analogData.rightX < -3072.0) Camera_Data1->Rotation.z = (int)(Camera_Data1->Rotation.z + (movementSpeed * 100));

		}
		else {
			if (analogData.leftX > 3072.0 || analogData.leftX < -3072.0 || analogData.leftY > 3072.0 || analogData.leftY < -3072.0) {
				Angle v10 = -Camera_Data1->Rotation.y - (unsigned __int64)(atan2(analogData.leftY, analogData.leftX) * 65536.0 * -0.1591549762031479);
				Camera_Data1->Position.x = njCos(v10) * (movementSpeed / 2) + Camera_Data1->Position.x;
				Camera_Data1->Position.z = njSin(v10) * (movementSpeed / 2) + Camera_Data1->Position.z;
			}

			if (analogData.rightY > 3072.0)  Camera_Data1->Rotation.x = (int)(Camera_Data1->Rotation.x - (movementSpeed * 100));
			if (analogData.rightY < -3072.0) Camera_Data1->Rotation.x = (int)(Camera_Data1->Rotation.x + (movementSpeed * 100));
			if (analogData.rightX > 3072.0)  Camera_Data1->Rotation.y = (int)(Camera_Data1->Rotation.y - (movementSpeed * 100));
			if (analogData.rightX < -3072.0) Camera_Data1->Rotation.y = (int)(Camera_Data1->Rotation.y + (movementSpeed * 100));
		}
	}
}
