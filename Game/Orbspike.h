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
	void OnUpdate() override;   // 弾を撃つ

private:
	float shotTimer = 0.0f;   // 前に撃ってからの時間(秒)
};