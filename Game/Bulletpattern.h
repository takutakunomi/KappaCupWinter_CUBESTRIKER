#pragma once
#include "EnemyBullet.h"

// 敵の弾の撃ち方。どの敵からでも使える
//   from  : 撃つ位置(敵の位置)
//   type  : 弾の種類
//   speed : 弾の速さ(1秒に進む距離)
namespace BulletPattern
{
	// 自機に向けて、1発撃つ
	void ShootAimed(const Vector3& from, EnBulletType type, float speed);

	// 自機の方向を真ん中にして、横に扇状に広げて撃つ
	//   count     : 撃つ数(3なら「扇状3way」)
	//   angleStep : 弾と弾の間の角度(度)
	void ShootFan(const Vector3& from, EnBulletType type, float speed, int count, float angleStep);

	// 自機の方向を中心に、ランダムな向きへ count 発ばらまく
	//   spreadAngle : ばらける範囲の角度(度)。40なら、左右・上下に±20度
	void ShootSpread(const Vector3& from, EnBulletType type, float speed, int count, float spreadAngle);
}