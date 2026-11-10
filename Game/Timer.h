#pragma once

class Timer : public IGameObject
{
public:
	Timer() {}
	~Timer() {}

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	// 経過秒数(整数)を返す。他のクラスから使いたいとき用
	int GetSeconds() const { return static_cast<int>(elapsedTime); }

private:
	FontRender timeFont;

	float elapsedTime = 0.0f;   // 経過時間(秒)
	int displaySeconds = -1;    // 表示中の秒数(変わったときだけ文字を更新する)
};