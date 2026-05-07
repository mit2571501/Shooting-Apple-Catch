#pragma once
enum class GameState
{
	TITLE,      // タイトル画面
	RULE,       // ルール説明画面
	PLAYING,    // ゲーム中
	GAMECLEAR,  // ゲームクリア
	GAMEOVER    // ゲームオーバー
};

//====================================================
// Scene
// ゲームの各シーン（タイトル / ルール説明 / ゲーム中 / 結果）で共通のインターフェース
//====================================================
class Scene
{
public:
	virtual ~Scene() {}	// 仮想デストラクタ（派生クラスの正しい破棄のため）

	virtual void Init() = 0;	// シーン開始時
	virtual void Update() = 0;	// 更新
	virtual void Draw() = 0;	// 描画
};
