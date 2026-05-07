#pragma once
#include "DxLib.h"
#include "Rect.h"

/**
 * @class UIButton
 * @brief マウス操作用のシンプルなUIボタン
 *
 * 矩形領域に対してマウスカーソルの
 * ・ホバー判定
 * ・クリック判定
 * ・描画
 * を行う軽量UIコンポーネント。
 *
 * Scene に依存しないため、
 * Title / Pause / Result などで共通利用できる。
 */

class UIButton
{
public:
	/**
	 * @brief ボタンの初期化
	 *
	 * @param x     左上X座標
	 * @param y     左上Y座標
	 * @param w     横幅
	 * @param h     縦幅
	 * @param text  ボタンに表示する文字列
	 */
	void Init(int x, int y, int w, int h, int imgHandle);

	/**
	 * @brief ボタンの更新処理
	 *
	 * マウスカーソルがボタン内にあり、
	 * かつクリック入力があったフレームで true を返す。
	 *
	 * @param cursorX マウスカーソルX座標
	 * @param cursorY マウスカーソルY座標
	 * @param click   クリック入力（1フレーム押下）
	 *
	 * @return true  ボタンが押された
	 * @return false 押されていない
	 */
	bool Update(float cursorX, float cursorY, bool click);

	/**
	 * @brief ボタンの描画処理
	 *
	 * ホバー状態に応じて色を変えつつ、
	 * ボタンとカーソルを描画する。
	 *
	 * @param cursorX マウスカーソルX座標
	 * @param cursorY マウスカーソルY座標
	 */
	void Draw(float cursorX, float cursorY) const;

private:
	Rect rect;    // ボタンの矩形領域

	int img = -1;	// ボタンの画像（Resource.hで定義された画像IDを使用）
	double scale = 1.0;	// ボタンの拡大率（ホバー時に拡大するための変数）

	// 現在ホバー状態かどうか
	bool isHover = false;
};
