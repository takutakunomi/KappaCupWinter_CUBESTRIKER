#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

bool GameCamera::Start()
{
    // カメラの位置を設定する
    toCameraPos.Set(0.0f, 0.0f, -650.0f);

    // プレイヤーを取得する
    player = FindGO<Player>("player");

    // カメラの描画範囲を設定する
    g_camera3D->SetNear(1.0f);
    g_camera3D->SetFar(20000.0f);

    return true;
}

void GameCamera::Update()
{
    // 注視点を固定する
    Vector3 target;
    target.Set(0.0f, 60.0f, 0.0f);

    // カメラの位置を計算する
    Vector3 pos = target + toCameraPos;

    // 注視点と視点を設定する
    g_camera3D->SetTarget(target);
    g_camera3D->SetPosition(pos);

    // カメラを更新する
    g_camera3D->Update();
}

void GameCamera::Render(RenderContext& rc)
{

}