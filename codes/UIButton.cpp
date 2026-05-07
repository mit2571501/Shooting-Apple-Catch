#include "UIButton.h"
#include "Resource.h"
#include "common.h"

/**
 * @brief ボタンの初期化処理
 *
 * @param _x    ボタン左上X座標
 * @param _y    ボタン左上Y座標
 * @param _w    ボタンの横幅
 * @param _h    ボタンの縦幅
 * @param _text ボタンに表示する文字列
 */
void UIButton::Init(int x, int y, int w, int h, int imgHandle)
{
	rect = Rect(x, y, w, h);

	img = imgHandle;

	scale = 1.0;
}

/**
 * @brief ボタンの更新処理
 *
 * マウスカーソルがボタン上にあり、
 * かつクリックされた場合に true を返す
 *
 * @param cursorX マウスカーソルのX座標
 * @param cursorY マウスカーソルのY座標
 * @param click   クリック入力（1フレームの押下判定）
 *
 * @return true  ボタンがクリックされた
 * @return false それ以外
 */
bool UIButton::Update(float cursorX, float cursorY, bool click)
{
	// カーソルがボタン内にあるか
	isHover = rect.Contains((int)cursorX, (int)cursorY);

	double target = isHover ? 1.2 : 1.0;	// ホバー中は拡大、そうでないときは通常サイズ
	scale += (target - scale) * 0.2;	// 拡大率を徐々に変化させる（0.2は変化の速さ）

	// 「ホバー中 ＆ クリック」で決定
	return click && isHover;
}

/**
 * @brief ボタンの描画処理
 *
 * カーソル位置に応じて色を変え、
 * ボタンとカーソル画像を描画する
 *
 * @param cursorX マウスカーソルのX座標
 * @param cursorY マウスカーソルのY座標
 */
void UIButton::Draw(float cursorX, float cursorY) const
{
	int cx = rect.x + rect.w / 2;	// ボタンの中心X座標
	int cy = rect.y + rect.h / 2;	// ボタンの中心Y座標

	DrawRotaGraph(cx, cy, scale, 0.0, img, TRUE);

	// カーソル描画
	DrawRotaGraph(
		(int)cursorX,	// カーソルのX座標
		(int)cursorY,	// カーソルのY座標
		0.5,			// カーソル画像を小さく表示
		0.0,			// 回転なし
		imgCursor,		// カーソル画像
		TRUE
	);
}
