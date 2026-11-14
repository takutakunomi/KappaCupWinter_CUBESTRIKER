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
		{ "Assets/sprite/enemybullet/seaofclouds/orbspike/orb_medium.DDS",       64.0f, 20.0f },   // 中玉・赤
		{ "Assets/sprite/enemybullet/seaofclouds/grainspike/grain_rice.DDS",     48.0f, 14.0f },   // 米弾・オレンジ
		{ "Assets/sprite/enemybullet/seaofclouds/pelletspike/pellet_small.DDS",  32.0f, 10.0f },   // 小玉・紫
		{ "Assets/sprite/enemybullet/seaofclouds/purplebaron/purple_medium.DDS", 64.0f, 20.0f },   // 中玉・紫
		{ "Assets/sprite/enemybullet/seaofclouds/purplebaron/purple_rice.DDS",   48.0f, 14.0f },   // 米弾・紫
		{ "Assets/sprite/enemybullet/seaofclouds/purplebaron/purple_small.DDS",  32.0f, 10.0f },   // 小玉・紫
		{ "Assets/sprite/enemybullet/seaofclouds/fauxoni/fake_large.DDS",        96.0f, 30.0f },   // 大玉
		{ "Assets/sprite/enemybullet/seaofclouds/fauxoni/fake_medium.DDS",       64.0f, 20.0f },   // 中玉
		{ "Assets/sprite/enemybullet/seaofclouds/fauxoni/fake_rice.DDS",         48.0f, 14.0f },   // 米弾
		{ "Assets/sprite/enemybullet/seaofclouds/fauxoni/fake_small.DDS",        32.0f, 10.0f },   // 小玉
	};

	const float LIFE_TIME = 8.0f;          // 弾が消えるまでの時間(秒)
	const float SCALE_DISTANCE = 1000.0f;  // この距離のとき、絵が等倍
	const float MIN_SCALE = 0.5f;          // 遠くでも、絵の大きさの半分は残す
	const float MAX_SCALE = 2.0f;          // 近づいても、絵の2倍までにする
	const int BOX_CORNER_COUNT = 8;        // 立方体の角の数

	// 画面上の1点(画面の中央が原点)
	struct ScreenPoint
	{
		float x = 0.0f;
		float y = 0.0f;
		float scale = 1.0f;   // 遠近の拡大率(カメラに近いほど大きい)
	};

	// 画面上の四角形
	struct ScreenBounds
	{
		float minX = FLT_MAX;
		float maxX = -FLT_MAX;
		float minY = FLT_MAX;
		float maxY = -FLT_MAX;
	};

	// 絵の拡大率を、見やすい範囲に収める
	float ClampScale(float scale)
	{
		return max(MIN_SCALE, min(MAX_SCALE, scale));
	}

	// 3D の位置を画面の位置に直す。カメラの後ろにあるときは false
	bool WorldToScreen(const Vector3& world, ScreenPoint& result)
	{
		Matrix viewProj = g_camera3D->GetViewProjectionMatrix();
		Vector4 clip(world.x, world.y, world.z, 1.0f);
		viewProj.Apply(clip);

		if (clip.w <= 0.0f)
		{
			return false;
		}

		result.x = clip.x / clip.w * g_graphicsEngine->GetFrameBufferWidth() * 0.5f;
		result.y = clip.y / clip.w * g_graphicsEngine->GetFrameBufferHeight() * 0.5f;
		result.scale = SCALE_DISTANCE / clip.w;
		return true;
	}

	// 立方体の角を1つ求める(番号の各ビットが、x・y・z で + 側か - 側かを決める)
	Vector3 CalcBoxCorner(const Vector3& center, float halfSize, int index)
	{
		Vector3 corner = center;
		corner.x += (index & 1) ? halfSize : -halfSize;
		corner.y += (index & 2) ? halfSize : -halfSize;
		corner.z += (index & 4) ? halfSize : -halfSize;
		return corner;
	}

	// 立方体が画面で占める四角形を求める。求められないときは false
	bool CalcBoxScreenBounds(const Vector3& center, float halfSize, ScreenBounds& bounds)
	{
		for (int i = 0; i < BOX_CORNER_COUNT; i++)
		{
			ScreenPoint corner;
			if (!WorldToScreen(CalcBoxCorner(center, halfSize, i), corner))
			{
				return false;
			}

			bounds.minX = min(bounds.minX, corner.x);
			bounds.maxX = max(bounds.maxX, corner.x);
			bounds.minY = min(bounds.minY, corner.y);
			bounds.maxY = max(bounds.maxY, corner.y);
		}
		return true;
	}

	// 点が、四角形を margin ずつ広げた範囲の中にあるか
	bool IsInsideBounds(const ScreenBounds& bounds, const ScreenPoint& point, float margin)
	{
		return point.x > bounds.minX - margin && point.x < bounds.maxX + margin
			&& point.y > bounds.minY - margin && point.y < bounds.maxY + margin;
	}

	// プレイヤーより奥にいて、画面上でプレイヤーの箱と少しでも重なっているか
	// (絵は3Dモデルの手前に描かれるので、そのままだと「当たった」ように見えてしまう)
	//   margin : 弾の絵の半分の大きさ。絵の端が箱にかかるだけでも、隠す
	bool IsHiddenByPlayer(const ScreenPoint& bulletPoint, float margin)
	{
		Player* player = FindGO<Player>("player");
		if (player == nullptr)
		{
			return false;
		}

		ScreenPoint playerPoint;
		if (!WorldToScreen(player->GetPosition(), playerPoint))
		{
			return false;
		}

		// 拡大率が大きいほど、カメラに近い。弾のほうが近ければ、隠れない
		bool isInFrontOfPlayer = bulletPoint.scale >= playerPoint.scale;
		if (isInFrontOfPlayer)
		{
			return false;
		}

		ScreenBounds playerBounds;
		if (!CalcBoxScreenBounds(player->GetPosition(), player->GetHalfSize(), playerBounds))
		{
			return false;
		}

		return IsInsideBounds(playerBounds, bulletPoint, margin);
	}
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
	lifeTimer += g_gameTime->GetFrameDeltaTime();

	if (IsExpired())
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

bool EnemyBullet::IsExpired() const
{
	return lifeTimer >= LIFE_TIME;
}

bool EnemyBullet::IsHitPlayer() const
{
	Player* player = FindGO<Player>("player");
	if (player == nullptr)
	{
		return false;
	}

	return player->IsHitSphere(position, BULLET_INFO[type].hitRadius);
}

void EnemyBullet::UpdateSprite()
{
	ScreenPoint point;
	isVisible = WorldToScreen(position, point);
	if (!isVisible)
	{
		return;
	}

	float scale = ClampScale(point.scale);
	float spriteHalfSize = BULLET_INFO[type].spriteSize * 0.5f * scale;

	isVisible = !IsHiddenByPlayer(point, spriteHalfSize);
	if (!isVisible)
	{
		return;
	}

	spriteRender.SetPosition(Vector3(point.x, point.y, 0.0f));
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