#include "stdafx.h"
#include "Title.h"
#include "Game.h"

bool Title::Start()
{
	titleRender.Init("Assets/sprite/title/title.DDS", 1920.0f, 1080.0f);
	titleRender.Update();

	return true;
}

void Title::Update()
{
	// Aボタンでゲームへ
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		NewGO<Game>(0, "game");
		DeleteGO(this);
	}
}

void Title::Render(RenderContext& rc)
{
	titleRender.Draw(rc);
}