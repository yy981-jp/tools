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
 * @file string.h
 * @brief 文字列操作ユーティリティ
 *
 * 文字列の置換・検索・分割・変換など、よく使う文字列操作関数を
 * `st` 名前空間および汎用関数として提供します。
 * UNICODE ビルド時は Boost.Locale による UTF-8/wstring 変換も利用できます。
 */

/**
 * @brief 文字列が複数の候補値のいずれかと等しいか判定する
 * @tparam Args 可変テンプレート引数（std::string に変換可能な型）
 * @param value  判定する文字列
 * @param args   比較する候補値（可変長）
 * @return value が args のいずれかと等しい場合 true
 *
 * @code
 * if (is_or(cmd, "quit", "exit", "q")) return;
 * @endcode
 */
template <typename... Args> inline bool is_or(const std::string& value, Args... args) {
    std::vector<std::string> values = {args...};
    return std::find(values.begin(), values.end(), value) != values.end();
}

/**
 * @namespace st
 * @brief 文字列ユーティリティ用名前空間
 */
namespace st {

#ifdef UNICODE
	/**
	 * @brief UTF-8 文字列を wstring に変換する（UNICODE ビルド専用）
	 * @param u8 変換元の UTF-8 文字列
	 * @return 変換後の wstring
	 */
	inline std::wstring to_wstring(const std::string& u8) {
		return boost::locale::conv::to_utf<wchar_t>(u8, "UTF-8");
	}
#endif

/**
 * @brief 文字列中のすべての from を to に置換する（参照渡し、インプレース版）
 * @param str  置換対象の文字列（この文字列が変更される）
 * @param from 置換前の文字列
 * @param to   置換後の文字列
 * @note from が空文字列の場合は何もしません
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
 * @brief 文字列中のすべての from を to に置換した新しい文字列を返す（値返し版）
 * @param str  置換元の文字列（変更されない）
 * @param from 置換前の文字列
 * @param to   置換後の文字列
 * @return 置換後の文字列
 */
inline std::string replace(std::string str, const std::string from, const std::string to) {
	replace_r(str,from,to);
	return str;
}

/**
 * @brief 文字列中から start〜end に囲まれた部分文字列をすべて取り出す
 * @param input 検索対象の文字列
 * @param start 開始デリミタ
 * @param end   終了デリミタ
 * @return 開始デリミタと終了デリミタの間の部分文字列のベクタ
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
 * @brief 文字列中の数字のみを抽出して整数に変換する
 * @param str 変換元の文字列
 * @return 数字部分のみを連結して変換した整数値。数字がない場合は 0
 */
inline int toi(const std::string& str) {
	std::string filtered;
	std::copy_if(str.begin(), str.end(), std::back_inserter(filtered),
		[](unsigned char c) { return std::isdigit(c); });

	return filtered.empty() ? 0 : std::stoi(filtered);
}

/**
 * @brief 文字列ベクタの各要素を toi で整数に変換する
 * @param input 変換元の文字列ベクタ
 * @return 変換後の整数ベクタ
 */
inline std::vector<int> toi(const std::vector<std::string>& input) {
	std::vector<int> result;
	for (std::string e: input) {
		result.emplace_back(st::toi(e));
	}
	return result;
}

/**
 * @brief 文字列を単一のデリミタで分割する（文字列ベクタを返す）
 * @param str       分割する文字列
 * @param delimiter デリミタ文字列
 * @return 分割結果の文字列ベクタ
 */
inline std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
	std::vector<std::string> tokens;
	size_t start = 0, end;

	while ((end = str.find(delimiter, start)) != std::string::npos) {
		tokens.emplace_back(str.substr(start, end - start));
		start = end + delimiter.size();
	}
	tokens.emplace_back(str.substr(start));
	return tokens;
}

/**
 * @brief 文字列を単一のデリミタで分割し、各要素を整数に変換する
 * @param str       分割する文字列
 * @param delimiter デリミタ文字列
 * @return 分割・変換後の整数ベクタ
 */
inline std::vector<int> spliti(const std::string& str, const std::string& delimiter) {
	std::vector<std::string> output = split(str,delimiter);
	std::vector<int> result;
	for (std::string e: output) {
		result.emplace_back(st::toi(e));
	}
	return result;
}

/// @brief split() の複数デリミタ対応版の戻り値型: デリミタ → 部分文字列リスト
typedef std::unordered_map<std::string,std::vector<std::string>> splits;
/// @brief spliti() の複数デリミタ対応版の戻り値型: デリミタ → 整数リスト
typedef std::unordered_map<std::string,std::vector<int>> splitsi;

/**
 * @brief 文字列を複数のデリミタで分割し、どのデリミタで区切られたかを記録する
 * @param input   分割する文字列
 * @param targets 検索するデリミタ文字列のベクタ
 * @return デリミタをキー、そのデリミタで終わる部分文字列リストを値とするマップ
 */
inline splits split(const std::string& input, const std::vector<std::string>& targets) {
	splits result;
	std::string current = input;
	std::string current_segment;
	std::string current_delimiter;

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
			current_segment = current.substr(pos, next_pos - pos + delimiter.size());
			current_delimiter = delimiter;

		result[current_delimiter].push_back(current_segment);

		pos = next_pos + delimiter.size();
	}

	return result;
}

/**
 * @brief 複数デリミタ版 split の結果を整数に変換する
 * @param input   分割する文字列
 * @param targets 検索するデリミタ文字列のベクタ
 * @return デリミタをキー、整数リストを値とするマップ
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
 * @brief char 配列 (argc/argv 形式) を文字列ベクタに変換する
 * @param i_argc 引数の数
 * @param i_argv 引数文字列の配列
 * @return 文字列ベクタ
 */
inline std::vector<std::string> charV(const int i_argc, const char* const i_argv[]) {
	std::vector<std::string> result;
	for (int i = 0; i < i_argc; ++i) {
		result.emplace_back(i_argv[i]);
	}
	return result;
}

/**
 * @brief char 配列 (argc/argv 形式) を整数ベクタに変換する
 * @param i_argc 引数の数
 * @param i_argv 引数文字列の配列
 * @return 数字を抽出した整数ベクタ
 */
inline std::vector<int> charVi(const int i_argc, const char* i_argv[]) {
	return st::toi(st::charV(i_argc,i_argv));
}

/**
 * @brief UTF-8 文字列のマルチバイト文字数（文字単位の長さ）を返す
 * @param input UTF-8 エンコードされた文字列
 * @return 文字列の文字数（バイト数ではなく文字数）
 * @note ASCII (1バイト), 2バイト文字, 3バイト文字（日本語など）, 4バイト文字に対応
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

/**
 * @brief 行両端のスペースなどを取り除く
 * @param s 1行分の文字列
 * @return トリム後の文字列
 */
inline std::string_view trim(std::string_view s) {
	auto begin = s.find_first_not_of(" \t\n\r\f\v");
	auto end = s.find_last_not_of(" \t\n\r\f\v");

	if (begin == std::string_view::npos)
		return {};

	return s.substr(begin, end - begin + 1);
}

}
