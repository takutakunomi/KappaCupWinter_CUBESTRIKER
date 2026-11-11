#include "stdafx.h"
#include "FauxOni.h"

namespace
{
	const char* MODEL_PATH = "Assets/modelData/seaofclouds/fauxoni/fauxoni.tkm";   // 実際のパスに変更
	const float SIDE_LENGTH = 450.0f;   // 本体の一辺(当たり判定)
	const int MAX_HP = 150;
	const int SCORE = 5000;
	const float STAY_TIME = 9999.0f;    // 倒されるまで去らない
}

FauxOni::FauxOni()
{
	maxHp = MAX_HP;
	score = SCORE;
	lifeTime = STAY_TIME;
}

const char* FauxOni::GetModelPath()
{
	return MODEL_PATH;
}

EnemyCollider FauxOni::GetCollider()
{
	return MakeBoxCollider(SIDE_LENGTH);
}
