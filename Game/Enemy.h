#pragma once

// 当たり判定の形
enum EnColliderShape
{
	enShape_Sphere,   // 球
	enShape_Box,      // 箱
};

// 当たり判定の情報(敵ごとに子クラスが決める)
struct EnemyCollider
{
	EnColliderShape shape = enShape_Sphere;
	Vector3 size = Vector3(60.0f, 60.0f, 60.0f);   // 球: x が半径 / 箱: x,y,z が中心からの半分の長さ
	Vector3 offset = Vector3::Zero;                // モデルの原点からのずれ(判定の中心位置)
};

class Player;

// 敵の親クラス(抽象クラス)。単体では作れない
class Enemy : public IGameObject
{
public:
	// 敵の状態
	enum EnState
	{
		enState_Alive,      // 生存中
		enState_Defeated,   // 撃破済み(次のフレームで消える)
	};

	Enemy() {}
	virtual ~Enemy() {}

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	// 出現位置を決める(NewGO の直後に呼ぶ)
	void SetPosition(const Vector3& pos) { position = pos; }

	// ---- 情報の取得 ----
	const Vector3& GetPosition() const { return position; }
	int GetHp() const { return hp; }
	int GetMaxHp() const { return maxHp; }
	int GetScore() const { return score; }
	bool IsAlive() const { return state == enState_Alive; }

	// ---- ダメージ(共通) ----
	void Damage(int damage);

	// ---- 当たり判定(共通) ----
	bool IsHit(const Vector3& targetPos, float targetRadius) const;
	bool IsHitPlayer() const;

protected:
	// ---- 子クラスが必ず書くもの(純粋仮想関数) ----
	virtual const char* GetModelPath() = 0;     // モデルのパス
	virtual EnemyCollider GetCollider() = 0;    // 当たり判定の形・大きさ・位置

	// ---- 子クラスが書いてもよいもの ----
	virtual void OnDefeated() {}                // 撃破されたときの追加処理
	virtual void OnHitPlayer() {}               // プレイヤーに触れたときの処理

	// ---- 子クラスが使える道具 ----
	// 一辺の長さから、立方体の当たり判定を作る(原点が本体の中心の場合)
	EnemyCollider MakeBoxCollider(float sideLength) const;

protected:
	// 敵ごとの数値(子クラスのコンストラクタで設定する)
	Vector3 position = Vector3::Zero;
	int maxHp = 3;              // 最大HP
	int score = 100;            // 撃破したときの得点
	float lifeTime = 10.0f;     // 出現してから去るまでの時間(秒)

private:
	// ---- 撃破 ----
	void CheckDefeat();
	void Defeat();

	// ---- 当たり判定の部品 ----
	Vector3 CalcColliderCenter() const;
	bool IsHitSphere(const Vector3& targetPos, float targetRadius) const;
	bool IsHitBox(const Vector3& targetPos, float targetRadius) const;

	// ---- その他 ----
	Player* FindPlayer() const;
	void UpdateModel();

private:
	ModelRender modelRender;
	EnemyCollider collider;        // Start で子クラスから受け取る
	EnState state = enState_Alive;
	int hp = 0;                    // 現在のHP(Start で maxHp になる)
	float elapsedTime = 0.0f;      // 出現してからの経過時間(秒)
};