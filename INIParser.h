#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

class INIParser {
public:
    // コンストラクタでINIファイルをロード
    INIParser(const std::string& filename) {
        load(filename);
    }

    // 値を取得（セクションとキーを指定）
    std::string get(const std::string& section, const std::string& key) const {
        auto secIt = data.find(section);
        if (secIt != data.end()) {
            auto keyIt = secIt->second.find(key);
            if (keyIt != secIt->second.end()) {
                return keyIt->second;
            }
        }
        return "UNKNOWN";
    }

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;

    // INIファイルをロードする
    void load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return;
        }

        std::string line, currentSection;

        while (std::getline(file, line)) {
            // コメントや空行を無視
            if (line.empty() || line[0] == ';' || line[0] == '#') {
                continue;
            }

            // セクションをパース
            if (line.front() == '[' && line.back() == ']') {
                currentSection = line.substr(1, line.size() - 2);
            }
            // キーと値をパース
            else {
                std::string key, value;
                std::istringstream lineStream(line);
                if (std::getline(lineStream, key, '=') && std::getline(lineStream, value)) {
                    // 余分な空白を除去
                    key = trim(key);
                    value = trim(value);
                    data[currentSection][key] = value;
                }
            }
        }
    }

    // 文字列の余分な空白を除去
    std::string trim(const std::string& str) const {
        const char* whitespace = " \t\n\r\f\v";
        size_t start = str.find_first_not_of(whitespace);
        size_t end = str.find_last_not_of(whitespace);

        if (start == std::string::npos || end == std::string::npos) {
            return "";
        }
        return str.substr(start, end - start + 1);
    }
};