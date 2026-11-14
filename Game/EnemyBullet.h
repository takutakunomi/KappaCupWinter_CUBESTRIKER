#pragma once

// 弾の種類(形と色)。名前は、絵のファイル名に合わせてある
// 絵・大きさ・当たり判定は、EnemyBullet.cpp の表に書いてある(同じ順番で並べる)
// 弾を増やすときは、ここと表に1行ずつ足す
enum EnBulletType
{
	enBullet_OrbMedium,   // 中玉・赤         ダマトゲ
	enBullet_GrainRice,   // 米弾・オレンジ   コメトゲ
	enBullet_PelletSmall, // 小玉・紫       ツブトゲ
	enBullet_PurpleMedium, // 中玉・紫         ムラサキング
	enBullet_PurpleRice,   // 米弾・紫         ムラサキング
	enBullet_PurpleSmall,  // 小玉・紫         ムラサキング
	enBullet_FakeLarge,    // 大玉             ニセオニ
	enBullet_FakeMedium,   // 中玉             ニセオニ
	enBullet_FakeRice,     // 米弾             ニセオニ
	enBullet_FakeSmall,    // 小玉             ニセオニ
	enBullet_Count,       // 種類の数(表の大きさ)
};

// 敵の弾(1発ぶん)。まっすぐ飛んで、プレイヤーに当たるか、時間がたつと消える
class EnemyBullet : public IGameObject
{
public:
	EnemyBullet() {}
	~EnemyBullet() {}

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	// 発射位置、向き、弾の種類、速さを決める(NewGO の直後に呼ぶ)
	void Init(const Vector3& startPos, const Vector3& direction, EnBulletType bulletType, float speed);

private:
	bool IsExpired() const;     // 飛び続ける時間が終わったか
	bool IsHitPlayer() const;   // プレイヤーに当たったか
	void UpdateSprite();        // 3D の位置を、画面の位置に直して絵を動かす

private:
	SpriteRender spriteRender;
	EnBulletType type = enBullet_OrbMedium;   // 弾の種類
	Vector3 position = Vector3::Zero;         // 3D 空間での位置
	Vector3 velocity = Vector3::Zero;         // 1秒あたりの移動量
	float lifeTimer = 0.0f;                   // 発射してからの時間(秒)
	bool isVisible = true;                    // 画面に描くか
};