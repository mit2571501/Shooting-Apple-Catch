#include "ResultScene.h"
#include "Resource.h"
#include "DxLib.h"
#include "SceneManager.h"
#include "common.h"
#include "PlayingScene.h"
#include "BgmManager.h"
#include "UIButton.h"
#include "RankingManager.h"

// コンストラクタ
ResultScene::ResultScene(bool clear)	// clear : クリアかどうか
	:isClear(clear)	// メンバ初期化リスト
{
}

void ResultScene::Init()
{
	SoundVolume();	// 音量設定
	// クリアしていたら
	if (isClear)
	{
		float clearTime = GAME_TIME - remainingTime;			// クリアタイム計算
		RankingManager::Instance().Load();					// ランキングデータ取得
		RankingManager::Instance().AddClearTime(clearTime);	// 今回のクリアタイムをランキングに追加
		BgmManager::Instance().Play(sndClear);				// クリアBGM再生
	}
	else
	{
		BgmManager::Instance().Play(sndOver);	// オーバーBGM再生
	}

	// カーソル初期位置を画面中央に設定
	cursorX = gWindow.width / 2;
	cursorY = gWindow.height / 2;

	// ボタン初期化
	btnRestart.Init(BTN_RESTART_X, BTN_RESTART_Y, BTN_W, BTN_H, imgRestartButton);
	btnTitle.Init(BTN_TITLE_X, BTN_TITLE_Y, BTN_W, BTN_H, imgTitleButton);
	btnRankingReset.Init(BTN_RANKING_RESET_X, BTN_RANKING_RESET_Y, BTN_W, BTN_H, imgRankingResetButton);
}

void ResultScene::Update()
{
	// ----- マウス更新 -----
	int mx, my;
	GetMousePoint(&mx, &my);

	if (mx != oldMouseX || my != oldMouseY)
	{
		useMouse = true;
		cursorX = (float)mx;
		cursorY = (float)my;
	}
	oldMouseX = mx;
	oldMouseY = my;

	// ----- コントローラー更新 -----
	int pad = GetJoypadInputState(DX_INPUT_PAD1);	// ゲームパッド登録

	// クリック取得
	bool click =
		(GetMouseInput() & MOUSE_INPUT_LEFT) |
		CheckHitKey(KEY_INPUT_RETURN) | pad & PAD_INPUT_2;

	// パッド切り替え
	if (pad)
	{
		useMouse = false;  // コントローラー操作中

		// 移動量初期化
		moveX = 0.0f;
		moveY = 0.0f;

		if (pad & PAD_INPUT_LEFT)  moveX = -1;	// 左入力
		if (pad & PAD_INPUT_RIGHT) moveX = 1;	// 右入力
		if (pad & PAD_INPUT_UP)    moveY = -1;	// 上入力
		if (pad & PAD_INPUT_DOWN)  moveY = 1;	// 下入力

		// カーソル座標更新（速度適用）
		cursorX += moveX * cursorSpeed;
		cursorY += moveY * cursorSpeed;
	}

	// -----------------------------
	// 画面外制限
	// 
	// 全ての辺から10px
	// -----------------------------
	int maxX = gWindow.width - 10;		// 画面右端から10pxに設定
	int maxY = gWindow.height - 10;		// 画面下端から10pxに設定
	if (cursorX < 10) cursorX = 10;		// 画面左端から10px
	if (cursorX > maxX) cursorX = maxX;	// 画面右端から10px
	if (cursorY < 10) cursorY = 10;		// 画面上端から10px
	if (cursorY > maxY) cursorY = maxY;	// 画面下端から10px

	//--------------------------
	//---- リスタートボタン ----
	//--------------------------
	if (btnRestart.Update(cursorX, cursorY, click))	// リスタートボタンが押された
	{
		SceneManager::Instance().ChangeScene(GameState::PLAYING);	// PlayingSceneへ遷移
		return;
	}

	//------------------------
	//---- タイトルボタン ----
	//------------------------
	if (btnTitle.Update(cursorX, cursorY, click))	// タイトルボタンが押された
	{
		SceneManager::Instance().ChangeScene(GameState::TITLE);		// TitleSceneへ移動
		return;
	}

	//----------------------------------
	//---- ランキングリセットボタン ----
	//----------------------------------
	if (btnRankingReset.Update(cursorX, cursorY, click))	// ランキングリセットボタンが押された
	{
		RankingManager::Instance().Reset();	// ランキングリセット
		return;
	}

	// Escキーでゲーム終了
	if (CheckHitKey(KEY_INPUT_ESCAPE))
	{
		BgmManager::Instance().Stop();	// BGM停止
		// リソース解放
		ReleaseResource();
		DxLib_End();	// ゲーム終了
		exit(0);
	}
}
void ResultScene::Draw()
{
	//-----------------------------
	// 背景とロゴ描画
	//-----------------------------
	if (isClear)
	{
		// ===== GAME CLEAR =====
		
		// ゲームクリア背景描画
		DrawExtendGraph(0, 0, gWindow.width, gWindow.height, imgBackground2, TRUE);

		// 「GAME CLEAR」ロゴを表示
		DrawGraph(65, 25, imgClear, TRUE);
	}
	else
	{
		// ==== GAME OVER =====
		
		// ゲームオーバー背景描画
		DrawExtendGraph(0, 0, gWindow.width, gWindow.height, imgBackground3, TRUE);

		// 「GAME OVER」ロゴを表示
		DrawGraph(90, 25, imgOver, TRUE);
	}

	// -----------------------------
	// 白半透明のボックスを描画
	// -----------------------------
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 204);             // 透明度 204 (0-255)
	DrawBox(200, 130, 440, 380, GetColor(255, 255, 255), TRUE);	// 白半透明の背景
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);            // ブレンドモード解除

	if (isClear)	// クリアしていたら
	{
		// -----------------------------
		// クリアタイム表示
		// -----------------------------
		int clearTime = GAME_TIME - remainingTime;                // ここで clearTime を定義
		DrawFormatString(240, 140, GetColor(10, 10, 10),         // 黒文字で表示
			"クリアタイム: %d秒", clearTime);

		// -----------------------------
		// クリアタイムによる称号コメント表示
		// -----------------------------
		if (clearTime <= 40) {                                    // 40秒以下
			DrawString(210, 170, "青森県民レベルのリンゴ農家", GetColor(10, 10, 10));
		}
		else if (clearTime <= 50) {                               // 41～50秒
			DrawString(260, 170, "一般リンゴ農家", GetColor(10, 10, 10));
		}
		else {                                                    // 51秒以上
			DrawString(240, 170, "もっとリンゴを愛せ！", GetColor(10, 10, 10));
		}

		// ================================
		// ランキング表を作成
		// ================================
		//----------------------------
		// 背景描画
		//----------------------------
		DrawBox(
			RANK_X_LEFT,	//┬左上
			RANK_Y_TOP,		//┘
			RANK_X_RIGHT,					//┬右下
			RANK_Y_TOP + RANK_ROW_HEIGHT,	//┘
			GetColor(200, 200, 100),	// イエロー（少し明るめ）
			TRUE
		); // タイトル背景

		DrawBox(
			RANK_X_LEFT,					//┬左上
			RANK_Y_TOP + RANK_ROW_HEIGHT,	//┘
			RANK_X_MIDDLE,	//┬右下
			RANK_Y_BOTTOM,	//┘
			GetColor(200, 100, 200),	// マゼンタ（少し明るめ）
			TRUE
		); // 順位背景

		DrawBox(
			RANK_X_MIDDLE,					//┬左上
			RANK_Y_TOP + RANK_ROW_HEIGHT,	//┘
			RANK_X_RIGHT,	//┬右下
			RANK_Y_BOTTOM,	//┘
			GetColor(100, 200, 200),	// シアン（少し明るめ）
			TRUE
		); // タイム背景

		//----------------------------
		// 枠・罫線
		//----------------------------
		DrawBox(
			RANK_X_LEFT,	//┬左上
			RANK_Y_TOP,		//┘
			RANK_X_RIGHT,	//┬右下
			RANK_Y_BOTTOM,	//┘
			GetColor(0, 0, 0),	//黒色
			FALSE,	// 塗りつぶさない
			RANK_FRAME_THICKNESS	// 枠の太さ
		); // 外枠

		DrawLine(
			RANK_X_MIDDLE,					//┬上端
			RANK_Y_TOP + RANK_ROW_HEIGHT,	//┘
			RANK_X_MIDDLE,	//┬下端
			RANK_Y_BOTTOM,	//┘
			GetColor(0, 0, 0)	// 黒色
		); // 縦線

		// 横線（タイトル + 10行）
		for (int i = 0; i <= RANK_ROW_COUNT; i++)
		{
			int y = RANK_Y_TOP + (i * RANK_ROW_HEIGHT);
			DrawLine(
				RANK_X_LEFT,
				y,
				RANK_X_RIGHT,
				y,
				GetColor(0, 0, 0),
				2
			);
		}

		// -----------------------------
		// タイトルとデータを描画
		// -----------------------------
		DrawString(490, 130, "CLEAR TIME", GetColor(10, 10, 10)); // ランキングタイトル

		const auto& times = RankingManager::Instance().GetTimes();				// クリアタイムを取得
		int newRecordIndex = RankingManager::Instance().GetNewRecordIndex();	// 最新の記録を取得

		if (RankingManager::Instance().GetTimes().empty())	// ランキングデータがない場合
		{
			DrawString(470, 157, "--位   No Record", GetColor(10, 10, 10));	// 「No Record」表示
		}

		// ランキングデータを順位とともに表示	
		for (int i = 0; i < times.size(); i++)
		{
			bool isNew = (i == newRecordIndex);	// 最新の記録か

			int color = isNew			//┬色を設定
				? GetColor(255, 255, 0)	//┤最新の記録の場合イエロー
				: GetColor(0, 0, 0);	//┘そうでなければ黒色
			
			if (times[i] >= 10)
			{
				DrawFormatString(470, 157 + (i * 30), color, "%2d位  %.1f秒", i + 1, times[i]);	// 順位とタイムを表示
			}
			else
			{
				DrawFormatString(470, 157 + (i * 30), color, "%2d位   %.1f秒", i + 1, times[i]);	// 順位とタイムを表示
			}
			
			if (isNew)
			{
				DrawString(580, 157 + (i * 30), "NEW!", GetColor(255, 80, 80));	// 最新の記録の横に「NEW!」と薄い赤色で表示
			}
		}
	}
	// クリアしていなければ
	else
	{
		// -----------------------------
		// ゲームオーバー文言を表示
		// -----------------------------
		DrawString(240, 140, "クリアタイム: --秒", GetColor(10, 10, 10)); // タイム表示（ゲームオーバーなので仮表示）
		DrawString(215, 170, "もっとがんばりましょう...", GetColor(10, 10, 10)); // コメント
	}

	// -----------------------------
	// 次の操作を促す文字表示
	// -----------------------------
	DrawString(280, 200, "↓Click", GetColor(10, 10, 10));	// クリック促す文言

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(						// 背景四角
		465, gWindow.height - 25,
		gWindow.width, gWindow.height,
		GetColor(150, 150, 150), 
		TRUE
	);
	DrawBox(						// 枠
		465, gWindow.height - 25,
		gWindow.width, gWindow.height,
		GetColor(100, 100, 100),
		FALSE,
		5
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawString(470, gWindow.height - 20, "ESCキーでアプリ終了", GetColor(0, 0, 0));	// 終了促す文言

	// ボタン表示
	btnRestart.Draw(cursorX, cursorY);	// リスタートボタン
	btnTitle.Draw(cursorX, cursorY);	// タイトルボタン

	if (isClear)	// ランキングリセットボタンはクリア時のみ表示
	{
		btnRankingReset.Draw(cursorX, cursorY);	// ランキングリセットボタン
	}
}
