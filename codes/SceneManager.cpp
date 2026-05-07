#include "SceneManager.h"
#include "TitleScene.h"
#include "RuleScene.h"
#include "PlayingScene.h"
#include "ResultScene.h"
#include "Resource.h"
#include "DxLib.h"
#include "BgmManager.h"
#include "common.h"

// シングルトンインスタンス取得
SceneManager& SceneManager::Instance()
{
	static SceneManager instance;
	return instance;
}

// シーン遷移
void SceneManager::ChangeScene(GameState state)
{
	if (isFading) return;	// フェード中は遷移要求を無視

	nextState = state;	// 遷移先をセット
	isFading = true;	// フェード開始
	fadeOut = true;		// まずはフェードアウトから
	fadeAlpha = 0;		// フェード開始時は透明

	BgmManager::Instance().Stop();	// BGM停止（シーンごとに再生するため）
}

void SceneManager::Update()
{
	if (isFading)	// フェード処理
	{
		if (fadeOut)	// フェードアウト中
		{
			fadeAlpha += FADE_SPEED;	// 徐々に不透明に
			if (fadeAlpha >= 255)		// 完全に黒くなったら
			{
				fadeAlpha = 255;		// 上限固定

				// シーン切り替え
				if (currentScene)	// 現在のシーンがあれば削除
				{
					delete currentScene;	// メモリ解放
					currentScene = nullptr;	// ポインタを無効化
				}

				currentState = nextState;	// 状態更新
				CreateScene(currentState);	// 新シーン構築

				fadeOut = false; // フェードインへ
			}
		}
		else
		{
			fadeAlpha -= FADE_SPEED;	// 徐々に透明に
			if (fadeAlpha <= 0)			// 完全に透明になったら
			{
				fadeAlpha = 0;	// 下限固定
				isFading = false;	// フェード終了
			}
		}
	}

	if (currentScene)	// シーン更新
	{
		currentScene->Update();	// シーンのロジック更新
	}
}

void SceneManager::Draw()
{
	if (currentScene)	// シーン描画
	{
		currentScene->Draw();	// シーンの描画処理
	}

	// フェード描画（最前面）
	if (isFading)	// フェード中なら黒い半透明レイヤーを描画
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeAlpha);	// ブレンドモードを半透明に設定
		DrawBox(
			0, 0,
			gWindow.width,	//┬画面全体
			gWindow.height,	//┘
			GetColor(0, 0, 0),	// 黒色
			TRUE
		);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ブレンドモードを元に戻す
	}
}

// シーン構築
void SceneManager::CreateScene(GameState state)
{
	switch (state)	// シーン状態に応じたシーンを生成
	{
	case GameState::TITLE:	// タイトルシーン
		currentScene = new TitleScene();	// 動的に生成（ポインタで管理）
		break;

	case GameState::RULE:	// ルール説明シーン
		currentScene = new RuleScene();
		break;

	case GameState::PLAYING:	// ゲーム中シーン
		currentScene = new PlayingScene();
		break;
		
	case GameState::GAMECLEAR:	// ゲームクリアシーン
		currentScene = new ResultScene(true);	// クリアフラグを渡す
		break;

	case GameState::GAMEOVER:	// ゲームオーバーシーン
		currentScene = new ResultScene(false);	// クリアフラグを渡す
		break;
	}

	if (currentScene)	// シーンが生成されたら初期化
	{
		currentScene->Init();	// シーン初期化
	}
}

GameState SceneManager::GetCurrentState() const	// 現在のシーン状態取得
{
	return currentState;	// 現在のシーン状態を返す
}
