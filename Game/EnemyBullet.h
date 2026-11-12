#pragma once

// 敵の弾(1発ぶん)。まっすぐ飛んで、プレイヤーに当たるか、時間がたつと消える
class EnemyBullet : public IGameObject
{
public:
	EnemyBullet() {}
	~EnemyBullet() {}

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	// 発射位置、向き、速さを決める(NewGO の直後に呼ぶ)
	void Init(const Vector3& startPos, const Vector3& direction, float speed);

private:
	bool IsHitPlayer() const;   // プレイヤーに当たったか
	void UpdateSprite();        // 3D の位置を、画面の位置に直して絵を動かす

private:
	SpriteRender spriteRender;
	Vector3 position = Vector3::Zero;   // 3D 空間での位置
	Vector3 velocity = Vector3::Zero;   // 1秒あたりの移動量
	float lifeTimer = 0.0f;             // 発射してからの時間(秒)
	bool isVisible = true;              // カメラの前にいるか
};