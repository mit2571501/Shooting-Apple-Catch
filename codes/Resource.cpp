#include "Resource.h"
#include "DxLib.h"

// ===== 画像 =====
int imgTimer;			// タイマー
int imgRedTimer;		// タイマー（残り時間少ない）
int imgPlayer;			// プレイヤー
int imgShot;			// 弾
int imgApple;			// リンゴ
int imgPoisonApple;		// 毒リンゴ
int imgMissApple;		// ミスリンゴ
int imgGoldenApple;		// 金リンゴ（ボス）
int imgBackground1;		// メイン背景
int imgBackground2;		// ゲームクリア背景
int imgBackground3;		// ゲームオーバー背景
int imgClear;			//「CLEAR」ロゴ
int imgOver;			//「GAME OVER」ロゴ
int imgRule;			//「RULE」ロゴ
int imgPause;			//「Pause」ロゴ
int imgTitle;			//「Apple Catch」タイトルロゴ
int imgExplosion[16];	// 爆発
int imgSpaceKey;		// スペースキー
int imgEscKey;			// Escキー
int imgCursor;			// カーソル
int imgUpKey;			// 上矢印キー
int imgDownKey;			// 下矢印キー
int imgLeftKey;			// 左矢印キー
int imgRightKey;		// 右矢印キー
int imgStartButton;		// ゲーム開始ボタン
int imgRuleButton;		// ルール説明ボタン
int imgExitButton;		// 終了ボタン
int imgRestartButton;	// ゲーム再開ボタン
int imgTitleButton;		// タイトルボタン
int imgRankingResetButton;	// ランキングリセットボタン
int imgResumeButton;		// ゲーム再開ボタン
int imgTitleInfo;		// タイトル画面の説明テキスト
int imgTitleApple;		// タイトル画面のリンゴ
int imgTitleCursor;		// タイトル画面のカーソル

// ===== 音声 =====
int sndTitle;		// タイトルBGM
int sndBgm;			// ゲームBGM
int sndClear;		// ゲームクリアBGM
int sndOver;		// ゲームオーバーBGM
int sndSelect;		// 決定音
int sndShot;		// 弾発射音
int sndCatch;		// リンゴキャッチ音
int sndPoison;		// 毒リンゴキャッチ音
int sndTimer;		// タイマー
int sndTimerFast;	// タイマー（高速）

// 全リソース読み込み
void LoadResources()
{
	imgTimer			= LoadGraph("image/timerGraph.png"); // タイマー
	imgRedTimer			= LoadGraph("image/timerRedGraph.png"); // タイマー（残り時間少ない）
	imgPlayer			= LoadGraph("image/player.png");     // プレイヤー
	imgShot				= LoadGraph("image/shot.png");       // 弾
	imgApple			= LoadGraph("image/Apple.png");      // リンゴ
	imgPoisonApple		= LoadGraph("image/PoisonApple.png");// 毒リンゴ
	imgMissApple		= LoadGraph("image/MissApple.png");  // ミスリンゴ
	imgGoldenApple		= LoadGraph("image/GoldenApple.png");// 金リンゴ（ボス）
	imgBackground1		= LoadGraph("image/bg1.png");        // 背景１
	imgBackground2		= LoadGraph("image/bg2.png");		 // 背景２
	imgBackground3		= LoadGraph("image/bg3.jpg");		 // 背景３
	imgClear			= LoadGraph("image/clear.png");		 //「CLEAR」ロゴ
	imgOver				= LoadGraph("image/over.png");		 //「GAME OVER」ロゴ
	imgRule				= LoadGraph("image/rule.png");		 //「RULE」ロゴ
	imgPause			= LoadGraph("image/pause.png");		 //「Pause」ロゴ
	imgTitle			= LoadGraph("image/title.png");      // タイトル
	imgSpaceKey			= LoadGraph("image/SpaceKey.png");	 // スペースキー
	imgEscKey			= LoadGraph("image/EscKey.png");	 // Escキー
	imgCursor			= LoadGraph("image/MouseApple.png"); // カーソル
	imgUpKey			= LoadGraph("image/Up.png");		 // 上矢印キー
	imgDownKey			= LoadGraph("image/Down.png");		 // 下矢印キー
	imgLeftKey			= LoadGraph("image/Left.png");		 // 左矢印キー
	imgRightKey			= LoadGraph("image/Right.png");		 // 右矢印キー
	imgStartButton		= LoadGraph("image/StartButton.png");// ゲーム開始ボタン
	imgRuleButton		= LoadGraph("image/RuleButton.png"); // ルール説明ボタン
	imgExitButton		= LoadGraph("image/ExitButton.png"); // 終了ボタン
	imgRestartButton	= LoadGraph("image/RestartButton.png"); // ゲーム再開ボタン
	imgTitleButton		= LoadGraph("image/TitleButton.png"); // タイトルボタン
	imgRankingResetButton = LoadGraph("image/RankingResetButton.png"); // ランキングリセットボタン
	imgResumeButton		= LoadGraph("image/ResumeButton.png"); // ゲーム再開ボタン
	imgTitleInfo		= LoadGraph("image/TitleInfo.png");	 // タイトル画面の説明テキスト
	imgTitleApple		= LoadGraph("image/TitleApple.png"); // タイトル画面のリンゴ
	imgTitleCursor		= LoadGraph("image/TitleCursor.png");// タイトル画面のカーソル

	// 爆発エフェクト
	LoadDivGraph(
		"image/explosion.png",
		16,
		4, 4,
		64, 64,
		imgExplosion
	);

	sndTitle		= LoadSoundMem("sound/title.mp3");		// タイトルBGM
	sndBgm			= LoadSoundMem("sound/bgm.mp3");		// ゲームBGM
	sndClear		= LoadSoundMem("sound/clear.mp3");		// ゲームクリアBGM
	sndOver			= LoadSoundMem("sound/over.mp3");		// ゲームオーバーBGM
	sndSelect		= LoadSoundMem("sound/select.mp3");		// 決定音
	sndShot			= LoadSoundMem("sound/shot.mp3");		// 弾発射音
	sndCatch		= LoadSoundMem("sound/catch.mp3");		// キャッチ音
	sndPoison		= LoadSoundMem("sound/poison.mp3");		// 毒リンゴキャッチ音
	sndTimer		= LoadSoundMem("sound/timer.mp3");		// タイマー
	sndTimerFast	= LoadSoundMem("sound/timer_fast.mp3");	// タイマー（倍速）
}

void SoundVolume()
{
	// 音量設定（0～255）
	ChangeVolumeSoundMem(70, sndTitle);		// タイトルBGM
	ChangeVolumeSoundMem(70, sndBgm);		// ゲームBGM
	ChangeVolumeSoundMem(70, sndClear);		// ゲームクリアBGM
	ChangeVolumeSoundMem(70, sndOver);		// ゲームオーバーBGM
	ChangeVolumeSoundMem(100, sndSelect);	// 決定音
	ChangeVolumeSoundMem(60, sndShot);		// 弾発射音
	ChangeVolumeSoundMem(60, sndCatch);		// リンゴキャッチ音
	ChangeVolumeSoundMem(60, sndPoison);	// 毒リンゴキャッチ音
	ChangeVolumeSoundMem(100, sndTimer);	// タイマー
	ChangeVolumeSoundMem(100, sndTimerFast);// タイマー（高速）
}

// リソース解放
void ReleaseResource()
{
	// ===== 画像 =====
	DeleteGraph(imgTimer);			// タイマー
	DeleteGraph(imgRedTimer);		// タイマー（残り時間少ない）
	DeleteGraph(imgPlayer);			// プレイヤー
	DeleteGraph(imgShot);			// 弾
	DeleteGraph(imgApple);			// リンゴ
	DeleteGraph(imgPoisonApple);	// 毒リンゴ
	DeleteGraph(imgMissApple);		// ミスリンゴ
	DeleteGraph(imgGoldenApple);	// 金リンゴ（ボス）
	DeleteGraph(imgBackground1);	// メイン背景
	DeleteGraph(imgBackground2);	// ゲームクリア背景
	DeleteGraph(imgBackground3);	// ゲームオーバー背景
	DeleteGraph(imgClear);			//「GAME CLEAR」ロゴ
	DeleteGraph(imgOver);			//「GAME OVER」ロゴ
	DeleteGraph(imgPause);			//「Pause」ロゴ
	DeleteGraph(imgTitle);			//「Apple Catch」タイトルロゴ
	DeleteGraph(imgExplosion[0]);	// 爆発
	DeleteGraph(imgSpaceKey);		// スペースキー
	DeleteGraph(imgEscKey);			// Escキー
	DeleteGraph(imgCursor);			// マウスカーソル
	DeleteGraph(imgUpKey);			// 上矢印キー
	DeleteGraph(imgDownKey);		// 下矢印キー
	DeleteGraph(imgLeftKey);		// 左矢印キー
	DeleteGraph(imgRightKey);		// 右矢印キー
	DeleteGraph(imgStartButton);	// ゲーム開始ボタン
	DeleteGraph(imgRuleButton);		// ルール説明ボタン
	DeleteGraph(imgExitButton);		// 終了ボタン
	DeleteGraph(imgRestartButton);	// ゲーム再開ボタン
	DeleteGraph(imgTitleButton);	// タイトルボタン
	DeleteGraph(imgRankingResetButton);	// ランキングリセットボタン
	DeleteGraph(imgResumeButton);	// ゲーム再開ボタン
	DeleteGraph(imgTitleInfo);		// タイトル画面の説明テキスト
	DeleteGraph(imgTitleApple);		// タイトル画面のリンゴ
	DeleteGraph(imgTitleCursor);	// タイトル画面のカーソル

	// ===== 音声 =====
	DeleteSoundMem(sndTitle);		// タイトルBGM
	DeleteSoundMem(sndBgm);			// ゲームBGM
	DeleteSoundMem(sndClear);		// ゲームクリアBGM
	DeleteSoundMem(sndOver);		// ゲームオーバーBGM
	DeleteSoundMem(sndSelect);		// 決定音
	DeleteSoundMem(sndShot);		// 弾発射音
	DeleteSoundMem(sndCatch);		// リンゴキャッチ音
	DeleteSoundMem(sndPoison);		// 毒リンゴキャッチ音
	DeleteSoundMem(sndTimer);		// タイマー
	DeleteSoundMem(sndTimerFast);	// タイマー（高速）
}

// ======================================================
// @brief  横スクロールした背景を描画する関数
// ======================================================
void DrawScrollingBackground(int& bgX, int drawW, int drawH, int imgBg)
{
	bgX -= 2;
	if (bgX <= -drawW) bgX += drawW;
	DrawExtendGraph(bgX, 0, bgX + drawW, drawH, imgBg, TRUE);
	DrawExtendGraph(bgX + drawW, 0, bgX + drawW * 2, drawH, imgBg, TRUE);
}
