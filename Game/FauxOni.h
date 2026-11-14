#pragma once
#include "Enemy.h"

// 1面 ボス ニセオニ(Faux Oni)
//   HPが4分の1減るごとに、撃つ弾の種類が1つずつ増える
//   中玉 → 中玉と米玉 → 中玉と米玉と小玉 → 中玉と米玉と小玉と大玉
class FauxOni : public Enemy
{
public:
	static const int PHASE_COUNT = 4;   // 弾が増える段階の数(= 弾の種類の数)

	FauxOni();
	~FauxOni() {}

protected:
	const char* GetModelPath() override;
	EnemyCollider GetCollider() override;
	void OnUpdate() override;   // HPに合わせて弾を撃つ

private:
	float shotTimers[PHASE_COUNT] = {};   // 弾の種類ごとの、前に撃ってからの時間(秒)
};