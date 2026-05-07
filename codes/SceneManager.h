#pragma once
#include "Scene.h"
#include <stack>
#include <vector>

//====================================================
// SceneManager
//====================================================
// 【役割】
// ゲーム全体のシーン遷移を一元管理する。
// ・シーンの生成／破棄
// ・フェード演出付きの遷移制御
// ・Update / Draw の振り分け
//
// 各Sceneは直接他Sceneを操作せず、
// 必ず SceneManager 経由で遷移要求を行う。
//====================================================
class SceneManager
{
public:
	//-------------------------
	// シングルトン取得
	//-------------------------
	static SceneManager& Instance();

	//-------------------------
	// シーン切り替え要求
	//-------------------------
	// Scene から呼び出される遷移リクエスト
	// 即時切り替えではなく、フェード処理を挟む
	void ChangeScene(GameState state);

	//-------------------------
	// メインループ呼び出し
	//-------------------------
	void Update();	// シーン更新＋フェード制御
	void Draw();	// シーン描画＋フェード描画

	//-------------------------
	// 現在のシーン状態取得
	//-------------------------
	GameState GetCurrentState() const;

private:
	SceneManager() = default;

	//====================================================
	// シーン管理
	//====================================================
	GameState currentState = GameState::TITLE;	// 現在のシーン
	GameState nextState = GameState::TITLE;	// 遷移先シーン

	Scene* currentScene = nullptr;				// 実行中のシーン

	// 指定された GameState に対応する Scene を生成
	void CreateScene(GameState state);


	//====================================================
	// フェード管理
	//====================================================
	bool isFading = false;	// フェード中か
	bool fadeOut = true;	// true: フェードアウト / false: フェードイン
	int  fadeAlpha = 0;		// フェード不透明度（0～255）

	static constexpr int FADE_SPEED = 12;	// フェード速度
};
