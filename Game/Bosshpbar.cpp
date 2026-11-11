#include "stdafx.h"
#include "BossHpBar.h"
#include "Enemy.h"

namespace
{
	// ---- 絵 ----
	const char* BACK_PATH = "Assets/sprite/hpbar/backgroundbar.DDS";   // 背景
	const char* TRAIL_PATH = "Assets/sprite/hpbar/damagebar.DDS";      // ダメージの残像
	const char* HP_PATH = "Assets/sprite/hpbar/hpbar.DDS";             // いまのHP

	// ---- 大きさと位置(画面中央が原点) ----
	const float BAR_WIDTH = 1000.0f;
	const float BAR_HEIGHT = 24.0f;
	const float BAR_CENTER_X = 0.0f;
	const float BAR_Y = 480.0f;   // 画面の上

	// ---- 残像の動き ----
	const float TRAIL_DELAY = 0.5f;   // 最後のダメージから、残像が減り始めるまでの時間(秒)
	const float TRAIL_SPEED = 0.4f;   // 残像が減る速さ(1秒に、全体の何割か)
}

bool BossHpBar::Start()
{
	backRender.Init(BACK_PATH, BAR_WIDTH, BAR_HEIGHT);
	trailRender.Init(TRAIL_PATH, BAR_WIDTH, BAR_HEIGHT);
	hpRender.Init(HP_PATH, BAR_WIDTH, BAR_HEIGHT);

	previousHp = target->GetHp();

	UpdateGauge(backRender, 1.0f);   // 背景は、いつも全部
	UpdateGauge(trailRender, trailRatio);
	UpdateGauge(hpRender, 1.0f);
	return true;
}

void BossHpBar::Update()
{
	// 倒されたら、バーも消える
	if (target == nullptr || !target->IsAlive())
	{
		DeleteGO(this);
		return;
	}

	int hp = target->GetHp();
	float hpRatio = static_cast<float>(hp) / target->GetMaxHp();

	UpdateTrail(hp, hpRatio);
	UpdateGauge(trailRender, trailRatio);
	UpdateGauge(hpRender, hpRatio);
}

// 攻撃を受けている間は残像を止めておき、受けなくなったら、いまのHPまでゆっくり減らす
void BossHpBar::UpdateTrail(int hp, float hpRatio)
{
	bool isDamaged = (hp < previousHp);
	previousHp = hp;

	if (isDamaged)
	{
		damageTimer = TRAIL_DELAY;
	}

	// 残像がHPより短くなっていたら(回復など)、HPに合わせる
	if (trailRatio < hpRatio)
	{
		trailRatio = hpRatio;
		return;
	}

	// 攻撃を受けている間は、残像を止めておく
	if (damageTimer > 0.0f)
	{
		damageTimer -= g_gameTime->GetFrameDeltaTime();
		return;
	}

	// 攻撃を受けなくなったら、いまのHPまで減らす
	trailRatio -= TRAIL_SPEED * g_gameTime->GetFrameDeltaTime();
	if (trailRatio < hpRatio)
	{
		trailRatio = hpRatio;
	}
}

// 左端を固定して、右へ伸び縮みするように見せる
void BossHpBar::UpdateGauge(SpriteRender& render, float ratio)
{
	ratio = max(0.0f, min(1.0f, ratio));

	float leftX = BAR_CENTER_X - BAR_WIDTH * 0.5f;
	float centerX = leftX + BAR_WIDTH * ratio * 0.5f;   // 伸び縮みした絵の中心

	render.SetPosition(Vector3(centerX, BAR_Y, 0.0f));
	render.SetScale(Vector3(ratio, 1.0f, 1.0f));
	render.Update();
}

void BossHpBar::Render(RenderContext& rc)
{
	// 下から順に重ねる
	backRender.Draw(rc);
	trailRender.Draw(rc);
	hpRender.Draw(rc);
}