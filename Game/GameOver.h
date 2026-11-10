#pragma once

// ゲームオーバー画面:画像を出して、Aボタンでタイトルへ戻る
class GameOver : public IGameObject
{
public:
	GameOver() {}
	~GameOver() {}

	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	void ReturnToTitle();   // ゲームを片づけて、タイトルに戻る

private:
	SpriteRender spriteRender;
	float waitTimer = 0.0f;   // 出てからの時間(秒)
};
