#include "SADXModLoader.h"
#include "Functions.h"
#include <string>

int objindex = 0;
int subIndex = 0;
task* selection = 0;

static const std::string subMode[] = {
	"POSITION", "ROTATION", "SCALE"
};

void displayObjectSpawnerInfo() {
	DisplayDebugString(4, "Object Spawner (ROUGH)");
	DisplayDebugString(5, "Press A to spawn.");
	DisplayDebugStringFormatted(6, "Index: %d (0x%02X)", objindex, objindex);

	if (selection) {
		DisplayDebugStringFormatted(11, "SUB MODE: %s", subMode[subIndex].c_str());
		DisplayDebugStringFormatted(12, "ITEM NAME: %s", CurrentObjectList->List[objindex].Name);
		DisplayDebugStringFormatted(13, "POS X : %09.2f", selection->twp->pos.x);
		DisplayDebugStringFormatted(14, "POS Y : %09.2f", selection->twp->pos.y);
		DisplayDebugStringFormatted(15, "POS Z : %09.2f", selection->twp->pos.z);
		DisplayDebugStringFormatted(16, "ANG X : %09X", selection->twp->ang.x);
		DisplayDebugStringFormatted(17, "ANG Y : %09X", 0x4000 - selection->twp->ang.y);
		DisplayDebugStringFormatted(18, "ANG Z : %09X", selection->twp->ang.z);
		DisplayDebugStringFormatted(19, "SCL X : %09.2f", selection->twp->scl.x);
		DisplayDebugStringFormatted(20, "SCL Y : %09.2f", selection->twp->scl.y);
		DisplayDebugStringFormatted(21, "SCL Z : %09.2f", selection->twp->scl.z);
	}


}

void doObjectSpawner(int playerNo) {
	if (!selection || (ObjectFuncPtr)selection->exec != CurrentObjectList->List[objindex].LoadSub) {
		DestroyTask(selection);
		selection = 0;
		selection = (task*)LoadObject((LoadObj)CurrentObjectList->List[objindex].Flags, CurrentObjectList->List[objindex].ObjectListIndex, CurrentObjectList->List[objindex].LoadSub);
		OBJ_CONDITION* selectionocp = new OBJ_CONDITION();
		selection->ocp = selectionocp;
	}

	if (selection) {
		if ((ObjectFuncPtr)selection->exec == CurrentObjectList->List[objindex].LoadSub) {
			selection->twp->pos = PlayerPtrs[0]->Data1->Position;
			selection->twp->pos.y = PlayerPtrs[0]->Data1->Position.y + 20.0f;
			selection->twp->ang.x = PlayerPtrs[0]->Data1->Rotation.x;
			selection->twp->ang.y = 0x4000 - PlayerPtrs[0]->Data1->Rotation.y;
			selection->twp->ang.z = PlayerPtrs[0]->Data1->Rotation.z;
			selection->twp->scl = PlayerPtrs[0]->Data1->Scale;
			selection->twp->id = -1;
		}
	}


	switch (ControllerPointers[playerNo]->PressedButtons) {
	case Buttons_Up:
		if (objindex < CurrentObjectList->Count - 1) {
			objindex++;
		}
		else {
			objindex = 0;
		}
		break;
	case Buttons_Down:
		if (objindex > 0) {
			objindex--;
		}
		else {
			objindex = CurrentObjectList->Count - 1;
		}
		break;
	case Buttons_Left:
		if (ControllerPointers[playerNo]->HeldButtons & Buttons_X) {
			if (subIndex <= 1) {
				subIndex = 0;
			}
			else {
				subIndex = 1;
			}
		}
		else {
			if (objindex >= 10) {
				objindex -= 10;
			}
			else {
				if (objindex == 0) {
					objindex = CurrentObjectList->Count - 11;
				}
				else { objindex = 0; }
			}
		}

		break;
	case Buttons_Right:
		if (ControllerPointers[playerNo]->HeldButtons & Buttons_X) {
			if (subIndex >= 1) {
				subIndex = 2;
			}
			else {
				subIndex = 1;
			}
		}
		else {
			if (objindex <= CurrentObjectList->Count - 11) {
				objindex += 10;
			}
			else {
				if (objindex == CurrentObjectList->Count - 1) {
					objindex = 0;
				}
				else {
					objindex = CurrentObjectList->Count - 11;
				}

			}
		}
		break;
	case Buttons_A:
			task* newObject = (task*)LoadObject((LoadObj)CurrentObjectList->List[objindex].Flags, CurrentObjectList->List[objindex].ObjectListIndex, CurrentObjectList->List[objindex].LoadSub);
			OBJ_CONDITION* newObjectocp = new OBJ_CONDITION();
			newObject->ocp = newObjectocp;
			newObject->twp->pos = selection->twp->pos;
			newObject->twp->ang = selection->twp->ang;
			newObject->twp->scl = selection->twp->scl;
		break;
	}
}