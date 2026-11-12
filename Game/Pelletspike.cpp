#include "stdafx.h"
#include "Pelletspike.h"
#include "BulletPattern.h"

namespace
{
	const float SIDE_LENGTH = 100.0f;   // 本体の一辺
	const float SHOT_INTERVAL = 1.5f;   // 撃つ間隔(秒)
	const float BULLET_SPEED = 350.0f;  // 弾の速さ
	const int SPREAD_COUNT = 11;         // 1回に撃つ数
	const float SPREAD_ANGLE = 40.0f;   // ばらける角度(度)
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

// 一定時間ごとに、自機の方向へ小玉をばらまく
void Pelletspike::OnUpdate()
{
	if (CountUpTimer(shotTimer, SHOT_INTERVAL))
	{
		BulletPattern::ShootSpread(position, enBullet_PelletSmall, BULLET_SPEED, SPREAD_COUNT, SPREAD_ANGLE);
	}
}