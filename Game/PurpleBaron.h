#pragma once
#include "Enemy.h"

// 1面 中ボス ムラサキング(Purple Baron)
//   HPが3分の1減るごとに、撃つ弾の種類が1つずつ増える
//   中玉 → 中玉と米玉 → 中玉と米玉と小玉
class PurpleBaron : public Enemy
{
public:
	static const int PHASE_COUNT = 3;   // 弾が増える段階の数(= 弾の種類の数)

	PurpleBaron();
	~PurpleBaron() {}

protected:
	const char* GetModelPath() override;
	EnemyCollider GetCollider() override;
	void OnUpdate() override;   // HPに合わせて弾を撃つ

private:
	float shotTimers[PHASE_COUNT] = {};   // 弾の種類ごとの、前に撃ってからの時間(秒)
};