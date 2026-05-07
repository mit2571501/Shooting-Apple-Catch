#include "DxLib.h"
#include "PlayingScene.h"
#include "Resource.h"
#include "common.h"
#include "SceneManager.h"
#include "BgmManager.h"
#include "UIButton.h"
#include "RankingManager.h"

// 金リンゴ出現条件
void PlayingScene::CheckGoldenAppleSpawn()
{
	if (!goldenAppleSpawned &&						// 一度も金リンゴが出現していなく、
		((remainingTime <= 15 && score < 23000)		// 残り時間15秒の時スコアが23000未満または -
		|| (remainingTime <= 10 && score < 25000)	// 残り時間10秒の時スコアが25000未満または -
		|| (remainingTime <= 10 && score < 27000)))	// 残り時間10秒の時スコアが27000未満の場合
	{
		goldenApple.isAlive = true;	// 金リンゴを存在させる
		goldenAppleSpawned = true;	// 金リンゴを出現させる

		goldenApple.x = gWindow.width / 2.0f;
		goldenApple.y = 120.0f;		// 固定位置（上部）

		goldenApple.radius = 80;		// 金リンゴのサイズ
		goldenApple.hp = 20;			// 金リンゴの体力（20hp）
		goldenApple.scoreValue = 2000;	// 金リンゴから得られるスコア
	}
}

// 出現するリンゴの種類を「重み付きランダム」で決定する関数
int GetRandomAppleType()
{
	int totalWeight = 0;	// 全リンゴの重みの合計値

	// 各リンゴの重みを合計する
	for (int i = 0; i < APPLE_TABLE_SIZE; i++)	// 全てのリンゴ
	{
		totalWeight += appleTable[i].weight;	// 重みを加算
	}

	// 0 ～ (合計重み - 1) の範囲で乱数を生成
	int r = GetRand(totalWeight - 1);

	int sum = 0;

	// 重みを累積しながら、どのリンゴに当たるか判定
	for (int i = 0; i < APPLE_TABLE_SIZE; i++)
	{
		sum += appleTable[i].weight;	// 現在までの累積重み

		// 乱数がこの範囲に入っていれば、そのリンゴを選択
		if (r < sum)
		{
			return appleTable[i].type;
		}
	}

	// 通常はここには来ないが、万が一の保険
	return APPLE_NORMAL;
}

// 初期化
void PlayingScene::Init()
{
	SoundVolume();
	BgmManager::Instance().Play(sndBgm);	// ゲームBGM再生

	playState = READY;	// 初期状態を READY に設定
	
	remainingTime = GAME_TIME;	// 残り時間を設定
	isFinished = false;			// ゲーム状況初期化
	score = 0;					// スコア初期化

	//--------------------
	// 基本設計
	//--------------------
	playerSpeed = 5.0f;
	bgX = 0;

	appleScale = 0.5f;
	effectScale = 0.5f;

	//--------------------
	// 画面サイズ依存
	//--------------------
	xPos = gWindow.width - CHARACTER_WIDTH;
	yPos = gWindow.height - CHARACTER_HEIGHT;
	exPos1 = gWindow.width - APPLE_W;
	eyPos1 = gWindow.height - APPLE_H;

	//--------------------
	// プレイヤー初期位置
	//--------------------
	PlayerX = xPos / 2;
	PlayerY = int(yPos / 1.1f);

	//--------------------
	// 弾初期化
	//--------------------
	for (int i = 0; i < SHOT_MAX; i++)
		shot[i].isAlive = false;

	//--------------------
	// リンゴ初期化
	//--------------------
	started = false;
	if (!started) {
		startTime = GetNowCount();	// スタート時の現在時刻を取得
		started = true;	// ゲームスタート
		for (int i = 0; i < APPLE_MAX; i++)	// すべてのリンゴの位置をリセット（再ポップ）
		{
			apple[i].x = GetRand(gWindow.width - (APPLE_W + 20));	// ランダムX
			apple[i].y = -32;			// 画面外上に配置
			apple[i].alpha = 255;		// 不透明
			apple[i].isAlive = true;	// 生存フラグ
			apple[i].fading = false;	// フェードOFF
			apple[i].isCross = false;	// 重なりフラグ
			apple[i].type = GetRandomAppleType();	// 移動パターン
		}
	}
	for (int i = 0; i < APPLE_MAX; i++)	// すべてのリンゴの位置をリセット（再ポップ）
	{
		apple[i].x = GetRand(gWindow.width - (APPLE_W + 20));	// ランダムX
		apple[i].y = -50;						// 画面外上に配置
		apple[i].dx = 2;						// 左右移動速度
		apple[i].dy = 2;						// 下方向移動速度
		apple[i].isAlive = true;                // 生存フラグ
		apple[i].fading = false;				// フェードOFF
		apple[i].alpha = 255;					// 不透明
		apple[i].type = GetRandomAppleType();	// 移動パターン
		apple[i].centerX = apple[i].x;          // 円運動中心X
		apple[i].centerY = apple[i].y;          // 円運動中心Y
	}

	// --------------------
	// 爆発初期化
	// --------------------
	for (int i = 0; i < EXPLOSION_MAX; i++)	// すべての爆発エフェクトを非表示
		explosion[i].isAlive = false;		// 非表示

	// --------------------
	// 危険エリア（デバフゾーン）
	// --------------------
	dangerZone.isActive = false;	// 非表示
	dangerZone.timer = 0;			// 表示時間リセット
	dangerIntervalTimer = 0;		// 危険エリア出現間隔タイマー

	oldSpace = 0;	// スペースキーの前回入力状態

	// --------------------
	// ポーズ用ボタン
	// --------------------
	int centerX = gWindow.width / 2 - BTN_W / 2;
	int startY = 220;

	btnResume.Init(						// ゲーム再開
		centerX,
		startY,
		BTN_W,
		BTN_H,
		imgResumeButton
	);

	btnRestart.Init(					// リスタート
		centerX,
		startY + BTN_H + 20,
		BTN_W,
		BTN_H,
		imgRestartButton
	);

	btnTitle.Init(						// タイトルに戻る
		centerX,
		startY + (BTN_H + 20) * 2,
		BTN_W,
		BTN_H,
		imgTitleButton
	);
}

// ゲーム終了チェック
void PlayingScene::CheckGameEnd()
{
	if (isFinished) return;	// 二重遷移防止
	
	// -------------------
	// ゲームクリア判定
	// -------------------
	if (score >= SCORE_MAX)		// スコアが目標値達成
	{
		if (!isFinished)	// 二重遷移防止
		{
			isFinished = true;	// 終了処理済みフラグを立てる
			SceneManager::Instance().ChangeScene(GameState::GAMECLEAR);	// ゲームクリアシーンへ遷移
		}
	}

	// -------------------
	// ゲームオーバー判定
	// -------------------
	if(apple[6].y == gWindow.height + APPLE_H)	// ミスリンゴが地面に落ちたら
	{
		if (!isFinished)	// 二重遷移防止
		{
			isFinished = true;	// 終了処理済みフラグを立てる
			SceneManager::Instance().ChangeScene(GameState::GAMEOVER);	// ゲームオーバーシーンへ遷移
		}
	}
	if (remainingTime <= 0)		// 残り時間０秒
	{
		if (!isFinished)	// 二重遷移防止
		{
			isFinished = true;	// 終了処理済みフラグを立てる
			SceneManager::Instance().ChangeScene(GameState::GAMEOVER);	// ゲームオーバーシーンへ遷移
		}
	}
}

// 各リンゴごとのスコア
int PlayingScene::GetAppleScore(int type)	// type は Apple 構造体の type として参照される
{
	switch (type)	// リンゴの種類
	{
	case 0: return Apple_Score_0;	// ジグザグ
	case 1: return Apple_Score_1;	// サイン波
	case 2: return Apple_Score_2;	// 直進
	case 3: return Apple_Score_3;	// 円運動
	case 4: return Apple_Score_4;	// ジグザグ加速
	case 5: return Apple_Score_5;	// 直進（毒リンゴ）
	case 6: return Apple_Score_6;	// 直進（ミスリンゴ）
	default: return 0;
		break;
	}
}

// ポップアップスコア演出開始
void PlayingScene::StartScorePopup(int addScore, int x, int y)
{
	showScorePopup = true;		// ポップアップ開始
	scoreAddValue = addScore;	// 表示されるスコア

	popupX = x;		// 表示されるx座標位置
	popupY = y;		// 表示されるx座標位置
	popupTimer = 30;// 表示時間
}

// ポップアップスコア演出
void PlayingScene::UpdateScorePopup()
{
	if (!showScorePopup) return;	// ポップアップスコアが無い場合スキップ

	popupY -= 1;	// スコア移動
	popupTimer--;	// 表示時間減少

	if (popupTimer <= 0)	// 表示時間が0になったら -
	{
		showScorePopup = false;	// 消す
	}
}

// 金リンゴの当たり判定
void PlayingScene::CheckGoldenAppleHit()
{
	if (!goldenApple.isAlive) return;	// 金リンゴが出現していなかったらスキップ

	for (int i = 0; i < SHOT_MAX; i++)	// 全ての弾
	{
		if (!shot[i].isAlive) continue;	// 弾が無かったらスキップ

		float dx = shot[i].x - goldenApple.x;	// 弾と金リンゴのx座標の差
		float dy = shot[i].y - goldenApple.y;	// 弾と金リンゴのy座標の差
		
		float dist2 = dx * dx + dy * dy;	// それぞれの座標の２乗
		float r = goldenApple.radius;		// 金リンゴのサイズ

		if (dist2 <= r * r)	// それぞれの座標の２乗がサイズの２乗以下の場合
		{
			shot[i].isAlive = false;	// その弾を消す
			goldenApple.hp--;			// 金リンゴの体力を2減らす

			// 金リンゴ撃破判定
			if (goldenApple.hp <= 0)	// 金リンゴの体力が０なったら
			{
				goldenApple.isAlive = false;	// 存在させなくする
				goldenApple.isKill = true;		// 倒した判定
				score += goldenApple.scoreValue;// スコア加算（2,000点）
				startTime += BOSS_TIME_BONUS * 1000;	// 時間を5秒加算する
			}
		}
	}
}

// ゲーム更新
void PlayingScene::Update()
{
	int pad = GetJoypadInputState(DX_INPUT_PAD1);
	int nowEnter =
		CheckHitKey(KEY_INPUT_RETURN)        // キーボード Enter
		| (pad & PAD_INPUT_2);               // コントローラ ボタン2
	switch (playState)
	{
	case READY:	// 準備中

		if (nowEnter && !oldEnter)	// エンターキーが押されたら
		{
			startTime = GetNowCount();	// スタート時の現在時刻を取得
			playState = MAIN;	// ゲーム中へ
		}
		oldEnter = nowEnter;
		return;
	case MAIN:	// ゲーム中
	{
		//=============================================
		// プレイヤー入力処理（キーボード + パッド）
		//=============================================

		// 移動入力ベクトル
		float moveX = 0.0f;
		float moveY = 0.0f;

		//---------------------------------------------
		// キーボード入力（矢印キー）
		//---------------------------------------------
		if (CheckHitKey(KEY_INPUT_LEFT))  moveX -= 1.0f;	// 左
		if (CheckHitKey(KEY_INPUT_RIGHT)) moveX += 1.0f;	// 右
		if (CheckHitKey(KEY_INPUT_UP))    moveY -= 1.0f;	// 上
		if (CheckHitKey(KEY_INPUT_DOWN))  moveY += 1.0f;	// 下

		//---------------------------------------------
		// コントローラー入力（十字キー）
		//---------------------------------------------

		if (pad & PAD_INPUT_LEFT)  moveX -= 1.0f;
		if (pad & PAD_INPUT_RIGHT) moveX += 1.0f;
		if (pad & PAD_INPUT_UP)    moveY -= 1.0f;
		if (pad & PAD_INPUT_DOWN)  moveY += 1.0f;

		// ESCでポーズ切替
		static int oldEsc = 0;
		int esc = CheckHitKey(KEY_INPUT_ESCAPE) | pad & PAD_INPUT_4;

		if (esc && !oldEsc)	// ESCキーが押されたら
		{
			isPaused = !isPaused;	// ポーズ開始
			if (isPaused)	// ポーズ中
			{
				pauseStartTime = GetNowCount();	// ポーズ開始時刻を取得
			}
			else
			{
				float pauseDuration = GetNowCount() - pauseStartTime;	// ポーズしていた時間
				startTime += pauseDuration;		// 現在の時間に加算
				remainingTime = pauseDuration;	// 残り時間を調節
			}
		}
		oldEsc = esc;

		if (isPaused)	// ポーズ中
		{
			// ----- マウス更新 -----
			int mx, my;
			GetMousePoint(&mx, &my);

			if (mx != oldMouseX || my != oldMouseY)
			{
				useMouse = true;
				cursorX = (float)mx;
				cursorY = (float)my;
			}
			oldMouseX = mx;
			oldMouseY = my;

			bool click =
				(GetMouseInput() & MOUSE_INPUT_LEFT) |
				CheckHitKey(KEY_INPUT_RETURN) | pad & PAD_INPUT_2;

			if (pad)
			{
				useMouse = false;  // コントローラー操作中

				// 移動量初期化
				moveX = 0.0f;
				moveY = 0.0f;

				if (pad & PAD_INPUT_LEFT)  moveX = -1;
				if (pad & PAD_INPUT_RIGHT) moveX = 1;
				if (pad & PAD_INPUT_UP)    moveY = -1;
				if (pad & PAD_INPUT_DOWN)  moveY = 1;

				// カーソル座標更新（速度適用）
				cursorX += moveX * cursorSpeed;
				cursorY += moveY * cursorSpeed;
			}

			// -----------------------------
			// 画面外制限
			// -----------------------------
			int maxX = gWindow.width - 10;
			int maxY = gWindow.height - 10;
			if (cursorX < 10) cursorX = 10;
			if (cursorX > maxX) cursorX = maxX;
			if (cursorY < 10) cursorY = 10;
			if (cursorY > maxY) cursorY = maxY;

			// ----- ボタン処理 -----
			if (btnResume.Update(cursorX, cursorY, click))
			{
				isPaused = false;	// ポーズ終了（再開）
			}

			if (btnRestart.Update(cursorX, cursorY, click))
			{
				SceneManager::Instance().ChangeScene(GameState::PLAYING);	// リスタート
			}

			if (btnTitle.Update(cursorX, cursorY, click))
			{
				SceneManager::Instance().ChangeScene(GameState::TITLE);	// タイトル
			}

			return;
		}

		if (!isPaused) {
			float nowTime = GetNowCount();  // 現在時刻(ms)
			remainingTime = GAME_TIME - (nowTime - startTime) / 1000.0f; // 秒に変換
		}

		//---------------------------------------------
		// 移動適用（方向入力 × 移動速度）
		//---------------------------------------------
		PlayerX += moveX * playerSpeed;
		PlayerY += moveY * playerSpeed;

		//---------------------------------------------
		// 画面外へのはみ出し防止
		// xPos, yPos はプレイヤーの最大移動可能範囲
		//---------------------------------------------
		PlayerX = (PlayerX < 0) ? 0 : (PlayerX > xPos ? xPos : PlayerX);
		PlayerY = (PlayerY < 0) ? 0 : (PlayerY > yPos ? yPos : PlayerY);

		// デバッグ用：ゲームクリア
		if (CheckHitKey(KEY_INPUT_C) == 1)
		{
			score = SCORE_MAX;	// CキーでスコアをMAXに
			CheckGameEnd();
		}
		// デバッグ用：ゲームオーバー
		if (CheckHitKey(KEY_INPUT_V) == 1)
		{
			remainingTime = 0;	// Vキーでタイムを0に
			CheckGameEnd();
		}
		// デバッグ用：ボス出現
		if (CheckHitKey(KEY_INPUT_L) == 1)
		{
			remainingTime = 15;	// Lキーでタイムを一瞬15秒にし、強制的にボスを出現させる
		}

		// -----------------
		// キー押下状態の取得
		// -----------------
		// ↓各キーの押下状態取得
		InputEscape = CheckHitKey(KEY_INPUT_ESCAPE);	// Escキー
		InputSpace = CheckHitKey(KEY_INPUT_SPACE);		// スペースキー
		InputUpArrow = CheckHitKey(KEY_INPUT_UP);		// 上矢印キー
		InputDownArrow = CheckHitKey(KEY_INPUT_DOWN);	// 下矢印キー
		InputLeftArrow = CheckHitKey(KEY_INPUT_LEFT);	// 左矢印キー
		InputRightArrow = CheckHitKey(KEY_INPUT_RIGHT);	// 右印キー

		// ===== 弾発射　開始 =====
		int nowSpace = CheckHitKey(KEY_INPUT_SPACE) | pad & PAD_INPUT_2;

		// スペースを押した瞬間だけ弾を発射
		if (nowSpace && !oldSpace)
		{
			for (int i = 0; i < SHOT_MAX; i++)
			{
				if (!shot[i].isAlive)
				{
					shot[i].x = PlayerX + 25;	// 自機の中央あたり
					shot[i].y = PlayerY;		// 自機位置
					shot[i].isAlive = true;		// 発射中状態
					break;
				}
			}

			// 弾発射SE
			PlaySoundMem(sndShot, DX_PLAYTYPE_BACK);
		}
		oldSpace = nowSpace;	// スペース押下状態のリセット

		// === 弾発射　終了 ================================================================


		// ==========================================================================
		// 弾・リンゴの移動処理
		// ==========================================================================

		//---------------------------------------------
		// 弾の移動
		//---------------------------------------------
		for (int i = 0; i < SHOT_MAX; i++)
		{
			if (shot[i].isAlive)
			{
				shot[i].y -= 10;                    // 弾を上方向へ移動

				// 画面外に出たら削除する
				if (shot[i].y < -16)
					shot[i].isAlive = false;
			}
		}

		//---------------------------------------------
		// リンゴの移動 ＆ フェードアウト処理
		//---------------------------------------------
		for (int i = 0; i < APPLE_MAX; i++)
		{
			if (!apple[i].isAlive) continue;

			//=====================================================
			// 通常状態（フェードしていない）
			//=====================================================
			if (!apple[i].fading)
			{
				switch (apple[i].type)
				{
					//-------------------------------------------------
					// 0 : ジグザグ移動
					//-------------------------------------------------
				case 0:
					apple[i].x += apple[i].dx;	// 左右の移動
					apple[i].y += 2;			// 下方向への移動

					// 左右の壁で反転
					if (apple[i].x < 0 || apple[i].x > exPos1)	// 画面端で反転
						apple[i].dx *= -1;						// 進行方向反転
					break;

					//-------------------------------------------------
					// 1 : サイン波で左右に揺れながら落下
					//-------------------------------------------------
				case 1:
					apple[i].y += 2;	// 下方向への移動
					apple[i].x += int(50 * sin(GetNowCount() / 200.0 + i));	// サイン波で左右に揺れる
					break;

					//-------------------------------------------------
					// 2 : そのまま直進（下へ）
					//-------------------------------------------------
				case 2:
					apple[i].y += 3;	// 下方向への移動
					break;

					//-------------------------------------------------
					// 3 : 円運動しながら落下
					//-------------------------------------------------
				case 3:
				{
					apple[i].centerY += apple[i].dy;	// 円運動の中心も下に移動
					apple[i].x = int(apple[i].centerX + 50 * cos(GetNowCount() / 100.0 + i));	// 円運動のX
					apple[i].y = int(apple[i].centerY + 50 * sin(GetNowCount() / 100.0 + i));	// 円運動のY
					break;
				}

				//-------------------------------------------------
				// 4 : 加速気味のジグザグ落下
				//-------------------------------------------------
				case 4:
				{
					apple[i].x += apple[i].dx;	// 左右の移動
					apple[i].y += int(2 + sin(GetNowCount() / 50.0)); // 上下に揺れながら加速

					if (apple[i].x < 0 || apple[i].x > exPos1)	// 画面端で反転
						apple[i].dx *= -1;						// 進行方向反転
					break;
				}

				//-------------------------------------------------
				// 5 : 毒リンゴ　そのまま直進（下へ）少し早い
				//-------------------------------------------------
				case 5:
				{
					apple[i].y += 4;	// 下方向への移動
					break;
				}

				//-------------------------------------------------
				// 6 : ミスリンゴ　そのまま直進（下へ）少し遅い
				//-------------------------------------------------
				case 6:
				{
					apple[i].y += 1;	// 下方向への移動
					break;
				}
				}

				//-------------------------------------------------
				// 下に抜けたら上のランダム位置から再出現
				//-------------------------------------------------
				if (apple[i].y > gWindow.height)	// 画面下に出たら
				{
					if(apple[i].type == 6)	// ミスリンゴが落ちたらゲームオーバー
					{
						if (!isFinished)	// 二重遷移防止
						{
							isFinished = true;	// 終了処理済みフラグを立てる
							SceneManager::Instance().ChangeScene(GameState::GAMEOVER);	// ゲームオーバーシーンへ遷移
						}
					}
					else
					{
						apple[i].y = -32;	// 画面外上に配置
						apple[i].x = GetRand(gWindow.width) + 20;	// ランダムX
					}
				}
			}

			//=====================================================
			// フェードアウト（被弾中）
			//=====================================================
			else
			{
				apple[i].alpha -= 10;			// 徐々に透明にする

				// 完全に消えたらパラメーターをリセットして再出現
				if (apple[i].alpha <= 0)
				{
					apple[i].x = GetRand(gWindow.width - APPLE_W);	// ランダムX
					apple[i].y = -32;			// 画面外上に配置
					apple[i].alpha = 255;		// 不透明
					apple[i].fading = false;	// フェード終了
					apple[i].type = GetRandomAppleType();  // 新しい動きパターン
				}
			}
		}

		// ==========================================================================
		// 弾・リンゴの移動処理 終了
		// ==========================================================================

		//====================================================
		// 危険エリア（Danger Zone）出現・管理処理
		//====================================================
		//
		// 【概要】
		// 一定時間ごとに画面内のランダム位置へ危険エリアを生成し、
		// 表示時間が経過すると自動的に消滅させる。
		// また、プレイヤーが現在危険エリア内にいるかを判定する。
		//
		// 【管理データ】
		// - dangerIntervalTimer : 危険エリア生成までの経過フレーム
		// - DANGER_INTERVAL     : 次の危険エリア生成までの待機フレーム数
		// - DANGER_DURATION     : 危険エリアの表示フレーム数
		// - dangerZone          : 危険エリアの状態・座標・サイズ管理
		// - isPlayerInDangerZone: プレイヤー侵入判定フラグ
		//====================================================

		//------------------------------------
		// 危険エリア出現インターバル管理
		//------------------------------------
		dangerIntervalTimer++;

		// 危険エリアが非表示かつ、出現インターバルに到達した場合
		if (!dangerZone.isActive && dangerIntervalTimer >= DANGER_INTERVAL)
		{
			// 次回出現に備えてタイマーをリセット
			dangerIntervalTimer = 0;

			dangerZone.rect = Rect(
				GetRand(gWindow.width - DANGER_SIZE_X),	// ランダムX
				GetRand(gWindow.height - DANGER_SIZE_Y),// ランダムY
				DANGER_SIZE_X,							// 幅
				DANGER_SIZE_Y							// 高さ
			);

			// 表示継続時間を設定
			dangerZone.timer = DANGER_DURATION;

			// 危険エリアを有効化
			dangerZone.isActive = true;
		}

		//------------------------------------
		// 危険エリア表示中の寿命管理
		//------------------------------------
		if (dangerZone.isActive)
		{
			// 表示フレームを減算
			dangerZone.timer--;

			// 表示時間終了時に非表示化
			if (dangerZone.timer <= 0)
			{
				dangerZone.isActive = false;	// 危険エリアを非表示化
			}
		}

		//------------------------------------
		// プレイヤー侵入判定初期化
		//------------------------------------
		isPlayerInDangerZone = false;

		//------------------------------------
		// プレイヤーが危険エリア内にいるか判定
		//------------------------------------
		if (dangerZone.isActive)
		{
			// プレイヤー中心座標で当たり判定を行う
			int playerCenterX = (int)(PlayerX + CHARACTER_WIDTH / 2);
			int playerCenterY = (int)(PlayerY + CHARACTER_HEIGHT / 2);

			isPlayerInDangerZone =
				dangerZone.rect.Contains(playerCenterX, playerCenterY);	// プレイヤーが危険エリア内にいるか
		}

		// ==========================================================================
		// 当たり判定処理
		// ==========================================================================
		for (int i = 0; i < APPLE_MAX; i++)	// 全てのリンゴをループ
		{
			if (PlayerX < apple[i].x + AppleW &&
				PlayerX + CHARACTER_WIDTH > apple[i].x &&
				PlayerY < apple[i].y + AppleH &&
				PlayerY + CHARACTER_HEIGHT > apple[i].y)
			{
				// プレイヤーがリンゴと重なっている場合の処理
				apple[i].isCross = true;	// 重なりフラグを立てる
			}
			else
			{
				apple[i].isCross = false;	// 重なっていない場合はフラグを下ろす
			}
		}

		for (int i = 0; i < SHOT_MAX; i++)		// すべての弾をループ
		{
			if (!shot[i].isAlive) continue;          // 弾が存在しない場合スキップ

			CheckGoldenAppleHit();	// 金リンゴへの当たり判定

			for (int j = 0; j < APPLE_MAX; j++)	// 全てのリンゴをループ
			{
				if (!apple[j].isAlive) continue;     // リンゴが存在しない場合スキップ

				if (!apple[j].fading)		// フェードしていない時のみ
				{
					//---------------------------------------------------------
					// AABB（Axis Aligned Bounding Box: 軸平行矩形）衝突判定
					//---------------------------------------------------------
					if (shot[i].x < apple[j].x + AppleW &&
						shot[i].x + SHOT_W > apple[j].x &&
						shot[i].y < apple[j].y + AppleH &&
						shot[i].y + SHOT_H > apple[j].y)
					{
						// --- 衝突処理 ------------------------------------------------
						shot[i].isAlive = false;     // 弾を消す
						apple[j].fading = true;      // リンゴをフェードアウト状態へ

						//-----------------------------------------------------
						// 爆発エフェクト生成（空いている枠を探す）
						//-----------------------------------------------------
						for (int k = 0; k < EXPLOSION_MAX; k++)
						{
							if (!explosion[k].isAlive) {			// 空いている枠を見つけたら
								explosion[k].x = apple[j].x - 10;	// 爆発のX位置（リンゴの中心あたり）
								explosion[k].y = apple[j].y - 10;	// 爆発のY位置（リンゴの中心あたり）
								explosion[k].frame = 0;				// 爆発アニメの開始フレーム
								explosion[k].isAlive = true;		// 爆発を有効化

								break;
							}
						}

						//-----------------------------------------------------
						// スコア加算（タイプごとに価値を変化）
						//-----------------------------------------------------
						if (apple[j].isAlive)
						{
							int addScore = 0;
							switch (apple[j].type)	// リンゴの種類によってスコアを変える
							{
							case 0: addScore = Apple_Score_0;	break;	// ジグザグ
							case 1: addScore = Apple_Score_1;	break;	// サイン波
							case 2: addScore = Apple_Score_2;	break;	// 直進
							case 3: addScore = Apple_Score_3;	break;	// 円運動
							case 4: addScore = Apple_Score_4;	break;	// ジグザグ加速
							case 5: addScore = Apple_Score_5;	break;	// 毒リンゴ
							case 6: addScore = Apple_Score_6;	break;	// ミスリンゴ
							}

							if (isPlayerInDangerZone && addScore > 0)	// プレイヤーが危険エリア内で、
								// かつスコアが加算されるリンゴを撃ち落とした場合
							{
								addScore /= 2;   // 半減デバフ
							}

							score += addScore;	// スコア加算
							StartScorePopup(addScore, apple[j].x, apple[j].y);	// スコアポップアップ開始
							// 効果音再生
							if (apple[j].type <= 4)	// リンゴ
							{
								PlaySoundMem(sndCatch, DX_PLAYTYPE_BACK);	// リンゴ取得音
							}
							else                    // 毒リンゴ
							{
								PlaySoundMem(sndPoison, DX_PLAYTYPE_BACK);	// 毒リンゴ取得音
							}
						}
						UpdateScorePopup();	// スコアポップアップ更新

						// ----------------------------
						// 表示スコアを実スコアに追従させる
						// ----------------------------
						if (showScorePopup)
						{
							popupY -= 1;	// 上に浮かせる
							popupTimer--;	// ポップアップ表示時間を減らす

							if (popupTimer <= 0)	// 表示時間が終了したらポップアップを消す
							{
								showScorePopup = false;		// ポップアップ非表示
							}
						}

						break;  // この弾は他のリンゴと判定しない
					}
				}
			}
		}

		// ==========================================================================
		// 爆発アニメーション更新
		// ==========================================================================
		for (int i = 0; i < EXPLOSION_MAX; i++) {
			if (explosion[i].isAlive) {
				explosion[i].frame += 1;            // 次のフレームへ進める

				if (explosion[i].frame >= 16)      // 全フレーム再生したら終了
					explosion[i].isAlive = false;	// 爆発を消す
			}
		}

		// ==========================================================================
		// 当たり判定処理 終了
		// ==========================================================================

		// =================================
		// 残り時間によって効果音
		// =================================
		if (remainingTime == 20)	// 残り20秒で警告音
		{
			if (CheckSoundMem(sndTimer) == 0)	// 効果音が再生されていない場合のみ再生
			{
				PlaySoundMem(sndTimer, DX_PLAYTYPE_BACK);	// タイマー警告音
			}
		}
		if (remainingTime == 5)		// 残り5秒でさらに緊迫感のある警告音
		{
			if (CheckSoundMem(sndTimerFast) == 0)	// 効果音が再生されていない場合のみ再生
			{
				PlaySoundMem(sndTimerFast, DX_PLAYTYPE_BACK);	// タイマー高速警告音
			}
		}

		// ========================
		// ボス出現
		// ========================
		CheckGoldenAppleSpawn();	// 金リンゴの出現判定

		// ==========================================================================
		// 終了判定
		// ==========================================================================
		CheckGameEnd();	// ゲームクリア・ゲームオーバーの判定
		break;
	}
	break;
	}
}


// スコアバー描画関数
void DrawScoreBar(int x, int y, int score, int maxScore)	// 現在スコアと最大スコアを受け取って描画
{
	const int barWidth = 200; // バーの横幅（ピクセル単位）
	const int barHeight = 20;  // バーの縦幅

	// -----------------------------
	// スコアが最大値を超えた場合は補正
	// -----------------------------
	if (score >= maxScore) score = maxScore;

	// -----------------------------
	// 現在のスコアに応じたバーの塗りつぶし幅を計算
	// -----------------------------
	int fillWidth = (score * barWidth) / maxScore;

	// -----------------------------
	// 塗りつぶし部分（現在スコア）を描画
	// 色は緑で表示
	// -----------------------------
	DrawBox(x, y, x + fillWidth, y + barHeight, GetColor(50, 255, 50), TRUE);

	// -----------------------------
	// バーの枠を白で描画
	// -----------------------------
	DrawBox(x, y, x + barWidth, y + barHeight, GetColor(255, 255, 255), FALSE);

	// -----------------------------
	// バーの隣にスコア数値を表示
	// 例: 0 / 30000
	// 横位置はバーの右端 + 15ピクセル
	// -----------------------------
	DrawFormatString(x + barWidth + 15, y + 2, GetColor(50, 255, 50),
		"%d / %d", score, maxScore);
}

// 金リンゴ描画関数
void PlayingScene::DrawGoldenApple()
{
	if (!goldenApple.isAlive) return;	// 金リンゴが存在しない場合は描画しない

	// 金リンゴ（ボス）表示
	DrawRotaGraph(
		(int)goldenApple.x,	// 金リンゴのX座標
		(int)goldenApple.y,	// 金リンゴのY座標
		2.0,	// 大きく
		0.0,	// 回転なし
		imgGoldenApple,	// 金リンゴの画像
		TRUE
	);

	// 数値による体力の表示
	DrawFormatString(
		goldenApple.x,			// 金リンゴのX座標
		goldenApple.y + 20,		// 金リンゴの下に表示
		GetColor(0, 0, 0),		// 黒色で表示
		"%d", goldenApple.hp,	// 金リンゴの残りHP
		TRUE
	);
}

// キー押下状態に応じたキーイラスト描画関数
void PlayingScene::KeyInput(int x, int y, float scale, float angle, int imgHandle, bool dark)
{
	SetDrawBright(255, 255, 255);	// デフォルトは明るく描画

	if (dark)	// darkフラグが立っている場合は暗く描画
	{
		SetDrawBright(120, 120, 120);	// 暗く描画
	}
	
	// Escキー
	if (InputEscape)
	{
		DrawRotaGraph(Graph_X, EscKey_Y, 0.11f, 0.0f, imgEscKey, TRUE);
	}
	// スペースキー
	if (InputSpace)
	{
		DrawRotaGraph(Graph_X, SpaceKey_Y, 0.09f, 0.0f, imgSpaceKey, TRUE);
	}
	// 上矢印キー
	if (InputUpArrow)
	{
		DrawRotaGraph(Graph_X, UpArrow_Y, 0.2f, 0.0f, imgUpKey, TRUE);
	}
	// 下矢印キー
	if (InputDownArrow)
	{
		DrawRotaGraph(Graph_X, DownArrow_Y, 0.2f, 0.0f, imgDownKey, TRUE);
	}
	// 左矢印キー
	if (InputLeftArrow)
	{
		DrawRotaGraph(LeftArrow_X, DownArrow_Y, 0.2f, 0.0f, imgLeftKey, TRUE);
	}
	// 右矢印キー
	if (InputRightArrow)
	{
		DrawRotaGraph(RightArrow_X, DownArrow_Y, 0.2f, 0.0f, imgRightKey, TRUE);
	}

	SetDrawBright(255, 255, 255);	// 描画明るさをリセット
}

// ゲーム描画関数
void PlayingScene::Draw()
{
	// =======================
	// 背景
	// =======================
	DrawScrollingBackground(bgX, gWindow.width, gWindow.height, imgBackground1);

	// =======================
	// プレイヤー（竹かご）
	// =======================
	DrawGraph(PlayerX, PlayerY, imgPlayer, TRUE);

	if (playState == READY)	// ゲーム開始前の状態
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);		// 半透明モードに設定
		DrawBox(0, 180, gWindow.width, 235, GetColor(255, 255, 50), TRUE);	// 黄色の半透明背景
		DrawBox(0, 190, gWindow.width, 225, GetColor(0, 0, 0), TRUE);		// 黒の半透明背景
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		// ブレンドモードをリセット
		DrawFormatString(200, 200, GetColor(255, 255, 255), "PRESS ENTER KEY or A BUTTON");	// 開始の案内テキスト
	}

	if (playState == MAIN)	// ゲーム進行中の状態
	{
		// =======================
		// 弾（手）
		// =======================
		for (int i = 0; i < SHOT_MAX; i++)	// すべての弾をループ
		{
			if (!shot[i].isAlive)continue;	// 弾が存在しない場合スキップ
			DrawGraph(shot[i].x, shot[i].y, imgShot, TRUE);	// 弾の描画
		}

		// =======================
		// リンゴ
		// =======================
		for (int i = 0; i < APPLE_MAX; i++)	// すべてのリンゴをループ
		{
			if (!apple[i].isAlive) continue;	// リンゴが存在しない場合スキップ

			int cx = apple[i].x + APPLE_W / 2;	// リンゴの中心X座標
			int cy = apple[i].y + APPLE_H / 2;	// リンゴの中心Y座標
			
			if(!apple[i].fading && apple[i].isCross == true)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);	// プレイヤーと重なっているリンゴは常に不透明で描画
			}
			else
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, apple[i].alpha);	// フェードアウトのアルファ値を設定
			}
			if (apple[i].type >= 0 && apple[i].type <= 4)	// 毒リンゴ以外
			{
				DrawRotaGraph(cx, cy, appleScale, 0.0, imgApple, TRUE);	// リンゴの描画
			}
			else if (apple[i].type == 5)	// 毒リンゴ
			{
				DrawRotaGraph(cx, cy, appleScale, 0.0, imgPoisonApple, TRUE);	// 毒リンゴの描画
			}
			else if (apple[i].type == 6)	// ミスリンゴ
			{
				if (apple[i].y >= 300)	// ミスリンゴの描画(y座標300以上の時拡大して強調)
				{
					DrawRotaGraph(cx, cy, appleScale + 0.5f, 0.0, imgMissApple, TRUE);	// ミスリンゴの描画（拡大）
				}
				else
				{
					DrawRotaGraph(cx, cy, appleScale, 0.0, imgMissApple, TRUE);	// ミスリンゴの描画
				}
			}
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ブレンドモードをリセット
	}

	DrawGoldenApple();	// 金リンゴ描画

	// =======================
	// 爆発エフェクト
	// =======================
	for (int i = 0; i < EXPLOSION_MAX; i++)	// すべての爆発エフェクトをループ
	{
		if (!explosion[i].isAlive) continue;	// 爆発エフェクトが存在しない場合スキップ

		int cx = explosion[i].x + APPLE_W / 2;	// 爆発の中心X座標
		int cy = explosion[i].y + APPLE_H / 2;	// 爆発の中心Y座標

		DrawRotaGraph(	// 爆発エフェクトの描画
			cx,		// X座標
			cy,		// Y座標
			effectScale = 1.2f,	// 少し大きく
			0.0,				// 回転なし
			imgExplosion[explosion[i].frame],	// フレームに応じた爆発画像
			TRUE
		);
	}

	//======================
	// 危険エリア描画
	//======================
	if (dangerZone.isActive)	// 危険エリアが有効な場合のみ描画
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);	// 半透明モードに設定

		DrawBox(					// 危険エリアの描画
			dangerZone.rect.x,	// X座標
			dangerZone.rect.y,	// Y座標
			dangerZone.rect.x + dangerZone.rect.w,	// X座標 + 幅
			dangerZone.rect.y + dangerZone.rect.h,	// Y座標 + 高さ
			GetColor(255, 0, 0),	// 赤色
			TRUE
		);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);	// 警告テキストの背景を少し濃くするために再度半透明モードで描画
		DrawBox(					// 警告テキストの背景
			gWindow.width / 2 - 110,	// X座標（画面中央から少し左）
			gWindow.height / 2 - 45,	// Y座標（画面中央から少し上）
			gWindow.width / 2 + 80,		// X座標（画面中央から少し右）
			gWindow.height / 2 - 20,	// Y座標（画面中央から少し下）
			GetColor(50, 50, 50),		// 黒色（背景）
			TRUE
		);
		DrawBox(		// 警告テキストの枠
			gWindow.width / 2 - 110,	// X座標（画面中央から少し左）
			gWindow.height / 2 - 45,	// Y座標（画面中央から少し上）
			gWindow.width / 2 + 80,		// X座標（画面中央から少し右）
			gWindow.height / 2 - 20,	// Y座標（画面中央から少し下）
			GetColor(255, 255, 0),		// 黄色（枠）
			FALSE,
			2
		);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ブレンドモードをリセット

		// 警告テキストの描画（赤色で表示）
		DrawString(
			gWindow.width / 2 - 100,	// X座標（画面中央から少し左）
			gWindow.height / 2 - 40,	// Y座標（画面中央から少し上）
			"危険エリア出現中！！",		// テキスト内容
			GetColor(255, 0, 0),	// 赤色
			GetColor(255, 200, 200)	// 薄い赤色（縁取り）
		);
	}

	//======================
	// 危険エリア警告UI
	//======================
	if (isPlayerInDangerZone)	// プレイヤーが危険エリア内にいる場合のみ警告UIを表示
	{
		// 点滅（0.3秒周期）
		bool visible = (warningBlinkTimer / 15) % 2 == 0;

		if (visible)	// 点滅が表示されるタイミングの場合のみ警告UIを描画
		{
			const int boxW = 360;	// 警告ボックスの横幅
			const int boxH = 60;	// 警告ボックスの縦幅
			const int boxX = (gWindow.width - boxW) / 2;	// 画面中央に配置するためのX座標
			const int boxY = 20;	// 画面上部に配置するためのY座標

			// 警告ボックスの描画：背景（半透明 赤）
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);	// 半透明モードに設定
			DrawBox(	// 警告ボックスの描画
				boxX,	// X座標
				boxY,	// Y座標
				boxX + boxW,	// X座標 + 横幅
				boxY + boxH,	// Y座標 + 縦幅
				GetColor(180, 0, 0),	// 赤色
				TRUE
			);

			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ブレンドモードをリセット

			// 警告テキストの描画（白色で表示）
			DrawFormatString(
				boxX + 78,		// X座標（ボックス内の適切な位置）
				boxY + 15,		// Y座標（ボックス内の適切な位置）
				GetColor(255, 255, 255),	// 白色
				"  WARNING!!  SCORE DOWN\n   取得スコア　減少中！！"	// テキスト内容
			);
		}
	}

	// =======================
	// スコアバー
	// =======================
	DrawScoreBar(10, 10, score, SCORE_MAX);

	// ----------------------------
	// ポップアップスコア表示
	// ----------------------------
	int color = (scoreAddValue >= 0)	// スコア加算がプラスの場合は黄、マイナスの場合は赤で表示
		? GetColor(255, 255, 0)   // プラス：黄色
		: GetColor(255, 0, 0);    // マイナス：赤

	if (scoreAddValue != 0)	// スコア加算がある場合のみ表示
	{
		DrawFormatString(
			popupX,	// ポップアップのX座標
			popupY,	// ポップアップのY座標
			color,	// スコア加算がプラスの場合は黄、マイナスの場合は赤で表示
			"%+d",
			scoreAddValue	// スコア加算値を表示
		);
	}

	// 金リンゴが倒された時、タイマー下に表示
	if (goldenApple.isKill) {
		int timer;
		DrawFormatString(570, 80, GetColor(0, 255, 255), "+5 SEC");

		timer = remainingTime;

		if (remainingTime <= timer - 3)
		{
			goldenApple.isKill = false;	// タイマー加算表示が3秒経過したらフラグをリセットして表示を消す
		}
	}

	// =======================
	// タイマー
	// =======================
	//---------------------------------------------
	// タイマー描画（円形ゲージ）
	//---------------------------------------------
	DrawCircleAA(599, 39, 37, 250, GetColor(0, 0, 0), TRUE);      // 外枠（黒）
	DrawCircleAA(599, 39, 32, 250, GetColor(255, 255, 255), TRUE); // 内円（白）

	if (!isPaused)
	{
		float now = GetNowCount();
		double elapsedSec = (now - startTime) / 1000.0;			// 経過秒
		AnglePrecent = (elapsedSec / GAME_TIME) * 100.0;
	}

	if (AnglePrecent > 100.0) AnglePrecent = 100.0;			// 最大100%に制限
	if (playState == MAIN)
	{
		if (remainingTime <= 10)
		{
			DrawCircleGauge(600, 40, AnglePrecent, imgRedTimer);	// 残り時間10秒以下は赤いゲージ描画
		}
		else
		{
			DrawCircleGauge(600, 40, AnglePrecent, imgTimer);		// ゲージ描画
		}
	}
	
	//---------------------------------------------
	// 残り時間表示（色分け）
	//
	// 21秒以上: 黒
	// 10～20秒: 赤
	// 10秒未満: 赤で位置調整
	//---------------------------------------------
	if (remainingTime >= 10) {	// 残り時間が10秒以上の場合
		DrawFormatString(592, 34, GetColor(0, 0, 0), "%.0f", remainingTime);	// 黒で表示
	}
	else if (remainingTime < 10 && remainingTime > 0) {								// 残り時間が10～0秒の場合
		DrawFormatString(592, 34, GetColor(0, 0, 0), "%.1f", remainingTime);	// 黒で表示（位置を微調整）
	}
	else if(remainingTime <= 0) {	// 残り時間が0秒以下の場合は0と表示
		DrawString(598, 34, "0", GetColor(0, 0, 0));	// 黒で表示（位置を微調整）
	}

	// =======================
	// UI（画面左下）
	// 
	// ＠ESCキー　	：ポーズ
	// ＠SPACEキー	：発射
	// ＠矢印キー	：移動
	// =======================
	// -------------------------
	// キーイラスト描画
	// -------------------------
	{
		//--------------------------------
		// **キーイラストの暗転処理**
		// 
		// 通常時は明るく、
		// 押されているキーは暗く表示する
		//--------------------------------
		// Escキー
		if (!InputEscape)
		{
			DrawRotaGraph(Graph_X, EscKey_Y, 0.11f, 0.0f, imgEscKey, TRUE);	// 押されていない場合は明るく描画
		}
		else
		{
			KeyInput(Graph_X, EscKey_Y, 0.11f, 0.0f, InputEscape, InputEscape);	// 押されている場合は暗く描画
		}
		// ------------------------------------------------------------------------------
		// スペースキー
		if (!InputSpace)
		{
			DrawRotaGraph(Graph_X, SpaceKey_Y, 0.09f, 0.0f, imgSpaceKey, TRUE);
		}
		else
		{
			KeyInput(Graph_X, SpaceKey_Y, 0.09f, 0.0f, InputSpace, InputSpace);
		}
		// ------------------------------------------------------------------------------
		// 上矢印キー
		if (!InputUpArrow)
		{
			DrawRotaGraph(Graph_X, UpArrow_Y, 0.2f, 0.0f, imgUpKey, TRUE);
		}
		else
		{
			KeyInput(Graph_X, UpArrow_Y, 0.2f, 0.0f, InputUpArrow, InputUpArrow);
		}
		// ------------------------------------------------------------------------------
		// 下矢印キー
		if (!InputDownArrow)
		{
			DrawRotaGraph(Graph_X, DownArrow_Y, 0.2f, 0.0f, imgDownKey, TRUE);
		}
		else
		{
			KeyInput(Graph_X, DownArrow_Y, 0.2f, 0.0f, InputDownArrow, InputDownArrow);
		}
		// ------------------------------------------------------------------------------
		// 左矢印キー
		if (!InputLeftArrow)
		{
			DrawRotaGraph(LeftArrow_X, DownArrow_Y, 0.2f, 0.0f, imgLeftKey, TRUE);
		}
		else
		{
			KeyInput(LeftArrow_X, DownArrow_Y, 0.2f, 0.0f, InputLeftArrow, InputLeftArrow);
		}
		// ------------------------------------------------------------------------------
		// 右矢印キー
		if (!InputRightArrow)
		{
			DrawRotaGraph(RightArrow_X, DownArrow_Y, 0.2f, 0.0f, imgRightKey, TRUE);
		}
		else
		{
			KeyInput(RightArrow_X, DownArrow_Y, 0.2f, 0.0f, InputRightArrow, InputRightArrow);
		}
	}
	SetDrawBright(255, 255, 255);	// 保険

	// -------------------------
	// ポーズ案内
	// -------------------------
	DrawFormatString(String_X, EscKeyString_Y, GetColor(255, 255, 255), ":ポーズ");
	
	// -------------------------
	// 発射方法の案内
	// -------------------------
	DrawFormatString(String_X, SpaceKeyString_Y, GetColor(255, 255, 255), ":発射");

	// -------------------------
	// 移動方法の案内
	// -------------------------
	DrawFormatString(String_X, ArrowKeyString_Y, GetColor(255, 255, 255), ":移動");

	//--------------------------------------------------
	// ポーズ開始時のオーバーレイ描画
	// ポーズ中は画面全体を半透明の白で覆い、ポーズタイトルとボタンを表示する
	// ポーズ中以外はここまで描画して終了
	//--------------------------------------------------

	if (!isPaused) return;	// ポーズ中以外はここまで描画して終了

	// ===== ポーズオーバーレイ =====
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);	// 半透明モードに設定
	DrawBox(	// 画面全体を半透明の白で覆う
		0, 0,	// X座標（画面左端）
		gWindow.width,	// X座標（画面右端）
		gWindow.height,	// Y座標（画面下端）
		GetColor(255, 255, 255),	// 白色
		TRUE
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	// ブレンドモードをリセット

	DrawGraph(gWindow.width / 2 - 180, 40, imgPause, true);	// ポーズタイトル

	// ===== ボタン描画 =====
	btnResume.Draw(cursorX, cursorY);	// 再開
	btnRestart.Draw(cursorX, cursorY);	// リスタート
	btnTitle.Draw(cursorX, cursorY);	// タイトル
}
