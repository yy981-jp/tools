#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

/**
 * @file INIParser.h
 * @brief INI ファイルパーサー
 *
 * セクションとキーバリュー形式の INI ファイルを読み込み、
 * セクション名とキーを指定して値を取得できます。
 * コメント行（; または #）と空行は無視されます。
 */

/**
 * @class INIParser
 * @brief INI ファイルを解析してキーと値を取得するクラス
 *
 * @code
 * INIParser parser("config.ini");
 * std::string host = parser.get("Server", "host");
 * @endcode
 */
class INIParser {
public:
    /**
     * @brief コンストラクタ。INI ファイルをロードして解析する
     * @param filename 読み込む INI ファイルのパス
     */
    INIParser(const std::string& filename) {
        load(filename);
    }

    /**
     * @brief 指定セクション・キーに対応する値を取得する
     * @param section セクション名（例: "Database"）
     * @param key     キー名（例: "host"）
     * @return 対応する値の文字列。見つからない場合は "UNKNOWN"
     */
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
    /// @brief [セクション名][キー名] → 値 の二重マップ
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;

    /**
     * @brief INI ファイルを読み込んで data に格納する
     * @param filename 読み込む INI ファイルのパス
     */
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

    /**
     * @brief 文字列の先頭・末尾にある空白文字を除去する
     * @param str トリム対象の文字列
     * @return 両端の空白を除去した文字列
     */
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
