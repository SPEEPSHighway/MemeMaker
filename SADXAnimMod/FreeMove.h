#pragma once

int getAnimLimit(int playerNo);
void doBasicAnimation(int playerNo);
void doPlayerMovement(int playerNo, float movementSpeed, AnalogData analogData);
void setRunningAnim(int playerNo);
void displayPlayerInfo(int playerNo);