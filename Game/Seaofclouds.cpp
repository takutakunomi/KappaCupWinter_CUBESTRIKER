#include "stdafx.h"
#include "Seaofclouds.h"
#include "Enemy.h"
#include "Orbspike.h"
#include "Grainspike.h"
#include "Pelletspike.h"
#include "PurpleBaron.h"
#include "FauxOni.h"

namespace
{
	const float ENEMY_Z = 1500.0f;      // 敵がいる奥行き(固定)

	const float MIDBOSS_TIME = 3.0f;   // 中ボスが出る時間(ステージ開始から)
	const float MIDBOSS_X = 0.0f;       // 中ボスの出現位置
	const float MIDBOSS_Y = 60.0f;

	const float BOSS_TIME = 3.0f;      // ボスが出る時間(中ボス撃破から)
	const float BOSS_X = 0.0f;          // ボスの出現位置
	const float BOSS_Y = 60.0f;

	// ---- 中ボスの前の出現予定(ステージ開始からの時間) ----
	// 書き方: { 時間, 敵の種類, X, Y }
	const SpawnData BEFORE_MIDBOSS[] =
	{
		{ 3.0f, enType_Orbspike,   -450.0f,   60.0f },
		{ 8.0f, enType_Orbspike,      0.0f,  240.0f },
		{ 13.0f, enType_Orbspike,    450.0f,   60.0f },

		{ 20.0f, enType_Grainspike,  450.0f,  240.0f },
		{ 25.0f, enType_Grainspike,    0.0f, -120.0f },
		{ 30.0f, enType_Grainspike, -450.0f,  240.0f },

		{ 35.0f, enType_Orbspike,   -450.0f,  240.0f },
		{ 35.0f, enType_Grainspike,  450.0f,  240.0f },

		{ 40.0f, enType_Orbspike,      0.0f, -120.0f },
		{ 40.0f, enType_Grainspike, -450.0f,   60.0f },

		{ 45.0f, enType_Orbspike,    450.0f, -120.0f },
		{ 45.0f, enType_Grainspike,    0.0f,  240.0f },

		// 50秒:中ボス(0, 60)は、まだ作っていない
	};

	// ---- 中ボス撃破後の出現予定(中ボスを倒してからの時間) ----
	const SpawnData AFTER_MIDBOSS[] =
	{
		{ 3.0f, enType_Orbspike,   -450.0f,   60.0f },
		{ 8.0f, enType_Grainspike,  450.0f,  240.0f },
		{ 13.0f, enType_Pelletspike,   0.0f, -120.0f },

		{ 18.0f, enType_Orbspike,   -450.0f,  240.0f },
		{ 18.0f, enType_Grainspike,  450.0f,  240.0f },

		{ 23.0f, enType_Pelletspike,   0.0f,  240.0f },
		{ 23.0f, enType_Orbspike,      0.0f, -120.0f },

		// 30秒後:ボス(三つ目大王)(0, 60)は、まだ作っていない
	};

	// 表の数(配列全体の大きさ ÷ 1件の大きさ)
	const int BEFORE_MIDBOSS_COUNT = sizeof(BEFORE_MIDBOSS) / sizeof(BEFORE_MIDBOSS[0]);
	const int AFTER_MIDBOSS_COUNT = sizeof(AFTER_MIDBOSS) / sizeof(AFTER_MIDBOSS[0]);
}

bool Seaofclouds::Start()
{
	return true;
}

void Seaofclouds::Update()
{
	phaseTimer += g_gameTime->GetFrameDeltaTime();

	switch (phase)
	{
	case enPhase_BeforeMidBoss:
		UpdateSpawn(BEFORE_MIDBOSS, BEFORE_MIDBOSS_COUNT);

		// 中ボスの時間になったら、中ボスを出す
		if (phaseTimer >= MIDBOSS_TIME)
		{
			SpawnMidBoss();
		}
		break;

	case enPhase_MidBoss:
		// 中ボスが倒されたら、撃破後の区間へ進む
		if (midBoss == nullptr || !midBoss->IsAlive())
		{
			midBoss = nullptr;
			StartAfterMidBoss();
		}
		break;

	case enPhase_AfterMidBoss:
		UpdateSpawn(AFTER_MIDBOSS, AFTER_MIDBOSS_COUNT);

		// ボスの時間になったら、ボスを出す
		if (phaseTimer >= BOSS_TIME)
		{
			SpawnBoss();
		}
		break;

	case enPhase_Boss:
		// ボスが倒されたら、ステージクリア
		if (boss == nullptr || !boss->IsAlive())
		{
			boss = nullptr;
			phase = enPhase_Clear;
		}
		break;

	case enPhase_Clear:
		// ステージクリアのあとの処理は、まだ作っていない
		break;
	}
}

// 時間が来た予定を、順番に出していく
void Seaofclouds::UpdateSpawn(const SpawnData* table, int tableCount)
{
	while (nextIndex < tableCount && table[nextIndex].time <= phaseTimer)
	{
		Spawn(table[nextIndex]);
		nextIndex++;
	}
}

// 敵を1体、予定の座標に出す
void Seaofclouds::Spawn(const SpawnData& data)
{
	Enemy* enemy = nullptr;

	switch (data.type)
	{
	case enType_Orbspike:
		enemy = NewGO<Orbspike>(0, "enemy");
		break;
	case enType_Grainspike:
		enemy = NewGO<Grainspike>(0, "enemy");
		break;
	case enType_Pelletspike:
		enemy = NewGO<Pelletspike>(0, "enemy");
		break;
	}

	if (enemy == nullptr)
	{
		return;
	}

	enemy->SetPosition(Vector3(data.x, data.y, ENEMY_Z));
}

// 中ボス「ムラサキング」を出す
void Seaofclouds::SpawnMidBoss()
{
	midBoss = NewGO<PurpleBaron>(0, "enemy");   // 名前を "enemy" にすると、プレイヤーの弾が当たる
	midBoss->SetPosition(Vector3(MIDBOSS_X, MIDBOSS_Y, ENEMY_Z));
	phase = enPhase_MidBoss;
}

// ボス「ニセオニ」を出す
void Seaofclouds::SpawnBoss()
{
	boss = NewGO<FauxOni>(0, "enemy");   // 名前を "enemy" にすると、プレイヤーの弾が当たる
	boss->SetPosition(Vector3(BOSS_X, BOSS_Y, ENEMY_Z));
	phase = enPhase_Boss;
}

// 中ボス撃破後の区間へ進む(時間と予定の番号を最初に戻す)
void Seaofclouds::StartAfterMidBoss()
{
	phase = enPhase_AfterMidBoss;
	phaseTimer = 0.0f;
	nextIndex = 0;
}