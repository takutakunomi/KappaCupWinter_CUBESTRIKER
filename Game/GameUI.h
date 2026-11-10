#pragma once
class GameUI : public IGameObject
{
public:
	GameUI() {}
	~GameUI() {}

	bool Start();

	void Update();
	void Render(RenderContext& rc);

	static const int MAX_LIFE = 3;
	static const int MAX_BOMB = 3;

	// ライフ表示(ハートを横に並べる)
	SpriteRender heartRender[MAX_LIFE];        // 残っているライフ(赤)
	SpriteRender emptyHeartRender[MAX_LIFE];   // 失ったライフ(黒)
	int displayLife = MAX_LIFE;                // いま残っているライフの数

	//ボム表示
	SpriteRender bombRender[MAX_BOMB];

	//スコア表示
	SpriteRender scoreRender;

	//タイム表示
	SpriteRender timeRender;
};