#include "stdafx.h"
#include "Timer.h"

namespace
{
	const float FONT_POS_X = -660.0f;   // 枠の右側の暗い部分に重ねる位置
	const float FONT_POS_Y = 400.0f;
	const float FONT_SCALE = 1.2f;
}

bool Timer::Start()
{
	timeFont.SetPosition(Vector3(FONT_POS_X, FONT_POS_Y, 0.0f));
	timeFont.SetScale(FONT_SCALE);
	timeFont.SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	return true;
}

void Timer::Update()
{
	// 経過時間を進める
	elapsedTime += g_gameTime->GetFrameDeltaTime();

	// 秒数が変わったときだけ、表示する文字を作り直す
	int seconds = static_cast<int>(elapsedTime);
	if (seconds != displaySeconds)
	{
		displaySeconds = seconds;

		wchar_t text[16];
		swprintf_s(text, 16, L"%d", seconds);
		timeFont.SetText(text);
	}
}

void Timer::Render(RenderContext& rc)
{
	timeFont.Draw(rc);
}