#include "stdafx.h"
#include "PurpleBaron.h"
#include "BulletPattern.h"

namespace
{
	const char* MODEL_PATH = "Assets/modelData/seaofclouds/purplebaron/purplebaron.tkm";
	const float SIDE_LENGTH = 300.0f;   // 本体の一辺(当たり判定)
	const int MAX_HP = 40;
	const int SCORE = 1000;
	const float STAY_TIME = 9999.0f;    // 倒されるまで去らない

	// ---- 弾の撃ち方(1行が1種類。HPが減るごとに、上から順に撃ち始める) ----
	enum ShotPattern
	{
		enPattern_Aimed,    // 自機狙い
		enPattern_Fan,      // 扇状
		enPattern_Spread,   // ばらまき
	};

	struct ShotSetting
	{
		EnBulletType type;   // 弾の種類(見た目)
		ShotPattern pattern; // 撃ち方
		float interval;      // 撃つ間隔(秒)
		float speed;         // 弾の速さ
		int count;           // 1回に撃つ数
		float angle;         // 扇状: 弾と弾の間の角度 / ばらまき: ばらける範囲(度)
	};

	const ShotSetting SHOT_SETTINGS[PurpleBaron::PHASE_COUNT] =
	{
		{ enBullet_PurpleMedium, enPattern_Aimed,  1.0f, 500.0f,  1,  0.0f },   // 1段階目から: 中玉(自機狙い)
		{ enBullet_PurpleRice,   enPattern_Fan,    1.5f, 450.0f,  5, 15.0f },   // 2段階目から: 米玉(扇状5way)
		{ enBullet_PurpleSmall,  enPattern_Spread, 1.2f, 350.0f, 15, 50.0f },   // 3段階目から: 小玉(ばらまき)
	};

	// 減ったHPから、今が何段階目かを求める(0 〜 PHASE_COUNT-1)
	int CalcPhaseIndex(int hp, int maxHp)
	{
		int lostHp = maxHp - hp;
		int index = lostHp * PurpleBaron::PHASE_COUNT / maxHp;
		return min(index, PurpleBaron::PHASE_COUNT - 1);
	}

	// 設定の撃ち方で、1回撃つ
	void Shoot(const Vector3& from, const ShotSetting& setting)
	{
		switch (setting.pattern)
		{
		case enPattern_Aimed:
			BulletPattern::ShootAimed(from, setting.type, setting.speed);
			break;
		case enPattern_Fan:
			BulletPattern::ShootFan(from, setting.type, setting.speed, setting.count, setting.angle);
			break;
		case enPattern_Spread:
			BulletPattern::ShootSpread(from, setting.type, setting.speed, setting.count, setting.angle);
			break;
		}
	}
}

PurpleBaron::PurpleBaron()
{
	maxHp = MAX_HP;
	score = SCORE;
	lifeTime = STAY_TIME;
}

const char* PurpleBaron::GetModelPath()
{
	return MODEL_PATH;
}

EnemyCollider PurpleBaron::GetCollider()
{
	return MakeBoxCollider(SIDE_LENGTH);
}

// 進んだ段階の数だけ、上の弾から順に撃つ(中玉 → 中玉と米玉 → 中玉と米玉と小玉)
void PurpleBaron::OnUpdate()
{
	int activeCount = CalcPhaseIndex(GetHp(), GetMaxHp()) + 1;

	for (int i = 0; i < activeCount; i++)
	{
		if (CountUpTimer(shotTimers[i], SHOT_SETTINGS[i].interval))
		{
			Shoot(position, SHOT_SETTINGS[i]);
		}
	}
}