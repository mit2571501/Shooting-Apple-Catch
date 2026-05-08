#pragma once

// 画像
extern int imgTimer;		// タイマー
extern int imgRedTimer;		// タイマー（残り時間少ない）
extern int imgPlayer;		// プレイヤー
extern int imgShot;			// 弾
extern int imgApple;		// リンゴ
extern int imgPoisonApple;	// 毒リンゴ
extern int imgMissApple;	// ミスリンゴ
extern int imgGoldenApple;	// 金リンゴ（ボス）
extern int imgBackground1;  // メイン背景
extern int imgBackground2;  // ゲームクリア背景
extern int imgBackground3;  // ゲームオーバー背景
extern int imgPauseBackground;	// ポーズ背景
extern int imgClear;		//「CLEAR」ロゴ
extern int imgOver;			//「GAME OVER」ロゴ
extern int imgRule;			//「RULE」ロゴ
extern int imgPause;		//「Pause」ロゴ
extern int imgTitle;		//「Apple Catch」タイトルロゴ
extern int imgExplosion[16];// 爆発エフェクト[要素数]
extern int imgSpaceKey;		// スペースキー
extern int imgEscKey;		// Escキー
extern int imgCursor;		// カーソル
extern int imgUpKey;		// 上矢印キー
extern int imgDownKey;		// 下矢印キー
extern int imgLeftKey;		// 左矢印キー
extern int imgRightKey;		// 右矢印キー
extern int imgStartButton;	// ゲーム開始ボタン
extern int imgRuleButton;	// ルール説明ボタン
extern int imgExitButton;	// 終了ボタン
extern int imgRestartButton;// ゲーム再開ボタン
extern int imgTitleButton;	// タイトルボタン
extern int imgRankingResetButton;	// ランキングリセットボタン
extern int imgResumeButton;	// ゲーム再開ボタン
extern int imgTitleInfo;	// タイトル画面の説明テキスト
extern int imgTitleApple;	// タイトル画面のリンゴ
extern int imgTitleCursor;	// タイトル画面のカーソル

// 背景を動かす
void DrawScrollingBackground(int& bgX, int drawW, int drawH, int imgBg);
void DrawScrollingPauseBackground(int& bgX, int drawW, int drawH, int imgBg);

// 音声
extern int sndTitle;		// タイトルBGM
extern int sndBgm;			// ゲームBGM
extern int sndClear;		// ゲームクリアBGM
extern int sndOver;			// ゲームオーバーBGM
extern int sndSelect;		// 決定音
extern int sndShot;			// 弾発射音
extern int sndCatch;		// リンゴキャッチ音
extern int sndPoison;		// 毒リンゴキャッチ音
extern int sndTimer;		// タイマー
extern int sndTimerFast;	// タイマー（高速）

// 初期化・解放・音量設定
void LoadResources();
void SoundVolume();
void ReleaseResource();
