#pragma once
#include "Enemy.h"

// 1面 中ボス ムラサキング(Purple Baron)
class PurpleBaron : public Enemy
{
public:
	PurpleBaron();
	~PurpleBaron() {}

protected:
	const char* GetModelPath() override;
	EnemyCollider GetCollider() override;
};