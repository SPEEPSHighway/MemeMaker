#include "SADXModLoader.h"

ObjectFunc(EV_ClrAction, 0x42FE40);
FunctionPointer(void, EV_SetAction, (ObjectMaster* tp, NJS_ACTION* ap, NJS_TEXLIST* lp, float speed, int mode, int linkframe), 0x42FE00);
FunctionPointer(void, EV_SetMotion, (ObjectMaster* tp, NJS_OBJECT* op, NJS_MOTION* mp, NJS_TEXLIST* lp, float speed, int mode, int linkframe), 0x42FE20);
FunctionPointer(ObjectMaster*, EV_GetPlayer, (int no), 0x42FC40);
FunctionPointer(void, EV_LookPoint, (ObjectMaster* tp, float x, float y, float z), 0x430000);
ObjectFunc(EV_LookFree, 0x42FFB0);
FunctionPointer(void, EV_SetFace, (ObjectMaster* tp, char* str), 0x4310D0);
DataPointer(NJS_MOTION**, SS_PEOPLE_MOTIONS, 0x38F6EA4);