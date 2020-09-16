#include "SADXModLoader.h"

ObjectFunc(EV_ClrAction, 0x42FE40);
FunctionPointer(void, EV_SetAction, (ObjectMaster* tp, NJS_ACTION* ap, NJS_TEXLIST* lp, float speed, int mode, int linkframe), 0x42FE00);
FunctionPointer(void, EV_SetMotion, (ObjectMaster* tp, NJS_OBJECT* op, NJS_MOTION* mp, NJS_TEXLIST* lp, float speed, int mode, int linkframe), 0x42FE20);
FunctionPointer(ObjectMaster*, EV_GetPlayer, (int no), 0x42FC40);
FunctionPointer(void, EV_LookPoint, (ObjectMaster* tp, float x, float y, float z), 0x430000);
ObjectFunc(EV_LookFree, 0x42FFB0);
FunctionPointer(void, EV_SetFace, (ObjectMaster* tp, char* str), 0x4310D0);
DataPointer(NJS_MOTION**, SS_PEOPLE_MOTIONS, 0x38F6EA4);
ObjectFunc(EV_ClrFace, 0x4310F0);
FunctionPointer(void, PFaceChange, (int pno, int face), 0x440740);
FunctionPointer(void, PFaceCancel, (int pno), 0x440770);
FunctionPointer(void, CreateChaos0, (float pos_x, float pos_y, float pos_z,
	int ang_x, int ang_y, int ang_z, signed int cng_int), 0x6EE930);
VoidFunc(DeleteChaos0, 0x6EDAB0);
FunctionPointer(ObjectMaster*, GetChaos0, (), 0x6EDCD0);
FunctionPointer(void, SetSclChaos0, (float scl_x, float scl_y, float scl_z), 0x6EDDB0);


struct FACETBL
{
	BYTE nbFrame;
	BYTE faceNo;
};

DataArray(FACETBL, faceTable, 0x91CEC8, 160);

struct AnalogData {
	long double leftX;
	long double leftY;
	long double rightX;
	long double rightY;
};