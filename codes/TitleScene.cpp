#include "TitleScene.h"
#include "DxLib.h"
#include "common.h"
#include "Resource.h"
#include "SceneManager.h"
#include "BgmManager.h"

void TitleScene::Init()
{
	SoundVolume();	// 音量設定
	BgmManager::Instance().Play(sndTitle);	// タイトルBGM再生

	selectIndex = 0;	// メニューの選択状態初期化

	oldUp = 0;		//┬入力状態初期化
	oldDown = 0;	//│
	oldEnter = 0;	//┘
}

void TitleScene::Update()
{
	//---------------------------------------------
	// 入力取得（キーボード + コントローラー）
	//---------------------------------------------
	int pad = GetJoypadInputState(DX_INPUT_PAD1);  // パッドの入力状態ビット取得
	// 上方向入力（ビット演算でキーボードとパッドを統合）
	int nowUp =
		CheckHitKey(KEY_INPUT_UP)            // キーボード ↑
		| (pad & PAD_INPUT_UP);              // コントローラー ↑

	// 下方向入力
	int nowDown =
		CheckHitKey(KEY_INPUT_DOWN)          // キーボード ↓
		| (pad & PAD_INPUT_DOWN);            // コントローラー ↓

	// 決定入力（Enter + コントローラのボタン2）
	int nowEnter =
		CheckHitKey(KEY_INPUT_RETURN)        // キーボード Enter
		| (pad & PAD_INPUT_2);               // コントローラ ボタン2

	// ===== 選択処理 =====

	//---------------------------------------------
	// 上キー：1段上へ（押し直しでのみ反応）
	//---------------------------------------------
	if (nowUp && !oldUp) {
		selectIndex--;                      // カーソルを上に移動
		if (selectIndex < 0) {
			selectIndex = TITLE_MAX - 1;    // 上端→最下段へループ
		}
	}

	//---------------------------------------------
	// 下キー：1段下へ（押し直しでのみ反応）
	//---------------------------------------------
	if (nowDown && !oldDown) {
		selectIndex++;                      // カーソルを下に移動
		if (selectIndex >= TITLE_MAX) {
			selectIndex = 0;                // 下端→先頭へループ
		}
	}

	//---------------------------------------------
	// 決定キー：選択項目に応じて状態遷移
	//---------------------------------------------
	if (nowEnter && !oldEnter) {
		PlaySoundMem(sndSelect, DX_PLAYTYPE_BACK);	// 決定音
		if (selectIndex == TITLE_START) {
			SceneManager::Instance().ChangeScene(GameState::PLAYING);	// ゲーム開始
		}
		else if (selectIndex == TITLE_RULE) {
			SceneManager::Instance().ChangeScene(GameState::RULE);		// ルール画面へ
		}
		else if (selectIndex == TITLE_EXIT) {
			BgmManager::Instance().Stop();	// BGM停止
			// リソース解放
			ReleaseResource();
			DxLib_End();                    // アプリ終了
			exit(0);
		}
	}

	// Escキーでアプリ終了
	if (CheckHitKey(KEY_INPUT_ESCAPE))
	{
		BgmManager::Instance().Stop();
		// リソース解放
		ReleaseResource();
		DxLib_End();
		exit(0);
	}

	// 入力状態を次フレームへ引き継ぎ
	oldUp = nowUp;
	oldDown = nowDown;
	oldEnter = nowEnter;
}

void TitleScene::Draw()
{
	// ===== 描画開始 =====
	// ===== 背景 =====
	DrawScrollingBackground(bgX, gWindow.width, gWindow.height, imgBackground1);		// スクロール背景

	// ---- 描画 ----
	//------------------------------
	// 
	// ＠背景
	// ＠タイトル
	// ＠次の動作を促す文
	// ＠タイトル画面のリンゴの回転とサイズ変化
	// ---------------
	// メニュー項目
	// ＠スタート
	// ＠ルール
	// ＠終了
	//------------------------------
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);	// 半透明モードに設定
	DrawBox(gWindow.width / 2 - 150, 140, gWindow.width / 2 + 150, 400, GetColor(200, 255, 200), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ブレンドモードをリセット

	// タイトル文字
	DrawGraph((gWindow.width / 2) - 250, 40, imgTitle, TRUE);

	// 次の動作を促す文
	DrawGraph(gWindow.width / 2 - 140, 145, imgTitleInfo, TRUE);

	// タイトル画面のリンゴの回転とサイズ変化
	// 回転
	for (int i = 0; i <= 360; i++)	// 毎フレーム回転
	{
		AppleRotation -= AppleRotationSpeed;
		if (AppleRotation >= 360)
		{
			AppleRotation == 0;	// 回転角度が360度を超えたら0に戻す
		}
	}
	// サイズ変化
	for (int i = 0; i <= 360; i++)	// 毎フレームサイズ変化
	{
		AppleSize += AppleSizeChangeSpeed * AppleSizeDirection;	// サイズを変化させる
		if (AppleSize >= AppleMaxSize || AppleSize <= AppleMinSize)
		{
			AppleSizeDirection *= -1;	// サイズが最大または最小に達したら変化方向を反転
		}
	}

	// タイトル画面のリンゴ
	// 左下と右下に同じリンゴを配置して回転とサイズ変化を適用
	// 左下のリンゴ
	DrawRotaGraph(
		ApplePosX, ApplePosY,	// X座標, Y座標
		AppleSize,				// 拡大率
		AppleRotation,			// 回転角度
		imgTitleApple,			// リンゴの画像（種類0）
		TRUE					// 半透明描画
	);
	// 右下のリンゴ
	DrawRotaGraph(
		gWindow.width - ApplePosX, ApplePosY,	// X座標, Y座標
		AppleSize,				// 拡大率
		AppleRotation,			// 回転角度
		imgTitleApple,			// リンゴの画像（種類0）
		TRUE					// 半透明描画
	);

	// メニュー項目
	// 選択中の項目は大きく、非選択は小さく表示
	// スタート
	if(selectIndex == TITLE_START)
	{
		DrawRotaGraph(gWindow.width / 2, 240, 1.2, 0.0, imgStartButton, TRUE);	// 選択中は大きい画像
		DrawGraph(gWindow.width / 2 - 105, 215, imgTitleCursor, TRUE);	// カーソルを表示
	}
	else
	{
		DrawRotaGraph(gWindow.width / 2, 240, 1.0, 0.0	, imgStartButton, TRUE);	// 非選択は小さい画像
	}

	// ルール
	if(selectIndex == TITLE_RULE)
	{
		DrawRotaGraph(gWindow.width / 2, 300, 1.2, 0.0, imgRuleButton, TRUE);	// 選択中は大きい画像
		DrawGraph(gWindow.width / 2 - 105, 275, imgTitleCursor, TRUE);	// カーソルを表示
	}
	else
	{
		DrawRotaGraph(gWindow.width / 2, 300, 1.0, 0.0, imgRuleButton, TRUE);	// 非選択は小さい画像
	}

	// 終了
	if(selectIndex == TITLE_EXIT)
	{
		DrawRotaGraph(gWindow.width / 2, 360, 1.2, 0.0, imgExitButton, TRUE);	// 選択中は大きい画像
		DrawGraph(gWindow.width / 2 - 105, 335, imgTitleCursor, TRUE);	// カーソルを表示
	}
	else
	{
		DrawRotaGraph(gWindow.width / 2, 360, 1.0, 0.0, imgExitButton, TRUE);	// 非選択は小さい画像
	}
}
