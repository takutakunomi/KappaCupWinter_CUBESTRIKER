#pragma once
#include "Enemy.h"

// 1面 ボス ニセオニ(Faux Oni)
class FauxOni : public Enemy
{
public:
	FauxOni();
	~FauxOni() {}

protected:
	const char* GetModelPath() override;
	EnemyCollider GetCollider() override;
};