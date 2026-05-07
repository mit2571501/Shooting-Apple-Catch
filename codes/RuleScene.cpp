#include "RuleScene.h"
#include "Resource.h"
#include "DxLib.h"
#include "SceneManager.h"
#include "common.h"

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void RuleScene::Init()
{
	oldEnter = 0;
}

//--------------------------------------------------
// 更新処理（Enter / Bボタンで戻る）
//--------------------------------------------------
void RuleScene::Update()
{
	int nowEnter =
		CheckHitKey(KEY_INPUT_RETURN) |
		(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_1);

	// 押した瞬間のみ反応
	if (nowEnter && !oldEnter)
	{
		SceneManager::Instance().ChangeScene(GameState::TITLE);	// タイトルに戻る
	}

	oldEnter = nowEnter;
}

//--------------------------------------------------
// 描画処理
//--------------------------------------------------
void RuleScene::Draw()
{
	//====================
	// 背景
	//====================
	DrawScrollingBackground(bgX, gWindow.width, gWindow.height, imgBackground1);

	//====================
	// 黒半透明パネル
	//====================
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 204);	// 半透明
	DrawBox(
		80, 80,			// 左上
		gWindow.width - 80,	//┬右下
		gWindow.height - 110,//┘
		GetColor(0, 0, 0),	// 黒色
		TRUE
	);
	DrawBox(
		80, gWindow.height - 110,	// 左上
		gWindow.width - 80,			//┬右下
		gWindow.height - 80,		//┘
		GetColor(100, 100, 100),	// 灰色
		TRUE
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ブレンドモードを元に戻す

	// 回転
	for (int i = 0; i <= 360; i++)	// 毎フレーム回転
	{
		AppleRotation -= AppleRotationSpeed;
		if (AppleRotation >= 360)
		{
			AppleRotation == 0;	// 回転角度が360度を超えたら0に戻す
		}
	}

	DrawRotaGraph(80, 80, 1.0, AppleRotation, imgTitleApple, TRUE);	// 回転リンゴ（左）
	DrawRotaGraph(gWindow.width - 80, 80, 1.0, AppleRotation, imgTitleApple, TRUE);	// 回転リンゴ（右）

	//====================
	// ルール説明
	//====================
	DrawRotaGraph(160, 110, 1.5, 0.0, imgRule, TRUE);	// 「RULE」ロゴ

	//--------------------
	// 操作・ルール説明
	//--------------------
	DrawString(String_X, String_Y, "・リンゴを採ると得点！　目標30,000点！", GetColor(100, 255, 100));						// １行目黄緑色
	DrawString(String_X, String_Y + String_I, "・採るとスコアが減る毒リンゴに注意！", GetColor(255, 255, 100));				// ２行目黄色
	DrawString(String_X, String_Y + String_I * 2, "・黒いリンゴが地面に着いたらゲームオーバー！", GetColor(255, 50, 50));	// ３行目赤色
	DrawString(String_X, String_Y + String_I * 3, "・キーボードとコントローラーで操作可能", GetColor(255, 255, 255));		// ４行目白色
	DrawString(String_X, String_Y + String_I * 4, "・矢印キー / スティック・十字ボタンで移動", GetColor(255, 255, 255));	// ５行目白色
	DrawString(String_X, String_Y + String_I * 5, "・SPACEキー / Aボタンで手を発射", GetColor(255, 255, 255));				// ６行目白色
	DrawString(String_X, String_Y + String_I * 6, "・ESCキー / Xボタンでポーズ", GetColor(255, 255, 255));					// ７行目白色

	//====================
	// 戻る案内
	//====================
	const char* msg = "> Press Enter or B button to return";	// 戻る案内

	int strLen = strlen(msg);	// 描画幅を取得
	int strWidth = GetDrawStringWidth(msg, strLen);	// 文字列の幅を取得

	int startX = gWindow.width / 2 - strWidth / 2;	// 画面中央から文字列の幅の半分を引いて、文字列が画面中央に来るようにX座標を計算
	int startY = gWindow.height - 105;		// 画面下から少し上にずらす

	DrawString(startX, startY, msg, GetColor(200, 200, 200));		// 文字列描画

	// 下線
	DrawLineAA(
		startX,				// 下線の開始X座標（文字列と同じ）
		startY + 20,		// 下線のY座標（文字列の下）
		startX + strWidth,	// 下線の終了X座標（文字列の幅分）
		startY + 20,		// 下線のY座標（文字列の下）
		GetColor(200, 200, 200)	// 下線の色（文字と同じ）
	);
}
