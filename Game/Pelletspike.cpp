#include "stdafx.h"
#include "Pelletspike.h"

namespace
{
	const float SIDE_LENGTH = 100.0f;   // 本体の一辺
	const float SHOT_INTERVAL = 2.5f;   // 撃つ間隔(秒)
	const float BULLET_SPEED = 350.0f;  // 弾の速さ
	const int SPREAD_COUNT = 8;         // 1回に撃つ数
	const float SPREAD_ANGLE = 25.0f;   // ばらける角度(度)
}

Pelletspike::Pelletspike()
{
	maxHp = 1;
	score = 120;
}

const char* Pelletspike::GetModelPath()
{
	return "Assets/modelData/seaofclouds/pelletspike/pelletspike.tkm";   
}

EnemyCollider Pelletspike::GetCollider()
{
	return MakeBoxCollider(SIDE_LENGTH);
}

