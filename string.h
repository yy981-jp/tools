/**
 * @file string.h
 * @brief 文字列操作ユーティリティを提供
 * @details 置換、分割、型変換、検索など多様な文字列操作関数
 * @author yy981
 * @version 1.0
 */
#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#ifdef UNICODE
	#include <boost/locale.hpp>
#endif


/**
 * @brief 複数の値の中から指定値を検索
 * @tparam Args 可変長テンプレート引数型
 * @param value 検索値
 * @param args 比較対象の値群
 * @return value が args に含まれる場合true、そうでない場合false
 */
template <typename... Args> inline bool is_or(const std::string& value, Args... args) {
    std::vector<std::string> values = {args...};
    return std::find(values.begin(), values.end(), value) != values.end();
}

/**
 * @namespace st
 * @brief 文字列操作関数の名前空間
 */
namespace st {

#ifdef UNICODE
	/**
	 * @brief UTF-8文字列をワイド文字列に変換
	 * @param u8 UTF-8文字列
	 * @return ワイド文字列
	 * @note UNICODE定義時のみ有効
	 */
	inline std::wstring to_wstring(const std::string& u8) {
		return boost::locale::conv::to_utf<wchar_t>(u8, "UTF-8");
	}
#endif


/**
 * @brief 文字列内のすべての部分文字列を置換（参照版）
 * @param str 対象文字列（参照で修正される）
 * @param from 置換対象の文字列
 * @param to 置換後の文字列
 * @details 空文字列の from は処理されない（無限ループ防止）
 */
inline void replace_r(std::string& str, const std::string from, const std::string to) {
	if (from.empty()) return; // 空文字列を弾く
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.size(), to);
		start_pos += to.size();
	}
}

/**
 * @brief 文字列内のすべての部分文字列を置換（コピー版）
 * @param str 対象文字列（コピー）
 * @param from 置換対象の文字列
 * @param to 置換後の文字列
 * @return 置換後の新しい文字列
 */
inline std::string replace(std::string str, const std::string from, const std::string to) {
	replace_r(str,from,to);
	return str;
}

/**
 * @brief 文字列内から開始・終了パターンで囲まれた部分を抽出
 * @param input 対象文字列
 * @param start 開始パターン
 * @param end 終了パターン
 * @return マッチした部分文字列のベクトル
 * @details
 * startとendで囲まれた部分をすべてヒット。複数回のマッチをサポート
 */
inline std::vector<std::string> find(const std::string& input, std::string start, std::string end) {
	std::vector<std::string> output;
	int startpos  = 0;
	while(true) {
		// 検索
		size_t f = input.find(start,startpos);
		size_t b = input.find(end, f);
		if (f==std::string::npos || b==std::string::npos) break;
		// 部分文字列を抽出
		output.emplace_back(input.substr(f + start.size(), b - f - start.size()));
		startpos = b+1;
	}
	return output;
}

/**
 * @brief 文字列を整数に変換（数字のみ抽出）
 * @param str 対象文字列
 * @return 抽出された数字からなる整数、数字がない場合は0
 * @details 文字列中の数字のみを抽出して整数に変換
 */
inline int toi(const std::string& str) {
	std::string filtered;
	std::copy_if(str.begin(), str.end(), std::back_inserter(filtered),
		[](unsigned char c) { return std::isdigit(c); });

	// 変換後の数値を返す
	return filtered.empty() ? 0 : std::stoi(filtered);
}

/**
 * @brief 文字列ベクトルを整数ベクトルに変換
 * @param input 文字列ベクトル
 * @return 変換された整数ベクトル
 */
inline std::vector<int> toi(const std::vector<std::string>& input) {
	std::vector<int> result;
	for (std::string e: input) {
		result.emplace_back(st::toi(e));
	}
	return result;
}

/**
 * @brief 文字列をデリミタで分割
 * @param str 対象文字列
 * @param delimiter デリミタ
 * @return 分割された文字列ベクトル
 * @details 末尾にデリミタがない場合も最後の部分を含める
 */
// split to string
inline std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
	std::vector<std::string> tokens;
	size_t start = 0, end;

	while ((end = str.find(delimiter, start)) != std::string::npos) {
		tokens.emplace_back(str.substr(start, end - start));
		start = end + delimiter.size();
	}
	// 最後の部分を追加
	tokens.emplace_back(str.substr(start));
	return tokens;
}

/**
 * @brief 文字列をデリミタで分割して整数ベクトルに変換
 * @param str 対象文字列
 * @param delimiter デリミタ
 * @return 分割・変換された整数ベクトル
 */
// split to int
inline std::vector<int> spliti(const std::string& str, const std::string& delimiter) {
	std::vector<std::string> output = split(str,delimiter);
	std::vector<int> result;
	for (std::string e: output) {
		result.emplace_back(st::toi(e));
	}
	return result;
}


/**
 * @typedef splits
 * @brief デリミタをキーとして部分文字列ベクトルを格納するマップ型
 */
typedef std::unordered_map<std::string,std::vector<std::string>> splits;
/**
 * @typedef splitsi
 * @brief デリミタをキーとして整数ベクトルを格納するマップ型
 */
typedef std::unordered_map<std::string,std::vector<int>> splitsi;
	
/**
 * @brief 文字列を複数のデリミタで分割（マップ形式）
 * @param input 対象文字列
 * @param targets デリミタベクトル
 * @return { デリミタ -> [分割結果] } のマップ
 * @details
 * 複数のデリミタで同時に分割し、各部分をそれぞれのデリミタのキーに関連付け
 */
inline splits split(const std::string& input, const std::vector<std::string>& targets) {
	splits result;
	std::string current = input;
	std::string current_segment;
	std::string current_delimiter;

	// Helper function to find the first target in the string
	auto find_next_target = [&](const std::string& str, size_t start_pos) {
		size_t min_pos = std::string::npos;
		std::string found_target;
		for (const auto& target : targets) {
			size_t pos = str.find(target, start_pos);
			if (pos != std::string::npos && (min_pos == std::string::npos || pos < min_pos)) {
				min_pos = pos;
				found_target = target;
			}
		}
		return std::make_pair(min_pos, found_target);
	};

	size_t pos = 0;
	while (pos < current.size()) {
		auto [next_pos, delimiter] = find_next_target(current, pos);
		if (next_pos == std::string::npos) break;
			// Capture segment and corresponding delimiter
		current_segment = current.substr(pos, next_pos - pos + delimiter.size());
		current_delimiter = delimiter;

		// Store in result map
		result[current_delimiter].push_back(current_segment);

		// Move position past the current delimiter
		pos = next_pos + delimiter.size();
	}

	return result;
}

/**
 * @brief 文字列を複数のデリミタで分割して整数に変換（マップ形式）
 * @param input 対象文字列
 * @param targets デリミタベクトル
 * @return { デリミタ -> [整数配列] } のマップ
 */
inline splitsi spliti(const std::string& input, const std::vector<std::string>& targets) {
	splitsi result;
	splits output = split(input,targets);
	for (const auto& [key, segments] : output) {
		for (const auto& segment : segments) {
			result[key].push_back(st::toi(segment));
		}
	}
	return result;
}


/**
 * @brief char配列をstring配列に変換
 * @param i_argc 要素数
 * @param i_argv char配列ポインタ
 * @return 変換されたstring配列
 * @details コマンドライン引数(argc, argv)の変換に有用
 */
// char配列 to string配列
inline std::vector<std::string> charV(const int i_argc, const char* const i_argv[]) {
	std::vector<std::string> result;
	for (int i = 0; i < i_argc; ++i) {
		result.emplace_back(i_argv[i]);
	}
	return result;
}

/**
 * @brief char配列を整数配列に変換
 * @param i_argc 要素数
 * @param i_argv char配列ポインタ
 * @return 変換された整数配列
 */
// char配列 to int配列
inline std::vector<int> charVi(const int i_argc, const char* i_argv[]) {
	return st::toi(st::charV(i_argc,i_argv));
}

/**
 * @brief UTF-8文字列の文字数（バイト数ではなく）を取得
 * @param input 対象文字列（UTF-8）
 * @return 文字数（マルチバイト文字も1としてカウント）
 * @details
 * UTF-8エンコーディングを考慮して、実際の文字数を計算
 * 最初のバイト値から続くバイト数を判定
 */
inline size_t size(const std::string& input) {
	unsigned char lead;
	size_t char_size=0, input_size=0, pos;
	for (pos = 0; pos < input.size(); pos += char_size) {
		input_size++;
		lead = input[pos];
		if (lead < 0x80) char_size=1; else if (lead < 0xE0) char_size=2; else if (lead < 0xF0) char_size=3; else char_size=4;
	}
	return input_size;
}

}