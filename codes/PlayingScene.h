#pragma once
#include "Scene.h"
#include "common.h"
#include "UIButton.h"

//====================================================
// PlayingScene
//====================================================
// 【役割】
// 実際のゲームプレイを管理するシーン。
// プレイヤー操作、敵・オブジェクト管理、
// UI、タイマー、BGM制御、ゲーム終了判定を担当する。
//====================================================
class PlayingScene : public Scene
{
public:
	//-------------------------
	// Scene インターフェース
	//-------------------------
	void Init() override;	// シーン初期化
	void Update() override;	// ロジック更新
	void Draw() override;	// 描画処理

private:
	//====================================================
	// ゲーム進行状態
	//====================================================
	// READY : 開始待ち（Enter入力待ち）
	// MAIN  : ゲーム進行中
	static enum PlayState
	{
		READY,	// 開始待ち
		MAIN,	// ゲーム進行中
	};
	PlayState playState;	// 現在のゲーム進行状態

	void CheckGameEnd();	// ゲーム終了条件判定

	bool isGameStarted = false;	// 実際にゲームが開始されたか
	bool isFinished = false;	// 終了処理済みか
	bool started = false;	// 開始フラグ（二重開始防止）
	float gameTime = 0.0f;	// プレイ経過時間（秒）


	//====================================================
	// プレイヤー
	//====================================================
	// プレイヤー描画サイズ
	static constexpr int CHARACTER_WIDTH = 80;	// 竹かごの画像サイズに合わせる
	static constexpr int CHARACTER_HEIGHT = 80;	// 竹かごの画像サイズに合わせる

	// プレイヤー位置・移動速度
	float PlayerX;	// プレイヤーX座標
	float PlayerY;	// プレイヤーY座標
	float playerSpeed;	// プレイヤー移動速度

	// カーソル位置（コントローラー / マウス共用）
	float xPos;	// カーソルX座標
	float yPos;	// カーソルY座標

	// 入力方式管理
	bool useMouse = true;	// true: マウス / false: コントローラー
	int oldMouseX = -1;		// マウス前回X座標（入力方式切替判定用）
	int oldMouseY = -1;		// マウス前回Y座標（入力方式切替判定用）
	int cursorSpeed = 8;	// コントローラー操作速度


	//====================================================
	// 制限時間・タイマーUI
	//====================================================
	static constexpr int BOSS_TIME_BONUS = 5; // 秒（ボーナス加算用）

	unsigned int startTime;	// ゲーム開始時刻(ms)

	int timerX = 580;	// タイマーUI中心X座標
	int timerY = 50;	// タイマーUI中心Y座標
	int timerRadius = 40;	// タイマーUI半径

	double AnglePrecent;	// 円形タイマー用角度


	//====================================================
	// 背景
	//====================================================
	int bgX;	// スクロール用X座標


	//====================================================
	// 弾・リンゴ・エフェクト
	//====================================================
	Shot shot[SHOT_MAX];		// プレイヤーの弾
	Apple apple[APPLE_MAX];		// 落ちてくるリンゴ
	GoldenApple goldenApple;	// 金リンゴ
	Explosion explosion[EXPLOSION_MAX];	// 爆発エフェクト

	bool goldenAppleSpawned = false;	// 金リンゴ出現済みか

	int exPos1;	// 爆発エフェクト描画位置X
	int eyPos1;	// 爆発エフェクト描画位置Y

	float appleScale;	// リンゴ描画スケール
	float effectScale;	// 爆発エフェクト描画スケール

	// リンゴの拡大スケールを考慮した当たり判定サイズ
	int AppleW = int((APPLE_W * appleScale) + 20);
	int AppleH = int((APPLE_H * appleScale) + 20);


	//====================================================
	// スコア設定
	//====================================================
	int Apple_Score_0 = 150;		// ジグザグ
	int Apple_Score_1 = 300;		// サイン波
	int Apple_Score_2 = 100;		// 直進
	int Apple_Score_3 = 280;		// 円運動
	int Apple_Score_4 = 200;		// ジグザグ加速
	int Apple_Score_5 = -500;		// 毒リンゴ
	int Apple_Score_6 = 200;		// ミスリンゴ
	

	void CheckGoldenAppleSpawn();	// 金リンゴ出現条件
	void CheckGoldenAppleHit();		// 金リンゴ当たり判定
	void DrawGoldenApple();			// 金リンゴ描画


	//====================================================
	// ポップアップスコア演出
	//====================================================
	int displayScore = 0;	// 表示スコア
	int scoreAddValue = 0;	// 加算スコア（ポップアップ用）
	bool showScorePopup = false;	// ポップアップ表示中か

	int popupX = 0;	// ポップアップ表示位置X
	int popupY = 0;	// ポップアップ表示位置Y
	int popupTimer = 0;	// ポップアップ表示管理タイマー

	int  GetAppleScore(int type);				// リンゴ種別スコア取得
	void StartScorePopup(int addScore, int x, int y);	// ポップアップスコア演出開始
	void UpdateScorePopup();	// ポップアップスコア演出更新


	//====================================================
	// 危険エリア（デバフゾーン）
	//====================================================
	DangerZone dangerZone;	// 危険エリアの状態管理

	static constexpr float DANGER_INTERVAL = 600.0;	// 出現間隔（10秒）
	static constexpr float DANGER_DURATION = 300.0;	// 表示時間（5秒）

	int DANGER_SIZE_X = 200;	// 危険エリアサイズX
	int DANGER_SIZE_Y = 150;	// 危険エリアサイズY

	float dangerIntervalTimer = 0;	// 出現管理タイマー
	bool isPlayerInDangerZone = false;	// 侵入判定

	int warningBlinkTimer = 0;		// 警告UI点滅用


	//====================================================
	// UI
	//====================================================
	// UI背景座標
	static constexpr int Bg_x1 = 0;		// 画面左端
	static constexpr int Bg_y1 = 310;	// 画面下から上に向かって310pxの位置
	static constexpr int Bg_x2 = 160;	// 画面左端から160pxの位置
	static constexpr int Bg_y2 = 470;	// 画面下端

	//-------------------------
	// 入力状態
	//-------------------------
	bool InputEscape = false;		// エスケープキー入力
	bool InputSpace = false;		// スペースキー入力
	bool InputUpArrow = false;		// 上矢印キー入力
	bool InputDownArrow = false;	// 下矢印キー入力
	bool InputLeftArrow = false;	// 左矢印キー入力
	bool InputRightArrow = false;	// 右矢印キー入力

	// キー入力表示（暗転対応）
	void KeyInput(int x, int y, float scale, float angle, int imgHandle, bool dark);


	//====================================================
	// ポーズ
	//====================================================
	float pauseStartTime = 0;	// ポーズ開始時刻
	bool isPaused = false;	// ポーズ中か

	UIButton btnResume;		// ポーズ中の「RESUME」ボタン
	UIButton btnRestart;	// ポーズ中の「RESTART」ボタン
	UIButton btnTitle;		// ポーズ中の「TITLE」ボタン
	
	float cursorX = gWindow.width / 2;	// カーソル初期位置X（画面中央）
	float cursorY = gWindow.height / 2;	// カーソル初期位置Y（画面中央）


	//====================================================
	// UI配置定数
	//====================================================
	static constexpr int EscKey_Y = 345;		// EscキーイラストY座標
	static constexpr int EscKeyString_Y = 340;	// Escキー案内文字Y座標

	static constexpr int SpaceKey_Y = 390;		// スペースキーイラストY座標
	static constexpr int SpaceKeyString_Y = 380;// スペースキー案内文字Y座標

	static constexpr int UpArrow_Y = 425;		// 上矢印キーイラストY座標
	static constexpr int DownArrow_Y = 445;		// 下矢印キーイラストY座標
	static constexpr int LeftArrow_X = 20;		// 左矢印キーイラストX座標
	static constexpr int RightArrow_X = 60;		// 右矢印キーイラストX座標
	static constexpr int ArrowKeyString_Y = 425;// 矢印キー案内文字Y座標

	static constexpr int Graph_X = 40;	// キーイラストX座標
	static constexpr int String_X = 75;	// キー案内文字X座標


	//====================================================
	// 入力管理
	//====================================================
	int oldSpace = 0;	// スペースキー押下判定（連射防止）
	int oldEnter = 0;	// Enterキー押下判定（開始待ち用）
};
