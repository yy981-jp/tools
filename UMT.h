// unordered_map text
/*
	O:	key: value
	X:	key:value
	X:	key : value
*/
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

/**
 * @file UMT.h
 * @brief unordered_map のテキスト形式永続化クラス
 *
 * `std::unordered_map<std::string, std::string>` を `key: value` 形式の
 * テキストファイルとして保存・読み込みします。
 * ファイルが存在しない場合は自動的に作成されます。
 *
 * @note セパレータは `": "` （コロン+スペース）固定です。
 * @note `key:value`（スペースなし）や `key : value`（前後スペースあり）はサポートしていません。
 */

/**
 * @class UMT
 * @brief テキストファイルによる unordered_map 永続化クラス
 *
 * @code
 * UMT db("data.txt");
 * db += UMT::set("name", "Alice");  // ファイルに追記
 * std::string v = db["name"];       // "Alice"
 * db -= "name";                     // エントリ削除してファイル更新
 * @endcode
 */
class UMT {
public:
	/**
	 * @brief コンストラクタ。ファイルを開いて内容をメモリにロードする
	 * @param filename 使用するテキストファイルのパス。存在しない場合は新規作成する
	 * @throws std::runtime_error ファイルの作成・読み込みに失敗した場合
	 */
	UMT(const std::string& filename): fname(filename) {
		if (!fs::exists(fname)) {
			std::ofstream ofs(fname);
			if (!ofs) throw std::runtime_error("UMT::UMT() ofstream");
			return;
		}
		std::ifstream ifs(fname);
		if (!ifs) throw std::runtime_error("UMT::UMT() ifstream");
		std::string line;
		while (std::getline(ifs,line)) {
			if (line.empty()) continue;
			separate(line);
		}
	}
	
	/**
	 * @brief キーに対応する値を参照で取得する（書き込み可能）
	 * @param key 取得するキー
	 * @return 対応する値の参照（存在しない場合は空文字列が追加される）
	 */
	std::string& operator[](const std::string& key) {
		return data[key];
	}
	
	/**
	 * @brief キーに対応する値を参照で取得する（読み取り専用）
	 * @param key 取得するキー
	 * @return 対応する値の const 参照。存在しない場合は空文字列への参照
	 */
	const std::string& operator[](const std::string& key) const {
		static const std::string empty;
		auto it = data.find(key);
		if (it != data.end()) return it->second;
		else return empty;
	}
	
	/**
	 * @brief 指定したキーのエントリを削除してファイルを更新する
	 * @param key 削除するキー
	 * @return *this への参照
	 * @throws std::runtime_error ファイルへの書き込みに失敗した場合
	 */
	UMT& operator-=(const std::string& key) {
		data.erase(key);
		save();
		return *this;
	}
	
	/**
	 * @brief `"key: value"` 形式の文字列でエントリを追加しファイルを更新する
	 * @param keyAndValue `UMT::set(key, value)` で生成したセパレータ付き文字列
	 * @return *this への参照
	 * @throws std::runtime_error セパレータが含まれていない場合、またはファイル書き込み失敗時
	 */
	UMT& operator+=(const std::string& keyAndValue) {
		if (!keyAndValue.contains(separator)) throw std::runtime_error("UMT::operator+= noContains separator");
		separate(keyAndValue);
		save();
		return *this;
	}

	/**
	 * @brief キーと値を `"key: value"` 形式の文字列に変換する
	 * @param key   キー文字列
	 * @param value 値文字列
	 * @return `"key: value"` 形式の文字列
	 * @note operator+= に渡す文字列を生成するために使用します
	 */
	static std::string set(const std::string& key, const std::string& value) {
		return key + separator + value;
	}
	
	/// @brief 内部データストア
	std::unordered_map<std::string, std::string> data;

	/**
	 * @brief 内部データをファイルに書き出す
	 * @throws std::runtime_error ファイルへの書き込みに失敗した場合
	 */
	void save() {
		std::ofstream ofs(fname);
		if (!ofs) throw std::runtime_error("UMT::save() ofstream");
		std::string content;
		for (const auto& [key,value]: data) {
			content += UMT::set(key,value) + "\n";
		}
		ofs << content;
	}

private:
	/// @brief 対応するファイルパス
	std::string fname;
	
	/**
	 * @brief `"key: value"` 形式の文字列を分割して data に登録する
	 * @param in 分割するエントリ文字列
	 * @throws std::runtime_error セパレータが含まれていない場合
	 */
	void separate(const std::string& in) {
		if (!in.contains(separator)) throw std::runtime_error("UMT::separate() noContains separator");
		size_t separatorPos = in.find(separator);
		data.emplace(in.substr(0,separatorPos),in.substr(separatorPos+2));
	}
	
	/// @brief キーと値の区切り文字列（": "）
	static constexpr std::string separator = ": ";
};
