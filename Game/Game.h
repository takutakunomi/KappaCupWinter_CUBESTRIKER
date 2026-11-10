#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class GameCamera;
class BackGround;
class GameUI;
class Timer;
class Score;

class Game : public IGameObject
{
public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Finish();   // ゲームを終わらせる(ゲーム中のものを全部消す)

	Player* player;
	GameCamera* gameCamera;
	BackGround* backGround;
	GameUI* gameUI;
	Timer* timer;
	Score* score;

	bool isGameOver = false;   // ゲームオーバー画面を出したか
};

