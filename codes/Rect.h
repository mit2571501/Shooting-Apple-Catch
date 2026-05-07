#pragma once

// 矩形（Rectangle）を表す汎用クラス
// UI、当たり判定、クリック判定などに使用する
class Rect
{
public:
	// 矩形の左上座標 (x, y)
	int x, y;

	// 矩形の幅 (w) と 高さ (h)
	int w, h;

	// デフォルトコンストラクタ
	// すべて 0 で初期化
	Rect()
		: x(0), y(0), w(0), h(0)
	{
	}

	// 引数付きコンストラクタ
	// 指定した座標とサイズで矩形を生成
	Rect(int _x, int _y, int _w, int _h)
		: x(_x), y(_y), w(_w), h(_h)
	{
	}

	// 点 (px, py) がこの矩形の中に含まれているか判定する
	// 含まれていれば true、外なら false を返す
	bool Contains(int px, int py) const
	{
		return (
			px >= x && px <= x + w &&
			py >= y && py <= y + h
			);
	}
};
