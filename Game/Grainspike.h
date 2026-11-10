#pragma once
#include "Enemy.h"

// 1面B コメトゲ
class Grainspike : public Enemy
{
public:
	Grainspike();
	~Grainspike() {}

protected:
	const char* GetModelPath() override;
	EnemyCollider GetCollider() override;
};