#include "stdafx.h"
#include "GameOver.h"
#include "Game.h"
#include "Title.h"

namespace
{
	const char* SPRITE_PATH = "Assets/sprite/gameover/gameover.DDS";   // 実際のパスに変更
	const float SCREEN_WIDTH = 1920.0f;
	const float SCREEN_HEIGHT = 1080.0f;
	const float INPUT_WAIT = 1.0f;   // この時間(秒)がたつまで、ボタンを受け付けない
}

bool GameOver::Start()
{
	spriteRender.Init(SPRITE_PATH, SCREEN_WIDTH, SCREEN_HEIGHT);
	spriteRender.Update();
	return true;
}

void GameOver::Update()
{
	// 出た直後は、撃っていたAボタンで飛ばさないように少し待つ
	waitTimer += g_gameTime->GetFrameDeltaTime();
	if (waitTimer < INPUT_WAIT)
	{
		return;
	}

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		ReturnToTitle();
	}
}

void GameOver::ReturnToTitle()
{
	Game* game = FindGO<Game>("game");
	if (game != nullptr)
	{
		game->Finish();   // ゲーム中のものを全部消す
	}

	NewGO<Title>(0, "title");
	DeleteGO(this);
}

void GameOver::Render(RenderContext& rc)
{
	spriteRender.Draw(rc);
}