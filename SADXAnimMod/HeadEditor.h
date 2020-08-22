#pragma once
#include <string>

void* getHeadData(int characterID);
void* getHeadDataS(); 
void* getHeadDataT(); 
void* getHeadDataK(); 
void* getHeadDataTi(); 
void* getHeadDataA(); 
void* getHeadDataB(); 

void doHeadFunctions(int playerNo, double moveSpeed, std::string faceFilePath, AnalogData analogData);
bool anyHeadsFrozen();
bool getCanHeadFreeze(int playerNo);

bool getEye();
void* getHeadPointer(int characterID);
void doHeadChecks(int playerNo);
void stopHeadEdits(int playerNo);
void writeHeadDisplay(int playerNo);
