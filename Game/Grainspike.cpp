#include "stdafx.h"
#include "Grainspike.h"

namespace
{
	const float SIDE_LENGTH = 100.0f;   // 本体の一辺
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