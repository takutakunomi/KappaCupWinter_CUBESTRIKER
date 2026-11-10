#include "stdafx.h"
#include "Orbspike.h"

namespace
{
	const float SIDE_LENGTH = 100.0f;   // 本体の一辺
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