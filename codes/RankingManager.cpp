#include "RankingManager.h"
#include <fstream>
#include <algorithm>

//--------------------------------------
// シングルトン
//--------------------------------------
RankingManager& RankingManager::Instance()
{
    static RankingManager instance;
    return instance;
}

//--------------------------------------
// CSV読み込み
//--------------------------------------
void RankingManager::Load()
{
	times.clear();          // 既存データクリア
	newRecordIndex = -1;    // 新記録インデックスリセット

	std::ifstream file(FILE_NAME);  // ファイルオープン
	if (!file.is_open())    // ファイルが存在しない場合は空のランキングとして扱う
        return;

	float time;   // クリアタイム読み込み
	while (file >> time)    // 読み込んだタイムをリストに追加
    {
		times.push_back(time);  // 読み込んだタイムをリストに追加
		if (file.peek() == ',') // カンマ区切りをスキップ
			file.ignore();      // カンマを読み飛ばす
    }

	SortAndTrim();  // 読み込んだデータを昇順にソートし、最大数制限を適用
}

//--------------------------------------
// CSV保存
//--------------------------------------
void RankingManager::Save() const
{
	std::ofstream file(FILE_NAME, std::ios::trunc); // ファイルオープン（上書きモード）
	if (!file.is_open())    // ファイルが開けない場合は保存できないので終了
        return;

	for (size_t i = 0; i < times.size(); ++i)   // タイムをカンマ区切りで保存
    {
		file << times[i];           // タイムを書き込む
		if (i + 1 < times.size())   // 最後のタイムでなければカンマを追加
			file << ",";            // カンマを追加
    }
}

//--------------------------------------
// 新しいクリアタイム追加
//--------------------------------------
void RankingManager::AddClearTime(float clearTime)
{
	times.push_back(clearTime); // 新しいタイムを追加
	SortAndTrim();  // タイムを昇順にソートし、最大数制限を適用

	newRecordIndex = -1;    // 新記録インデックス初期化
	for (size_t i = 0; i < times.size(); ++i)   // 新しいタイムの順位を検索
    {
		if (times[i] == clearTime)  // 新しいタイムが見つかった場合
        {
			newRecordIndex = static_cast<int>(i);   // 新記録の順位を保存
            break;
        }
    }

	Save();	// 変更をファイルに保存
}

//--------------------------------------
// 並び替え＋最大数制限
//--------------------------------------
void RankingManager::SortAndTrim()
{
    std::sort(times.begin(), times.end()); // 昇順

	if (times.size() > MAX_RANK)	// タイム数が最大数を超える場合
		times.resize(MAX_RANK);		// 上位10件のみ残す
}

//--------------------------------------
// ランキング取得
//--------------------------------------
const std::vector<float>& RankingManager::GetTimes() const
{
	return times;	// タイムのリストを返す
}

//--------------------------------------
// 新記録の順位取得
//--------------------------------------
int RankingManager::GetNewRecordIndex() const
{
	return newRecordIndex;	// 新記録の順位を返す（-1: 圏外）
}

//--------------------------------------
// リセット
//--------------------------------------
void RankingManager::Reset()
{
	times.clear();			// ランキングデータをクリア
	newRecordIndex = -1;	// 新記録インデックスリセット

	std::ofstream file(FILE_NAME, std::ios::trunc);	// ファイルオープン（上書きモード）して空にする
}