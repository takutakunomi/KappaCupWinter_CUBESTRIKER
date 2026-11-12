#include "stdafx.h"
#include "EnemyBullet.h"
#include "Player.h"

namespace
{
	// 弾の種類ごとの情報
	struct BulletInfo
	{
		const char* path;   // 絵のパス
		float spriteSize;   // 絵の大きさ(ピクセル)
		float hitRadius;    // 当たり判定の半径
	};

	// enum EnBulletType と同じ順番で書く
	const BulletInfo BULLET_INFO[enBullet_Count] =
	{
		{ "Assets/sprite/enemybullet/seaofclouds/orbspike/orb_medium.DDS",    64.0f, 20.0f },   // 中玉・赤
		{ "Assets/sprite/enemybullet/seaofclouds/grainspike/grain_rice.DDS",  48.0f, 14.0f },   // 米弾・オレンジ
	};

	const float LIFE_TIME = 8.0f;          // 弾が消えるまでの時間(秒)
	const float SCALE_DISTANCE = 1000.0f;  // この距離のとき、絵が等倍
}

bool EnemyBullet::Start()
{
	const BulletInfo& info = BULLET_INFO[type];
	spriteRender.Init(info.path, info.spriteSize, info.spriteSize);
	UpdateSprite();
	return true;
}

// 向きは長さ1にそろえて、速さをかける
void EnemyBullet::Init(const Vector3& startPos, const Vector3& direction, EnBulletType bulletType, float speed)
{
	position = startPos;
	type = bulletType;

	Vector3 dir = direction;
	dir.Normalize();
	velocity = dir * speed;
}

void EnemyBullet::Update()
{
	position += velocity * g_gameTime->GetFrameDeltaTime();

	// 時間がたったら消える
	lifeTimer += g_gameTime->GetFrameDeltaTime();
	if (lifeTimer >= LIFE_TIME)
	{
		DeleteGO(this);
		return;
	}

	// プレイヤーに当たったら、ダメージを与えて消える
	if (IsHitPlayer())
	{
		FindGO<Player>("player")->Damage();
		DeleteGO(this);
		return;
	}

	UpdateSprite();
}

bool EnemyBullet::IsHitPlayer() const
{
	Player* player = FindGO<Player>("player");
	if (player == nullptr)
	{
		return false;
	}

	float distance = (player->GetPosition() - position).Length();
	return distance < player->GetRadius() + BULLET_INFO[type].hitRadius;
}

// 3D の位置 → 画面の位置(画面中央が原点)。遠いほど小さく見せる
void EnemyBullet::UpdateSprite()
{
	Matrix viewProj = g_camera3D->GetViewProjectionMatrix();
	Vector4 clip(position.x, position.y, position.z, 1.0f);
	viewProj.Apply(clip);

	// w が 0 以下 = カメラの後ろなので描かない
	isVisible = (clip.w > 0.0f);
	if (!isVisible)
	{
		return;
	}

	float screenX = clip.x / clip.w * g_graphicsEngine->GetFrameBufferWidth() * 0.5f;
	float screenY = clip.y / clip.w * g_graphicsEngine->GetFrameBufferHeight() * 0.5f;
	float scale = SCALE_DISTANCE / clip.w;

	spriteRender.SetPosition(Vector3(screenX, screenY, 0.0f));
	spriteRender.SetScale(Vector3(scale, scale, 1.0f));
	spriteRender.Update();
}

void EnemyBullet::Render(RenderContext& rc)
{
	if (isVisible)
	{
		spriteRender.Draw(rc);
	}
}