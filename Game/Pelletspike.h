#pragma once
#include "Enemy.h"

// 1面C ツブトゲ
class Pelletspike : public Enemy
{
public:
	Pelletspike();
	~Pelletspike() {}

protected:
	const char* GetModelPath() override;
	EnemyCollider GetCollider() override;

};