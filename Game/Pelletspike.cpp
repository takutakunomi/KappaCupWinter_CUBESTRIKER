#include "stdafx.h"
#include "Pelletspike.h"

namespace
{
	const float SIDE_LENGTH = 100.0f;   // 本体の一辺
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