#include "stdafx.h"
#include "BulletPattern.h"
#include "Player.h"

namespace
{
	// 向きを、横方向(Y軸まわり)に degree 度回す
	Vector3 RotateYaw(const Vector3& direction, float degree)
	{
		Quaternion rotation;
		rotation.SetRotationDegY(degree);
		Vector3 result = direction;
		rotation.Apply(result);
		return result;
	}

	// 撃つ位置から、自機へ向かう向き(自機がいなければ、まっすぐ手前)
	Vector3 CalcAimDirection(const Vector3& from)
	{
		Player* player = FindGO<Player>("player");
		if (player == nullptr)
		{
			return Vector3(0.0f, 0.0f, -1.0f);
		}

		Vector3 direction = player->GetPosition() - from;
		direction.Normalize();
		return direction;
	}

	// 弾を1発撃つ
	void Fire(const Vector3& from, const Vector3& direction, EnBulletType type, float speed)
	{
		EnemyBullet* bullet = NewGO<EnemyBullet>(0, "enemybullet");
		bullet->Init(from, direction, type, speed);
	}
}

namespace BulletPattern
{
	void ShootAimed(const Vector3& from, EnBulletType type, float speed)
	{
		Fire(from, CalcAimDirection(from), type, speed);
	}

	void ShootFan(const Vector3& from, EnBulletType type, float speed, int count, float angleStep)
	{
		Vector3 center = CalcAimDirection(from);
		float firstAngle = -angleStep * (count - 1) * 0.5f;   // 3発なら -15, 0, +15

		for (int i = 0; i < count; i++)
		{
			Fire(from, RotateYaw(center, firstAngle + angleStep * i), type, speed);
		}
	}

	void ShootSpread(const Vector3& from, EnBulletType type, float speed, int count, float spreadAngle)
	{
		Vector3 center = CalcAimDirection(from);
		float half = spreadAngle * 0.5f;

		for (int i = 0; i < count; i++)
		{
			// -half 〜 +half のランダムな角度
			float yaw = ((float)rand() / RAND_MAX * 2.0f - 1.0f) * half;
			float pitch = ((float)rand() / RAND_MAX * 2.0f - 1.0f) * half;

			Quaternion pitchRotation;
			pitchRotation.SetRotationDegX(pitch);
			Vector3 direction = RotateYaw(center, yaw);
			pitchRotation.Apply(direction);

			Fire(from, direction, type, speed);
		}
	}
}