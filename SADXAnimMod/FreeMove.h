#pragma once

int getAnimLimit(int playerNo);
void doBasicAnimation(int playerNo);
void doPlayerMovement(int playerNo, float movementSpeed, AnalogData analogData);
void resetRunningAnim();
void displayPlayerInfo(int playerNo);