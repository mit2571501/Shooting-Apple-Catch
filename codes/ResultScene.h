#pragma once
#include "Scene.h"
#include "common.h"
#include "UIButton.h"

class ResultScene : public Scene
{
public:
	ResultScene(bool clear);	// クリアかゲームオーバーかを引数で指定して生成

	void Init() override;	// シーン開始時の初期化
	void Update() override;	// 入力監視とUIボタンの更新
	void Draw() override;	// 背景と結果表示・UIの描画

private:
	// 状態
	bool isClear;	// クリアかゲームオーバーか

	float cursorX = gWindow.width / 2.0f;	// マウスカーソルの位置（初期値は画面中央）
	float cursorY = gWindow.height / 2.0f;	// マウスカーソルの移動量

	float moveX = 0.0f;	// マウスカーソルの移動量
	float moveY = 0.0f;	// マウスカーソルの移動量
	int cursorSpeed = 8;	// マウスカーソルの移動速度（ピクセル/フレーム）
	
	bool useMouse = true;	// マウス操作を使用するか（true: マウス / false: キーボード）
	int oldMouseX = -1;	// マウスの前フレームX座標（移動量計算用、初期値は-1で未取得状態を表す）
	int oldMouseY = -1;	// マウスの前フレームY座標（移動量計算用、初期値は-1で未取得状態を表す）

	//==============================
	// ランキング表示レイアウト定数
	//==============================

	// 全体枠
	static constexpr int RANK_X_LEFT = 450;		// ランキング表示の左端X座標
	static constexpr int RANK_X_MIDDLE = 520;	// ランキング表示の中央X座標（スコアと名前の境界）
	static constexpr int RANK_X_RIGHT = 620;	// ランキング表示の右端X座標

	static constexpr int RANK_Y_TOP = 120;		// ランキング表示の上端Y座標
	static constexpr int RANK_Y_BOTTOM = 450;	// ランキング表示の下端Y座標

	// 行レイアウト
	static constexpr int RANK_ROW_HEIGHT = 30;	// ランキング表示の行の高さ
	static constexpr int RANK_ROW_COUNT = 10;	// ランキング表示の行数

	// 枠線
	static constexpr int RANK_FRAME_THICKNESS = 6;	// ランキング表示の枠線の太さ

	// =====================
	// UIボタン
	// =====================
	UIButton btnRestart;		// リスタート
	UIButton btnTitle;			// タイトルに戻る
	UIButton btnRankingReset;	// ランキングリセット
};
