#pragma once

class Player : public IGameObject
{
public:
	Player() {}
	~Player() {}

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	// ---- 敵などが使う情報 ----
	const Vector3& GetPosition() const { return position; }
	float GetRadius() const { return radius; }   // 当たり判定の半径
	int GetLife() const { return life; }
	bool IsInvincible() const { return invincibleTimer > 0.0f; }
	void Damage();   // 被弾する(無敵中は何も起きない)

private:
	// 弾1発ぶんのデータ
	struct Bullet
	{
		Vector3 position = Vector3::Zero;       // 現在位置
		Vector3 startPosition = Vector3::Zero;  // 発射した位置
		Vector3 direction = Vector3::Zero;      // 飛ぶ向き
		bool isActive = false;                  // 飛んでいるか
		bool isVisible = false;                 // 画面に表示するか
		SpriteRender render;                    // 弾の画像
	};

	void Move();                              // プレイヤーの移動
	void MoveReticle();                       // レティクルの移動
	Vector3 CalcAimTarget();                  // レティクルが指す3D空間の点を求める
	void UpdateShooting();                    // 発射の判断
	void FireBullet(Bullet& bullet);          // 弾を1発撃つ
	void UpdateBullets();                     // 飛んでいる弾の更新
	void UpdateBulletSprite(Bullet& bullet);  // 弾の3D位置を画面に反映
	void UpdateInvincible();                  // 無敵時間を進める

private:
	static const int MAX_BULLETS = 64;

	// プレイヤー
	ModelRender modelRender;
	Vector3 position = Vector3::Zero;
	float radius = 40.0f;   // 当たり判定の半径(モデルの見た目より少し小さめ)
	int life = 3;                   // ライフ
	float invincibleTimer = 0.0f;   // 無敵の残り時間(秒)

	// 弾
	Bullet bullets[MAX_BULLETS];
	float shotTimer = 0.0f;

	// レティクル
	SpriteRender reticleRender;
	Vector3 reticlePosition = Vector3::Zero;
};