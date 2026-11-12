#include "stdafx.h"
#include "Orbspike.h"
#include "BulletPattern.h"

namespace
{
	const float SIDE_LENGTH = 100.0f;   // 本体の一辺
	const float SHOT_INTERVAL = 2.0f;   // 撃つ間隔(秒)
	const float BULLET_SPEED = 500.0f;  // 弾の速さ
}

Orbspike::Orbspike()
{
	maxHp = 1;
	score = 100;
}

const char* Orbspike::GetModelPath()
{
	return "Assets/modelData/seaofclouds/orbspike/orbspike.tkm";   
}

EnemyCollider Orbspike::GetCollider()
{
	return MakeBoxCollider(SIDE_LENGTH);
}

// 一定時間ごとに、自機に向けて中玉を1発撃つ
void Orbspike::OnUpdate()
{
	if (CountUpTimer(shotTimer, SHOT_INTERVAL))
	{
		BulletPattern::ShootAimed(position, enBullet_OrbMedium, BULLET_SPEED);
	}
}