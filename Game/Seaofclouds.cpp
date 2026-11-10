#include "stdafx.h"
#include "Seaofclouds.h"
#include "Enemy.h"
#include "Orbspike.h"
#include "Grainspike.h"
#include "Pelletspike.h"

namespace
{
	const float ENEMY_Z = 1500.0f;      // 敵がいる奥行き(固定)
	const float MIDBOSS_TIME = 50.0f;   // 中ボスが出る時間(ステージ開始から)

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

	if (phase == enPhase_BeforeMidBoss)
	{
		UpdateSpawn(BEFORE_MIDBOSS, BEFORE_MIDBOSS_COUNT);

		// 中ボスの時間になったら、撃破後の区間へ進む
		// (中ボスを作ったら、「時間」ではなく「中ボスが倒されたとき」に切り替える)
		if (phaseTimer >= MIDBOSS_TIME)
		{
			StartAfterMidBoss();
		}
		return;
	}

	UpdateSpawn(AFTER_MIDBOSS, AFTER_MIDBOSS_COUNT);
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

// 中ボス撃破後の区間へ進む(時間と予定の番号を最初に戻す)
void Seaofclouds::StartAfterMidBoss()
{
	phase = enPhase_AfterMidBoss;
	phaseTimer = 0.0f;
	nextIndex = 0;
}