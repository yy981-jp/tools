#pragma once
#include <string>
#include <vector>


namespace st {

void replace(std::string& str, const std::string from, const std::string to) {
	if (from.empty()) return; // 空文字列を弾く
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // 重複置換を防ぐため位置を進める
	}
}

std::vector<std::string> find(const std::string& input, std::string start, std::string end) {
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

int toi(const std::string& str) {
	std::string filtered;
	std::copy_if(str.begin(), str.end(), std::back_inserter(filtered),
		[](unsigned char c) { return std::isdigit(c); });

	// 変換後の数値を返す
	return filtered.empty() ? 0 : std::stoi(filtered);
}

// split to string
std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
	std::vector<std::string> tokens;
	size_t start = 0, end;

	while ((end = str.find(delimiter, start)) != std::string::npos) {
		tokens.emplace_back(str.substr(start, end - start));
		start = end + delimiter.length();
	}
	// 最後の部分を追加
	tokens.emplace_back(str.substr(start));
	return tokens;
}

// split to int
std::vector<int> spliti(const std::string& str, const std::string& delimiter) {
	std::vector<int> tokens;
	size_t start = 0, end;

	while ((end = str.find(delimiter, start)) != std::string::npos) {
		tokens.emplace_back(st::toi(str.substr(start, end - start)));
		start = end + delimiter.length();
	}
	// 最後の部分を追加
	tokens.emplace_back(st::toi(str.substr(start)));
	return tokens;
}

bool chcmp(const char* cs1, const char* cs2) {
	return (std::string(cs1) == std::string(cs2));
}

}