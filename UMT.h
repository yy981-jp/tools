/**
 * @file UMT.h
 * @brief unordered_map をテキスト形式で永続化するクラスを提供
 * @details std::unordered_map<std::string, std::string>をテキストファイルに自動保存・読込するクラス
 *          キーと値は ": " (コロン+スペース)で区切られます
 * @author yy981
 * @version 1.0
 * 
 * @note テキスト形式: key: value (キーと値は ": " で区切る)
 *       O: key: value (正しい形式)
 *       X: key:value (コロン直後にスペースなし - NG)
 *       X: key : value (余分なスペース - NG)
 */
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
 * @class UMT
 * @brief キー-値ペアをテキストファイルに永続化するクラス
 * 
 * @details
 * - コンストラクタでファイルを読み込む、存在しない場合は作成
 * - operator[]でMapのようにキーにアクセス可能
 * - 変更は自動的にファイルに保存される
 * - キーの追加はoperator+=で実行可能
 * - キーの削除はoperator-=で実行可能
 */
class UMT {
public:
	/**
	 * @brief コンストラクタ - ファイルから読み込む
	 * @param filename 永続化するファイルパス
	 * @exception std::runtime_error ファイル操作に失敗した場合
	 * @details ファイルが存在しない場合は自動的に作成
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
	 * @brief キーに対応する値を取得(非const参照版)
	 * @param key キー
	 * @return キーに対応する値への参照
	 */
	std::string& operator[](const std::string& key) {
		return data[key];
	}
	
	/**
	 * @brief キーに対応する値を取得(const版)
	 * @param key キー
	 * @return キーに対応する値、存在しない場合は空文字列
	 */
	const std::string& operator[](const std::string& key) const {
		static const std::string empty;
		auto it = data.find(key);
		if (it != data.end()) return it->second;
		else return empty;
	}
	
	/**
	 * @brief キーを削除
	 * @param key 削除するキー
	 * @return このインスタンスへの参照（メソッドチェーン可能）
	 * @details 削除後は自動的にファイルに保存される
	 */
	UMT& operator-=(const std::string& key) {
		data.erase(key);
		save();
		return *this;
	}
	
	/**
	 * @brief キーと値を追加
	 * @param keyAndValue "key: value" 形式の文字列
	 * @return このインスタンスへの参照（メソッドチェーン可能）
	 * @exception std::runtime_error セパレータ(": ")が含まれない場合
	 * @details 追加後は自動的にファイルに保存される
	 */
	UMT& operator+=(const std::string& keyAndValue) {
		if (!keyAndValue.contains(separator)) throw std::runtime_error("UMT::operator+= noContains separator");
		separate(keyAndValue);
		save();
		return *this;
	}


	/**
	 * @brief キーと値を結合して"key: value"形式の文字列を生成
	 * @param key キー
	 * @param value 値
	 * @return "key: value" 形式の文字列
	 * @static
	 */
	static std::string set(const std::string& key, const std::string& value) {
		return key + separator + value;
	}
	
	std::unordered_map<std::string, std::string> data; ///< キー-値ペアを格納するマップ

	/**
	 * @brief 現在のデータをファイルに保存
	 * @exception std::runtime_error ファイル操作に失敗した場合
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
	std::string fname; ///< 永続化するファイルパス
	
	/**
	 * @brief "key: value"形式の文字列をパースしてmapに追加
	 * @param in パース対象の文字列
	 * @exception std::runtime_error セパレータ(": ")が含まれない場合
	 */
	void separate(const std::string& in) {
		if (!in.contains(separator)) throw std::runtime_error("UMT::separate() noContains separator");
		size_t separatorPos = in.find(separator);
		data.emplace(in.substr(0,separatorPos),in.substr(separatorPos+2));
	}
	
	static constexpr std::string separator = ": "; ///< キーと値のセパレータ
};