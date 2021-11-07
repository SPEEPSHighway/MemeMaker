#pragma once

int getAnimLimit(int playerNo);
void doBasicAnimation(int playerNo);
void doPlayerMovement(int playerNo, float movementSpeed, AnalogData analogData);
void setRunningAnim(int playerNo);
void displayPlayerInfo(int playerNo);

struct ENEMY_CART_DATA
{
	int flag;
	int ring_timer;
	float player_colli_r;
	int fan_angle;
	__int16 cart_color;
	__int16 invincible_timer;
	__int16 motion_timer;
	__int16 ignor_collision;
	__int16 drift_mode;
	__int16 drift_timer;
	int drift_angle;
	char vitality;
	Angle3 unstable;
	int loop_pos;
	float restless;
	float restless_timer;
	int smoke_timer;
	int drift_effect_timer;
	float shadow_pos;
	NJS_POINT3 vector;
	Angle3 shadow_ang;
	float max_spd;
	NJS_POINT3 add_key;
	NJS_POINT3 add_nature;
	float bonus_add;
	int rest_timer;
	unsigned __int8 next_point;
	unsigned __int8 load_line;
	unsigned __int8 load_indp[2];
	NJS_POINT3 last_pos;
	char cart_type;
	char start_wait;
	__int16 last_player_flag;
	char explose_flag[10];
	NJS_POINT3 explose_point[10];
	NJS_POINT3 explose_spd[10];
	Angle3 explose_angle[10];
	Angle3 explose_rotate[10];
	float tmp_posi[3];
	int tmp_angle[3];
	unsigned int hamari_cnt;
};

DataPointer(ENEMY_CART_DATA*, cart_data, 0x3D08E0C);
FunctionPointer(void, GetOutOfCartP, (char pno, float x, float y, float z), 0x441820);
DataPointer(task*, taskOfPlayerOn, 0x3D08E08);