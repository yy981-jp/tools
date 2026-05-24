/**
 * @file UMB.h
 * @brief unordered_map をバイナリ形式で保存・読み込みする機能を提供
 * @details std::unordered_map<std::string, std::string>をバイナリファイルに直列化して
 *          保存・復元するユーティリティ関数を提供します
 * @author yy981
 * @version 1.0
 * 
 * @note バイナリ形式:
 *   - キー: 固定64バイト(SHA-256相当)
 *   - 値のサイズ: uint32_t(4バイト)
 *   - 値データ: 可変長
 */
// unordered_map binary
#pragma once
#include <fstream>
#include <unordered_map>
#include <string>
#include <cstring>
#include <cstdint>


namespace UMB {

/**
 * @brief unordered_mapをバイナリファイルに保存
 * @param umap 保存するunordered_map
 * @param filename 保存先ファイルパス
 * @exception std::runtime_error ファイルが開けない場合
 * 
 * @details
 * キーは64バイト固定サイズで出力され、値はサイズ情報付きで出力されます
 */
inline void save(const std::unordered_map<std::string,std::string>& umap, const std::string& filename) {
	std::ofstream ofs(filename, std::ios::binary);
	if (!ofs) {
		throw std::runtime_error("ファイルを開けませんでした: " + filename);
		return;
	}

	for (const auto& [key, value] : umap) {
		// キーを書き込む（SHA-256 64バイト）
		ofs.write(key.data(), 64);

		// 値のサイズを書き込む（4バイトのint）
		uint32_t value_size = value.size();
		ofs.write(reinterpret_cast<const char*>(&value_size), sizeof(value_size));

		// 値データを書き込む
		ofs.write(value.data(), value_size);
	}
}

/**
 * @brief バイナリファイルからunordered_mapを読み込む
 * @param filename 読み込むファイルパス
 * @return 復元されたunordered_map
 * @exception std::runtime_error ファイルが開けない場合
 * 
 * @details
 * ファイルの形式は save()関数が出力した形式と同じです
 */
inline std::unordered_map<std::string,std::string> load(const std::string& filename) {
	std::unordered_map<std::string, std::string> umap;
	std::ifstream ifs(filename, std::ios::binary);
	if (!ifs) {
		throw std::runtime_error("ファイルを開けませんでした: " + filename);
		return umap;
	}

	while (ifs.peek() != EOF) {
		// キーを読み込む
		char key[64] = {};
		ifs.read(key, 64);
		std::string key_str(key, 64);

		// 値のサイズを読み込む
		uint32_t value_size;
		ifs.read(reinterpret_cast<char*>(&value_size), sizeof(value_size));

		// 値データを読み込む
		std::string value(value_size, '\0');  // value_size分のメモリを確保
		ifs.read(&value[0], value_size);

		// mapに追加
		umap.emplace(std::move(key_str), std::move(value));
	}

	return umap;
}

}