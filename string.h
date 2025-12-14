#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#ifdef UNICODE
	#include <boost/locale.hpp>
#endif


template <typename... Args> inline bool is_or(const std::string& value, Args... args) {
    std::vector<std::string> values = {args...};
    return std::find(values.begin(), values.end(), value) != values.end();
}

namespace st {

#ifdef UNICODE
	inline std::wstring to_wstring(const std::string& u8) {
		return boost::locale::conv::to_utf<wchar_t>(u8, "UTF-8");
	}
#endif


inline void replace_r(std::string& str, const std::string from, const std::string to) {
	if (from.empty()) return; // 空文字列を弾く
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.size(), to);
		start_pos += to.size();
	}
}

inline std::string replace(std::string str, const std::string from, const std::string to) {
	replace_r(str,from,to);
	return str;
}

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

inline int toi(const std::string& str) {
	std::string filtered;
	std::copy_if(str.begin(), str.end(), std::back_inserter(filtered),
		[](unsigned char c) { return std::isdigit(c); });

	// 変換後の数値を返す
	return filtered.empty() ? 0 : std::stoi(filtered);
}

inline std::vector<int> toi(const std::vector<std::string>& input) {
	std::vector<int> result;
	for (std::string e: input) {
		result.emplace_back(st::toi(e));
	}
	return result;
}

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

// split to int
inline std::vector<int> spliti(const std::string& str, const std::string& delimiter) {
	std::vector<std::string> output = split(str,delimiter);
	std::vector<int> result;
	for (std::string e: output) {
		result.emplace_back(st::toi(e));
	}
	return result;
}


typedef std::unordered_map<std::string,std::vector<std::string>> splits;
typedef std::unordered_map<std::string,std::vector<int>> splitsi;
	
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


// char配列 to string配列
inline std::vector<std::string> charV(const int i_argc, const char* const i_argv[]) {
	std::vector<std::string> result;
	for (int i = 0; i < i_argc; ++i) {
		result.emplace_back(i_argv[i]);
	}
	return result;
}

// char配列 to int配列
inline std::vector<int> charVi(const int i_argc, const char* i_argv[]) {
	return st::toi(st::charV(i_argc,i_argv));
}

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