#pragma warning(disable:28251)
#include "DxLib.h"
#include "Resource.h"
#include "common.h"
#include "SceneManager.h"

// ======================================================
// メイン関数
// ======================================================
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ウィンドウタイトル
	SetMainWindowText("Apple Catch");

	// ウィンドウモードに変更
	ChangeWindowMode(TRUE);

	// DXライブラリ初期化
	if (DxLib_Init() == -1) return -1;

	// 裏画面に描画
	SetDrawScreen(DX_SCREEN_BACK);

	// マウスカーソル非表示
	SetMouseDispFlag(FALSE);

	LoadResources();	// リソース読み込み
	SoundVolume();		// 音量設定
	
	// 初めのシーンをタイトルに設定
	SceneManager::Instance().ChangeScene(GameState::TITLE);

	// -----------------------------
	// メインループ
	// -----------------------------
	while (ProcessMessage() == 0)
	{
		ClearDrawScreen();	// 画面クリア

		SceneManager::Instance().Update();	// シーン更新＋フェード制御
		SceneManager::Instance().Draw();	// シーン描画＋フェード描画

		ScreenFlip();	// 裏画面と表画面の内容を交換する
	}

	// リソース解放
	ReleaseResource();

	// DXライブラリ使用の後始末
	DxLib_End();
	return 0;
}
