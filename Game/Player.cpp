#include "stdafx.h"
#include "Player.h"
#include "Enemy.h"

// このファイルだけで使う定数と関数
namespace
{
	// 移動
	const float MOVE_SPEED = 230.0f;        // 1秒に進む距離
	const float STICK_FULL_TILT = 0.85f;    // スティックがこの長さ以上なら最高速
	const float MOVE_LIMIT_X = 650.0f;
	const float MOVE_LIMIT_Y = 300.0f;

	// レティクル
	const float RETICLE_SPEED = 600.0f;     // 1秒に進むピクセル数
	const float RETICLE_DEAD_ZONE = 0.15f;  // これより小さい入力は無視
	const float RETICLE_LIMIT_X = 910.0f;
	const float RETICLE_LIMIT_Y = 490.0f;
	const float RETICLE_SIZE = 100.0f;
	const float AIM_DISTANCE = 2000.0f;     // 狙う点までの距離

	// 弾
	const float BULLET_SPEED = 6000.0f;      // 1秒に進む距離
	const float BULLET_RANGE = 3000.0f;     // ここまで飛んだら消える
	const float BULLET_RADIUS = 20.0f;      // 当たり判定の半径
	const float SHOT_INTERVAL = 0.08f;      // 発射の間隔(秒)
	const float MUZZLE_HEIGHT = 50.0f;      // 発射位置の高さ(プレイヤーの中心から)
	const float BULLET_SIZE = 64.0f;
	const float BULLET_SCALE_BASE = 600.0f; // 大きいほど弾が大きく見える
	const float BULLET_SCALE_MIN = 0.1f;
	const float BULLET_SCALE_MAX = 2.0f;

	// 被弾
	const float INVINCIBLE_TIME = 3.0f;   // 被弾したあとの無敵時間(秒)
	const float BLINK_SPEED = 10.0f;      // 無敵中の点滅の速さ

	// 値を min〜max の範囲に収める
	float Clamp(float value, float minValue, float maxValue)
	{
		return max(minValue, min(maxValue, value));
	}
}

bool Player::Start()
{
	modelRender.Init("Assets/modelData/player/player.tkm");

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bullets[i].render.Init("Assets/sprite/playerbullet/playerbullet.DDS", BULLET_SIZE, BULLET_SIZE);
	}

	reticleRender.Init("Assets/sprite/reticle/reticle.DDS", RETICLE_SIZE, RETICLE_SIZE);

	return true;
}

void Player::Update()
{
	// 無敵時間は、ライフが0でも進める(ゲームオーバーまでの点滅のため)
	UpdateInvincible();

	// ライフが0なら、操作できない
	if (life <= 0)
	{
		return;
	}

	Move();
	MoveReticle();
	UpdateShooting();
	UpdateBullets();
	UpdateInvincible();

	// 【テスト用】Bボタンで被弾(確認が終わったら消す)
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		Damage();
	}

	modelRender.Update();
}


// ---------- 被弾 ----------

void Player::Damage()
{
	// 無敵中や、ライフが0のときは、ダメージを受けない
	if (IsInvincible() || life <= 0)
	{
		return;
	}

	life--;
	invincibleTimer = INVINCIBLE_TIME;
}

void Player::UpdateInvincible()
{
	if (invincibleTimer > 0.0f)
	{
		invincibleTimer -= g_gameTime->GetFrameDeltaTime();
	}
}

// ---------- 移動 ----------

void Player::Move()
{
	// 左スティックの入力を読む
	Vector3 stick;
	stick.x = g_pad[0]->GetLStickXF();
	stick.y = g_pad[0]->GetLStickYF();
	stick.z = 0.0f;

	// 斜めに倒しても最高速になるように、スティックの長さを補正する
	float length = stick.Length();
	if (length > 0.0f)
	{
		float correctedLength = min(length / STICK_FULL_TILT, 1.0f);
		stick *= correctedLength / length;
	}

	// 速度 × 時間 だけ動かす
	position += stick * MOVE_SPEED * g_gameTime->GetFrameDeltaTime();

	// 移動できる範囲に収める
	position.x = Clamp(position.x, -MOVE_LIMIT_X, MOVE_LIMIT_X);
	position.y = Clamp(position.y, -MOVE_LIMIT_Y, MOVE_LIMIT_Y);

	modelRender.SetPosition(position);
}

// ---------- レティクル ----------

void Player::MoveReticle()
{
	// 右スティックの入力を読む
	float stickX = g_pad[0]->GetRStickXF();
	float stickY = g_pad[0]->GetRStickYF();

	// 小さな入力は無視する(スティックの勝手な揺れ対策)
	if (fabsf(stickX) < RETICLE_DEAD_ZONE)
	{
		stickX = 0.0f;
	}
	if (fabsf(stickY) < RETICLE_DEAD_ZONE)
	{
		stickY = 0.0f;
	}

	float distance = RETICLE_SPEED * g_gameTime->GetFrameDeltaTime();
	reticlePosition.x += stickX * distance;
	reticlePosition.y += stickY * distance;

	// 画面の外に出ないようにする
	reticlePosition.x = Clamp(reticlePosition.x, -RETICLE_LIMIT_X, RETICLE_LIMIT_X);
	reticlePosition.y = Clamp(reticlePosition.y, -RETICLE_LIMIT_Y, RETICLE_LIMIT_Y);

	reticleRender.SetPosition(reticlePosition);
	reticleRender.Update();
}

// レティクルが指す3D空間の点を求める
Vector3 Player::CalcAimTarget()
{
	// 画面座標を -1〜1 の範囲(NDC)に変換する
	float halfWidth = g_graphicsEngine->GetFrameBufferWidth() * 0.5f;
	float halfHeight = g_graphicsEngine->GetFrameBufferHeight() * 0.5f;
	float ndcX = reticlePosition.x / halfWidth;
	float ndcY = reticlePosition.y / halfHeight;

	// 画面の手前(z=0)と奥(z=1)の2点を、3D空間の座標に戻す
	Vector4 nearPoint(ndcX, ndcY, 0.0f, 1.0f);
	Vector4 farPoint(ndcX, ndcY, 1.0f, 1.0f);

	g_camera3D->GetViewProjectionMatrixInv().Apply(nearPoint);
	g_camera3D->GetViewProjectionMatrixInv().Apply(farPoint);

	Vector3 nearPos(nearPoint.x / nearPoint.w, nearPoint.y / nearPoint.w, nearPoint.z / nearPoint.w);
	Vector3 farPos(farPoint.x / farPoint.w, farPoint.y / farPoint.w, farPoint.z / farPoint.w);

	// 手前の点から奥の点へ向かう向きが、カメラからレティクルを通る線
	Vector3 rayDirection = farPos - nearPos;
	rayDirection.Normalize();

	// その線の上で、AIM_DISTANCE だけ進んだ点を狙う
	return nearPos + rayDirection * AIM_DISTANCE;
}

// ---------- 射撃 ----------

void Player::UpdateShooting()
{
	shotTimer -= g_gameTime->GetFrameDeltaTime();

	// Aボタンを押していて、発射の間隔が空いていれば撃つ
	if (!g_pad[0]->IsPress(enButtonRB2) || shotTimer > 0.0f)
	{
		return;
	}

	// 使っていない弾を探して撃つ
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (!bullets[i].isActive)
		{
			FireBullet(bullets[i]);
			shotTimer = SHOT_INTERVAL;
			return;
		}
	}
}

void Player::FireBullet(Bullet& bullet)
{
	// 発射位置
	bullet.position = position;
	bullet.position.y += MUZZLE_HEIGHT;
	bullet.startPosition = bullet.position;

	// 発射位置から、レティクルが指す点へ向かう向き
	bullet.direction = CalcAimTarget() - bullet.position;
	bullet.direction.Normalize();

	bullet.isActive = true;
	UpdateBulletSprite(bullet);   // 撃った瞬間から正しい位置に表示する
}

void Player::UpdateBullets()
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		Bullet& bullet = bullets[i];

		if (!bullet.isActive)
		{
			continue;
		}

		// 向きに沿って進める
		bullet.position += bullet.direction * BULLET_SPEED * g_gameTime->GetFrameDeltaTime();

		// 発射位置から遠くまで飛んだら消す
		Vector3 travelled = bullet.position - bullet.startPosition;
		if (travelled.Length() >= BULLET_RANGE)
		{
			bullet.isActive = false;
			continue;
		}

		// 敵に当たったか調べる
		bool isHit = false;
		const std::vector<Enemy*>& enemies = FindGOs<Enemy>("enemy");
		for (Enemy* enemy : enemies)
		{
			if (enemy->IsAlive() && enemy->IsHit(bullet.position, BULLET_RADIUS))
			{
				enemy->Damage(1);
				isHit = true;
				break;
			}
		}

		// 当たった弾は消す
		if (isHit)
		{
			bullet.isActive = false;
			continue;
		}

		UpdateBulletSprite(bullet);
	}
}

// 弾の3D空間での位置を、画面上の位置に変換して画像に反映する
void Player::UpdateBulletSprite(Bullet& bullet)
{
	Vector4 clipPos(bullet.position.x, bullet.position.y, bullet.position.z, 1.0f);
	g_camera3D->GetViewProjectionMatrix().Apply(clipPos);

	// カメラの後ろにある弾は表示しない
	if (clipPos.w <= 0.0f)
	{
		bullet.isVisible = false;
		return;
	}
	bullet.isVisible = true;

	// 画面上の位置(画面の中心が原点)
	float halfWidth = g_graphicsEngine->GetFrameBufferWidth() * 0.5f;
	float halfHeight = g_graphicsEngine->GetFrameBufferHeight() * 0.5f;
	float screenX = (clipPos.x / clipPos.w) * halfWidth;
	float screenY = (clipPos.y / clipPos.w) * halfHeight;

	// カメラから遠いほど小さく表示する(clipPos.w がカメラからの距離)
	float scale = Clamp(BULLET_SCALE_BASE / clipPos.w, BULLET_SCALE_MIN, BULLET_SCALE_MAX);

	bullet.render.SetPosition(Vector3(screenX, screenY, 0.0f));
	bullet.render.SetScale(Vector3(scale, scale, 1.0f));
	bullet.render.Update();
}

// ---------- 描画 ----------

void Player::Render(RenderContext& rc)
{
	// 無敵中は、モデルを点滅させる
	bool isBlinkHidden = IsInvincible() && (static_cast<int>(invincibleTimer * BLINK_SPEED) % 2 == 0);
	if (!isBlinkHidden)
	{
		modelRender.Draw(rc);
	}

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bullets[i].isActive && bullets[i].isVisible)
		{
			bullets[i].render.Draw(rc);
		}
	}

	reticleRender.Draw(rc);
}