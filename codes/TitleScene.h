#pragma once
#include "Scene.h"
#include "common.h"

class TitleScene : public Scene
{
public:
	void Init() override;	// シーン開始時の初期化
	void Update() override;	// 入力監視と選択状態更新
	void Draw() override;	// 背景とタイトル・メニューの描画

private:
	int selectIndex = 0;	// PLAY = 0 , RULE = 1 , EXIT = 2

	int oldUp = 0;		// 上
	int oldDown = 0;	// 下
	int oldEnter = 0;	// 決定

	int bgX = 0;    // 背景スクロール座標

	int ApplePosX = gWindow.width / 6;	// リンゴのX座標
	int ApplePosY = gWindow.height - gWindow.height / 4;	// リンゴのY座標

	float AppleMaxSize = 2.2;	// リンゴの最大サイズ
	float AppleMinSize = 1.0;	// リンゴの最小サイズ
	float AppleSize = AppleMinSize;	// リンゴの現在のサイズ
	float AppleSizeChangeSpeed = 0.05 / 1000;	// リンゴのサイズ変化速度
	int AppleSizeDirection = 1;	// リンゴのサイズ変化方向（1:拡大, -1:縮小）

	float AppleRotation = 0;	// リンゴの回転角度
	float AppleRotationSpeed = 0.35 / 1000;	// リンゴの回転速度
};
