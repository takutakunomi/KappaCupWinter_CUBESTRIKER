#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "Score.h"

namespace
{
	const float MODEL_ROTATION_Y = 180.0f;   // モデルを回す角度(度)
	// 値を min〜max の範囲に収める
	float Clamp(float value, float minValue, float maxValue)
	{
		return max(minValue, min(maxValue, value));
	}
}

// ---------- 初期化 ----------

bool Enemy::Start()
{
	hp = maxHp;
	state = enState_Alive;
	elapsedTime = 0.0f;

	// 当たり判定とモデルは、子クラスから受け取る
	collider = GetCollider();
	modelRender.Init(GetModelPath());

	// モデルが反対側を向いているので、Y軸まわりに回して向きを直す
	Quaternion rotation;
	rotation.SetRotationDegY(MODEL_ROTATION_Y);
	modelRender.SetRotation(rotation);

	UpdateModel();

	return true;
}

// ---------- 更新 ----------

void Enemy::Update()
{
	// 撃破済みなら、ここで消える
	if (state == enState_Defeated)
	{
		DeleteGO(this);
		return;
	}

	// 決まった時間がたったら、去る
	elapsedTime += g_gameTime->GetFrameDeltaTime();
	if (elapsedTime >= lifeTime)
	{
		DeleteGO(this);
		return;
	}

	OnUpdate();   // 子クラスの追加処理(弾を撃つなど)

	if (IsHitPlayer())
	{
		OnHitPlayer();
	}

	UpdateModel();
}

void Enemy::UpdateModel()
{
	modelRender.SetPosition(position);
	modelRender.Update();
}

// ---------- ダメージと撃破(共通) ----------

void Enemy::Damage(int damage)
{
	// 撃破済みの敵には、ダメージを与えない
	if (state != enState_Alive)
	{
		return;
	}

	hp -= damage;
	CheckDefeat();
}

// HP が 0 以下なら撃破にする
void Enemy::CheckDefeat()
{
	if (hp > 0)
	{
		return;
	}

	hp = 0;
	Defeat();
}

void Enemy::Defeat()
{
	state = enState_Defeated;

	// 得点を加える
	Score* scoreObject = FindGO<Score>("score");
	if (scoreObject != nullptr)
	{
		scoreObject->AddScore(score);
	}

	OnDefeated();   // 子クラスの追加処理(スコア加算など、後で使う)
}

// ---------- 当たり判定(共通) ----------

// 敵の当たり判定と、相手の球(位置と半径)が重なっているか
bool Enemy::IsHit(const Vector3& targetPos, float targetRadius) const
{
	if (collider.shape == enShape_Sphere)
	{
		return IsHitSphere(targetPos, targetRadius);
	}

	return IsHitBox(targetPos, targetRadius);
}

// 判定の中心 = 敵の位置 + モデルに合わせたずれ
Vector3 Enemy::CalcColliderCenter() const
{
	return position + collider.offset;
}

// 球どうし:中心の距離が、半径の合計より近ければ当たり
bool Enemy::IsHitSphere(const Vector3& targetPos, float targetRadius) const
{
	float distance = (targetPos - CalcColliderCenter()).Length();
	return distance < collider.size.x + targetRadius;
}

// 箱と球:箱の上で球の中心にいちばん近い点を探し、その距離が半径より近ければ当たり
bool Enemy::IsHitBox(const Vector3& targetPos, float targetRadius) const
{
	Vector3 local = targetPos - CalcColliderCenter();

	Vector3 closest;
	closest.x = Clamp(local.x, -collider.size.x, collider.size.x);
	closest.y = Clamp(local.y, -collider.size.y, collider.size.y);
	closest.z = Clamp(local.z, -collider.size.z, collider.size.z);

	float distance = (local - closest).Length();
	return distance < targetRadius;
}

bool Enemy::IsHitPlayer() const
{
	Player* player = FindPlayer();
	if (player == nullptr)
	{
		return false;
	}

	// 敵の判定の形に合わせて、プレイヤー(箱)と重なっているかを聞く
	if (collider.shape == enShape_Sphere)
	{
		return player->IsHitSphere(CalcColliderCenter(), collider.size.x);
	}

	return player->IsHitBox(CalcColliderCenter(), collider.size);
}

Player* Enemy::FindPlayer() const
{
	return FindGO<Player>("player");
}

// 一辺の長さから、立方体の当たり判定を作る
EnemyCollider Enemy::MakeBoxCollider(float sideLength) const
{
	float half = sideLength * 0.5f;   // size は中心からの半分の長さ

	EnemyCollider box;
	box.shape = enShape_Box;
	box.size = Vector3(half, half, half);
	box.offset = Vector3::Zero;       // 原点が本体の中心なので、ずれなし
	return box;
}

// 時間を進めて、interval 秒たったら true を返す(そのとき timer は 0 に戻る)
bool Enemy::CountUpTimer(float& timer, float interval) const
{
	timer += g_gameTime->GetFrameDeltaTime();
	if (timer < interval)
	{
		return false;
	}

	timer = 0.0f;
	return true;
}

// ---------- 描画 ----------

void Enemy::Render(RenderContext& rc)
{
	// 撃破済みの敵は描かない
	if (state == enState_Alive)
	{
		modelRender.Draw(rc);
	}
}