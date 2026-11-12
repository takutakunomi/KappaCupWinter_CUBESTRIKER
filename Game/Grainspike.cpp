#include "stdafx.h"
#include "Grainspike.h"

namespace
{
	const float SIDE_LENGTH = 100.0f;   // 本体の一辺
	const float SHOT_INTERVAL = 2.5f;   // 撃つ間隔(秒)
	const float BULLET_SPEED = 450.0f;  // 弾の速さ
	const int FAN_COUNT = 3;            // 1回に撃つ数
	const float FAN_ANGLE = 15.0f;      // 弾と弾の間の角度(度)
}

Grainspike::Grainspike()
{
	maxHp = 1;
	score = 120;
}

const char* Grainspike::GetModelPath()
{
	return "Assets/modelData/seaofclouds/grainspike/grainspike.tkm";
}

EnemyCollider Grainspike::GetCollider()
{
	return MakeBoxCollider(SIDE_LENGTH);
}

