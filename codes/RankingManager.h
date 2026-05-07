#pragma once
#include <vector>
//#include <string>

class RankingManager
{
public:
    // シングルトン取得
    static RankingManager& Instance();

    // CSVから読み込み
    void Load();

    // CSVへ保存
    void Save() const;

    // 新しいクリアタイムを追加（秒）
    void AddClearTime(float clearTime);

    // ランキング取得（昇順）
    const std::vector<float>& GetTimes() const;

    // 今回追加されたタイムの順位（-1: 圏外）
    int GetNewRecordIndex() const;

    // ランキング全消去
    void Reset();

private:
	RankingManager() = default; // コンストラクタはprivate（シングルトン）

	static constexpr int MAX_RANK = 10; // ランキング最大数
	const char* FILE_NAME = "ranking_time.csv"; // ランキングデータのファイル名

	std::vector<float> times;
	int newRecordIndex = -1;    // 今回追加されたタイムの順位（-1: 圏外）

	void SortAndTrim(); // タイムを昇順にソートし、最大数制限を適用
};