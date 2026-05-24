// unordered_map binary
#pragma once
#include <fstream>
#include <unordered_map>
#include <string>
#include <cstring>
#include <cstdint>

/**
 * @file UMB.h
 * @brief unordered_map のバイナリ形式永続化ユーティリティ
 *
 * `std::unordered_map<std::string, std::string>` をバイナリファイルに
 * 保存・読み込みする関数を提供します。キーは SHA-256 ハッシュ（64バイト固定長）、
 * 値は可変長データとして格納します。
 */

/**
 * @namespace UMB
 * @brief unordered_map バイナリ永続化用名前空間
 */
namespace UMB {

/**
 * @brief unordered_map をバイナリファイルに保存する
 *
 * 各エントリは以下の形式で書き込まれます：
 * - キー: 64 バイト固定長（SHA-256 ハッシュ文字列を想定）
 * - 値サイズ: 4 バイト (uint32_t)
 * - 値データ: 可変長
 *
 * @param umap     保存する unordered_map
 * @param filename 保存先ファイルパス
 * @throws std::runtime_error ファイルを開けない場合
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
 * @brief バイナリファイルから unordered_map を読み込む
 * @param filename 読み込むファイルパス
 * @return ファイルから復元した unordered_map
 * @throws std::runtime_error ファイルを開けない場合
 * @note UMB::save() で保存されたファイルを想定しています
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
