#include "stdafx.h"
#include "Score.h"

namespace
{
	const float FONT_POS_X = -660.0f;   // スコア枠の右側の暗い部分に重ねる位置
	const float FONT_POS_Y = 490.0f;
	const float FONT_SCALE = 1.2f;
	const int MAX_SCORE = 99999999;     // これ以上は増えない
}

bool Score::Start()
{
	scoreFont.SetPosition(Vector3(FONT_POS_X, FONT_POS_Y, 0.0f));
	scoreFont.SetScale(FONT_SCALE);
	scoreFont.SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	UpdateText();

	return true;
}

void Score::AddScore(int value)
{
	score += value;

	// 上限を超えないようにする
	if (score > MAX_SCORE)
	{
		score = MAX_SCORE;
	}

	UpdateText();
}

void Score::UpdateText()
{
	wchar_t text[16];
	swprintf_s(text, 16, L"%d", score);
	scoreFont.SetText(text);
}

void Score::Render(RenderContext& rc)
{
	scoreFont.Draw(rc);
}