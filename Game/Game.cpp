#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "GameUI.h"
#include "Timer.h"
#include "Score.h"
#include "Seaofclouds.h"
#include "Enemy.h"
#include "GameOver.h"

bool Game::Start()
{
	player = NewGO<Player>(0, "player");

	gameCamera = NewGO<GameCamera>(0, "gamecamra");

	//backGround = NewGO<BackGround>(0, "background");

	gameUI = NewGO<GameUI>(0, "gameui");

	timer = NewGO<Timer>(0, "timer");

	score = NewGO<Score>(1, "score");   

	NewGO<Seaofclouds>(0, "seaofclouds");
	
	return true;
}

void Game::Update()
{
	// ライフが0になり、最後の点滅が終わったら、ゲームオーバーにする
	if (!isGameOver && player->GetLife() <= 0 && !player->IsInvincible())
	{
		isGameOver = true;
		NewGO<GameOver>(3, "gameover");
	}
}

void Game::Render(RenderContext& rc)
{
	
}


// ゲームを終わらせる:ゲーム中のものを全部消して、最後に自分も消える
void Game::Finish()
{
	DeleteGO(player);
	DeleteGO(gameCamera);
	DeleteGO(gameUI);
	DeleteGO(timer);
	DeleteGO(score);
	DeleteGO(FindGO<Seaofclouds>("seaofclouds"));

	for (Enemy* enemy : FindGOs<Enemy>("enemy"))
	{
		DeleteGO(enemy);
	}

	DeleteGO(this);
}