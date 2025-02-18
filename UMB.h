// unordered_map binary
#pragma once
#include <fstream>
#include <unordered_map>
#include <string>
#include <cstring>


namespace UMB {

void save(const std::unordered_map<std::string,std::string>& umap, const std::string& filename) {
	std::ofstream ofs(filename, std::ios::binary);
	if (!ofs) {
		std::cerr << "ファイルを開けませんでした: " << filename << std::endl;
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

std::unordered_map<std::string,std::string> load(const std::string& filename) {
	std::unordered_map<std::string, std::string> umap;
	std::ifstream ifs(filename, std::ios::binary);
	if (!ifs) {
		std::cerr << "ファイルを開けませんでした: " << filename << std::endl;
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