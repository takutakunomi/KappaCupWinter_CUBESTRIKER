#pragma once

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
		enPhase_AfterMidBoss,    // 中ボス撃破後
	};

	void UpdateSpawn(const SpawnData* table, int tableCount);   // 時間が来た予定の敵を出す
	void Spawn(const SpawnData& data);                           // 敵を1体出す
	void StartAfterMidBoss();                                    // 中ボス撃破後の区間へ進む

private:
	EnPhase phase = enPhase_BeforeMidBoss;
	float phaseTimer = 0.0f;   // いまの区間が始まってからの経過時間(秒)
	int nextIndex = 0;         // 次に出す予定の番号
};