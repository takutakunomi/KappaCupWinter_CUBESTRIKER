#pragma once
#include "Enemy.h"

// 1面A ダマトゲ:止まって撃つ
class Orbspike : public Enemy
{
public:
	Orbspike();
	~Orbspike() {}

protected:
	const char* GetModelPath() override;
	EnemyCollider GetCollider() override;
};