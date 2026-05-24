/**
 * @file INIParser.h
 * @brief INIファイルのパースと値取得を行うクラスを提供する
 * @details INI形式の設定ファイルを読み込み、セクションとキーから値を取得するシンプルなパーサー
 * @author yy981
 * @version 1.0
 * 
 * @note INIファイル形式:
 *   - セクション: [SectionName]
 *   - キー=値: key=value
 *   - コメント: ; または # で始まる行
 */

#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

/**
 * @class INIParser
 * @brief INIファイルをパースして設定値を管理するクラス
 * 
 * @details
 * INI形式の設定ファイルを読み込み、セクション名とキーを指定して値を取得できます。
 * ファイルの読み込みはコンストラクタで自動的に実行されます。
 * 存在しないキーに対しては"UNKNOWN"を返します。
 */
class INIParser {
public:
    /**
     * @brief コンストラクタ - INIファイルを読み込む
     * @param filename パースするINIファイルのパス
     * @exception std::runtime_error ファイルが開けない場合
     */
    INIParser(const std::string& filename) {
        load(filename);
    }

    /**
     * @brief 指定されたセクションとキーの値を取得
     * @param section セクション名
     * @param key キー名
     * @return 対応する値、存在しない場合は"UNKNOWN"
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
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data; ///< セクション->キー->値のマッピング

    /**
     * @brief INIファイルを読み込んでパースする
     * @param filename ファイルパス
     * @exception std::cerr ファイルが開けない場合はエラーを標準エラー出力に出力
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
     * @brief 文字列の前後の空白を除去
     * @param str 処理対象の文字列
     * @return トリミングされた文字列
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