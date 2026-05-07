#include "DxLib.h"
#include "BgmManager.h"

//====================================================
// BgmManager : シングルトン取得
//====================================================
// アプリケーション全体で唯一のBgmManagerインスタンスを返す。
// static ローカル変数によりスレッドセーフかつ遅延生成。
BgmManager& BgmManager::Instance()
{
	static BgmManager instance;
	return instance;
}

//====================================================
// BGM再生
//====================================================
// soundHandle : DxLibでロード済みのサウンドハンドル
//
// ・同じBGMがすでに再生中の場合は何もしない
// ・別のBGMが再生中の場合は必ず停止してから再生
// ・BGMは常にループ再生
void BgmManager::Play(int soundHandle)
{
	// 同一BGMがすでに再生中なら処理しない
	if (currentBgm == soundHandle && CheckSoundMem(currentBgm))
		return;

	// 別BGMが再生中なら停止
	if (currentBgm != -1 && currentBgm != soundHandle)
	{
		StopSoundMem(currentBgm);	// BGM停止
	}

	// 新しいBGMを再生
	currentBgm = soundHandle;
	PlaySoundMem(currentBgm, DX_PLAYTYPE_LOOP);
}

//====================================================
// BGM停止
//====================================================
// 再生中のBGMがあれば停止し、管理状態をリセットする
void BgmManager::Stop()
{
	if (currentBgm != -1)
	{
		StopSoundMem(currentBgm);
		currentBgm = -1;
	}
}

//====================================================
// 再生状態取得
//====================================================
// return : true  再生中
//          false 停止中
bool BgmManager::IsPlaying() const
{
	return currentBgm != -1 && CheckSoundMem(currentBgm);
}
