#pragma once

class Enemy;

// 中ボス・ボス用の体力バー(画面の上)
// 下から順に「背景」「ダメージの残像」「いまのHP」の3枚を重ねて表示する
// 倒されると、バーごと消える
class BossHpBar : public IGameObject
{
public:
	BossHpBar() {}
	~BossHpBar() {}

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	// 体力を表示する相手を決める(NewGO の直後に呼ぶ)
	void SetTarget(Enemy* enemy) { target = enemy; }

private:
	void UpdateTrail(int hp, float hpRatio);               // ダメージの残像ゲージを動かす
	void UpdateGauge(SpriteRender& render, float ratio);   // ゲージの長さを、割合(0〜1)に合わせる

private:
	SpriteRender backRender;    // 背景(一番下)
	SpriteRender trailRender;   // ダメージの残像(真ん中)
	SpriteRender hpRender;      // いまのHP(一番上)

	Enemy* target = nullptr;    // 体力を表示する敵
	int previousHp = 0;         // 前のフレームのHP(減ったかを調べる用)
	float trailRatio = 1.0f;    // 残像ゲージの長さ(0〜1)
	float damageTimer = 0.0f;   // 残像を止めておく、残り時間(秒)
};