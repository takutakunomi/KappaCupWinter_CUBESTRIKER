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
	void OnUpdate() override;   // 弾を撃つ

private:
	float shotTimer = 0.0f;   // 前に撃ってからの時間(秒)
};