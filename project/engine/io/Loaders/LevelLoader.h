#pragma once
#include<string>
#include <vector>
#include <Vector3.h>
#include<json.hpp>

// 自キャラの生成データ
struct PlayerSpawnData {
	// 位置移動
	Vector3 translation;
	// 回転角
	Vector3 rotation;
};	


// レベルデータ
struct LevelData {
	// オブジェクト1個分のデータ
	struct ObjectData {
		// ファイル名
		std::string fileName;
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;
	};
	// オブジェクトのコンテナ
	std::vector<ObjectData> objects;
	// 自キャラ配列
	std::vector<PlayerSpawnData> players;
};

class LevelLoader
{
public:// メンバ関数			
    static const std::string kDefaultBaseDirectory;
    static const std::string kExtension;

	// ファイルの読み込み
	LevelData* LoadFile(const std::string& fileName);
	
	void LoadObjectsRecursive(const nlohmann::json& objectsJson, LevelData& levelData);

private: // メンバ変数
	LevelData* levelData = nullptr;
};