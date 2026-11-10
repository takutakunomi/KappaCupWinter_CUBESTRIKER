#pragma once

// 得点を数えて、枠の上に表示するクラス
class Score : public IGameObject
{
public:
	Score() {}
	~Score() {}

	bool Start();
	void Render(RenderContext& rc);

	// 得点を加える
	void AddScore(int value);

	int GetScore() const { return score; }

private:
	void UpdateText();   // 表示する文字を作り直す

private:
	FontRender scoreFont;
	int score = 0;
};
