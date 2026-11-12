#pragma once

// ステージの背景(雲海)
//   空 : 奥に立てた大きな板。動かない
//   床 : 空の下に敷いた雲の床。奥から手前へ流れ続け、同じ雲が繰り返される
class BackGround : public IGameObject
{
public:
	BackGround() {}
	~BackGround() {}

	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	void InitUnlit(ModelRender& render, const char* modelPath, const char* shaderPath, AlphaBlendMode blendMode);   // ライトの影響を受けない(絵そのままの色で出る)モデルとして読み込む
	void InitLight();
	void InitSky();      // 空の板を置く
	void InitFloor();    // 床の板を読み込む
	void UpdateFloor();  // 床の板を、流れた量に合わせてずらす

private:
	ModelRender skyRender;                         // 空
	ModelRender floorRender;                       // 雲の床(長い板1枚)
	float scrollZ = 0.0f;                          // 床が流れた量(0 〜 絵1枚の奥行き)
};