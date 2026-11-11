#pragma once

class Enemy;

// 敵の種類
enum EnEnemyType
{
	enType_Orbspike,      // ダマトゲ
	enType_Grainspike,    // コメトゲ
	enType_Pelletspike,   // ツブトゲ
};

// 出現の予定1件ぶん
struct SpawnData
{
	float time;          // 出現する時間(秒)
	EnEnemyType type;    // 敵の種類
	float x;             // 出現する X
	float y;             // 出現する Y
};

// 1面「昼の雲海」:時間になったら、予定どおりに敵を出す
class Seaofclouds : public IGameObject
{
public:
	Seaofclouds() {}
	~Seaofclouds() {}

	bool Start();
	void Update();

private:
	// ステージの区間
	enum EnPhase
	{
		enPhase_BeforeMidBoss,   // 中ボスの前
		enPhase_MidBoss,         // 中ボス戦
		enPhase_AfterMidBoss,    // 中ボス撃破後
		enPhase_Boss,            // ボス戦
		enPhase_Clear,           // ステージクリア
	};

	void UpdateSpawn(const SpawnData* table, int tableCount);   // 時間が来た予定の敵を出す
	void Spawn(const SpawnData& data);                           // 敵を1体出す
	void StartAfterMidBoss();                                    // 中ボス撃破後の区間へ進む
	void SpawnMidBoss();                                         // 中ボスを出す
	void SpawnBoss();                                            // ボスを出す

private:
	EnPhase phase = enPhase_BeforeMidBoss;
	float phaseTimer = 0.0f;   // いまの区間が始まってからの経過時間(秒)
	int nextIndex = 0;         // 次に出す予定の番号
	Enemy* midBoss = nullptr;  // 出している中ボス
	Enemy* boss = nullptr;     // 出しているボス
};