#include "stdafx.h"
#include "BackGround.h"

namespace
{
	const char* UNLIT_SHADER_PATH = "Assets/shader/unlit.fx";             // ライトを使わず、絵の色をそのまま出すシェーダー
	const char* FADE_SHADER_PATH = "Assets/shader/unlitfade.fx";          // 上に加えて、奥にいくほど透明にするシェーダー(床用)

	// ---- 空 ----
	const char* SKY_MODEL_PATH = "Assets/modelData/stage/skywall.tkm";
	const float SKY_Z = 3750.0f;            // 空を置く奥行き(敵のいる 1500 より奥)
	const float SKY_ROTATION_Y = 180.0f;    // モデルの正面が奥を向いているので、回してカメラ側に向ける

	// ---- 雲の床 ----
	// 絵4枚ぶんの長さの板を1枚だけ使う(板のつなぎ目をなくすため)
	const char* FLOOR_MODEL_PATH = "Assets/modelData/stage/cloudfloor_long.tkm";
	const float FLOOR_Y = -400.0f;          // 床の高さ(カメラの高さ 60 より下)
	const float FLOOR_TILE_LENGTH = 2400.0f;    // 絵1枚ぶんの奥行き(板の絵の繰り返しの長さ)
	const float FLOOR_CENTER_Z = 2400.0f;       // 板の中心の奥行き(画面の手前から空まで、床が途切れないように)
	const float SCROLL_SPEED = 500.0f;      // 1秒に手前へ流れる距離

	// ---- キャラクターを照らすライト(トーンマップを切ったぶん、明るさをここで決める) ----
	const Vector3 LIGHT_DIRECTION(0.3f, -0.5f, -1.0f);   // カメラ側を向いた面を照らす向き
	const Vector3 LIGHT_COLOR(2.5f, 2.5f, 2.5f);
	const Vector3 AMBIENT_COLOR(1.0f, 1.0f, 1.0f);
}

bool BackGround::Start()
{
	// 明るい背景が画面に入ると、トーンマップが「画面全体が明るい」と判断して、
	// 敵やプレイヤーまで暗くしてしまう。背景があるあいだは、トーンマップを切る
	g_renderingEngine->DisableTonemap();
	InitLight();

	InitSky();
	InitFloor();
	return true;
}

// ライトに照らされると暗く見えてしまうので、ライトを使わないシェーダーで読み込む
// 床は奥を透明にして、動かない空とのつなぎ目をぼかす
void BackGround::InitUnlit(ModelRender& render, const char* modelPath, const char* shaderPath, AlphaBlendMode blendMode)
{
	ModelInitData initData;
	initData.m_tkmFilePath = modelPath;
	initData.m_fxFilePath = shaderPath;
	initData.m_psEntryPointFunc = "PSMain";
	initData.m_alphaBlendMode = blendMode;

	render.InitForwardRendering(initData);
}

void BackGround::InitLight()
{
	Vector3 direction = LIGHT_DIRECTION;
	direction.Normalize();

	g_renderingEngine->SetDirectionLight(0, direction, LIGHT_COLOR);
	g_renderingEngine->SetAmbient(AMBIENT_COLOR);
}

void BackGround::InitSky()
{
	Quaternion rotation;
	rotation.SetRotationDegY(SKY_ROTATION_Y);

	InitUnlit(skyRender, SKY_MODEL_PATH, UNLIT_SHADER_PATH, AlphaBlendMode_None);
	skyRender.SetPosition(Vector3(0.0f, 0.0f, SKY_Z));
	skyRender.SetRotation(rotation);
	skyRender.Update();
}

void BackGround::InitFloor()
{
	InitUnlit(floorRender, FLOOR_MODEL_PATH, FADE_SHADER_PATH, AlphaBlendMode_Trans);
	UpdateFloor();
}

void BackGround::Update()
{
	scrollZ += SCROLL_SPEED * g_gameTime->GetFrameDeltaTime();

	// 絵1枚ぶん流れたら、最初に戻す(雲の絵がつながっているので、戻ったことは分からない)
	if (scrollZ >= FLOOR_TILE_LENGTH)
	{
		scrollZ -= FLOOR_TILE_LENGTH;
	}

	UpdateFloor();
}

// 床の板を、流れた量だけ手前(カメラ側)へずらす
void BackGround::UpdateFloor()
{
	floorRender.SetPosition(Vector3(0.0f, FLOOR_Y, FLOOR_CENTER_Z - scrollZ));
	floorRender.Update();
}

void BackGround::Render(RenderContext& rc)
{
	skyRender.Draw(rc);
	floorRender.Draw(rc);
}