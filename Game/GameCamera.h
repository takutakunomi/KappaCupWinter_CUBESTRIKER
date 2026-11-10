#pragma once

class Player;

class GameCamera : public IGameObject
{
public:
	GameCamera() {}
	~GameCamera() {}

	bool Start();

	void Update();
	void Render(RenderContext& rc);

	Player* player;
	Vector3 toCameraPos;
};

