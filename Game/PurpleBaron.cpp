#include "stdafx.h"
#include "PurpleBaron.h"

namespace
{
	const char* MODEL_PATH = "Assets/modelData/seaofclouds/purplebaron/purplebaron.tkm";   // 実際のパスに変更
	const float SIDE_LENGTH = 300.0f;   // 本体の一辺(当たり判定)
	const int MAX_HP = 40;
	const int SCORE = 1000;
	const float STAY_TIME = 9999.0f;    // 倒されるまで去らない
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