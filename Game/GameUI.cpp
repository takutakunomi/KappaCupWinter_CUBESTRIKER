#include "stdafx.h"
#include "GameUI.h"
#include "Player.h"

bool GameUI::Start()
{
	// ライフ(画面左下)
	const float heartSize = 64.0f;
	const float heartInterval = 80.0f;
	const float startX = -860.0f;
	const float startY = -470.0f;

	for (int i = 0; i < MAX_LIFE; i++)
	{
		Vector3 heartPos(startX + heartInterval * i, startY, 0.0f);

		// 残っているライフ(赤いハート)
		heartRender[i].Init("Assets/sprite/life/full.DDS", heartSize, heartSize);   // 実際のパスに変更
		heartRender[i].SetPosition(heartPos);
		heartRender[i].Update();

		// 失ったライフ(黒いハート)。赤いハートと同じ位置に置く
		emptyHeartRender[i].Init("Assets/sprite/life/empty.DDS", heartSize, heartSize);   // 実際のパスに変更
		emptyHeartRender[i].SetPosition(heartPos);
		emptyHeartRender[i].Update();
	}

	// ボム(ライフの上)
	const float bombSize = 64.0f;
	const float bombInterval = 80.0f;
	const float bombStartX = -860.0f;
	const float bombStartY = -390.0f;

	for (int i = 0; i < MAX_BOMB; i++)
	{
		bombRender[i].Init("Assets/sprite/bomb/full.DDS", bombSize, bombSize);   // 実際のパスに変更
		bombRender[i].SetPosition(Vector3(bombStartX + bombInterval * i, bombStartY, 0.0f));
		bombRender[i].Update();
	}

	// スコア枠(画面左上)
	const float scoreW = 400.0f;
	const float scoreH = 76.0f;
	const float scoreX = -740.0f;   // 画像の中心
	const float scoreY = 470.0f;

	scoreRender.Init("Assets/sprite/score/score.DDS", scoreW, scoreH);
	scoreRender.SetPosition(Vector3(scoreX, scoreY, 0.0f));
	scoreRender.Update();

	// タイム枠(スコア枠の下)
	const float timeW = 400.0f;
	const float timeH = 76.0f;
	const float timeX = -740.0f;    // スコアと同じ中心X
	const float timeY = 380.0f;     // スコア(470)の下に90ずらす

	timeRender.Init("Assets/sprite/time/time.DDS", timeW, timeH);
	timeRender.SetPosition(Vector3(timeX, timeY, 0.0f));
	timeRender.Update();

	return true;
}

void GameUI::Update()
{
	// プレイヤーのライフを、表示に反映する
	Player* player = FindGO<Player>("player");
	if (player != nullptr)
	{
		displayLife = player->GetLife();
	}
}

void GameUI::Render(RenderContext& rc)
{
	// ライフ(残っている数だけ赤、失った分は黒)
	for (int i = 0; i < MAX_LIFE; i++)
	{
		if (i < displayLife)
		{
			heartRender[i].Draw(rc);
		}
		else
		{
			emptyHeartRender[i].Draw(rc);
		}
	}

	//ボム
	for (int i = 0; i < MAX_BOMB; i++)
	{
		bombRender[i].Draw(rc);
	}

	//スコア
	scoreRender.Draw(rc);

	//タイム
	timeRender.Draw(rc);
}