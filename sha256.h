/**
 * @file sha256.h
 * @brief SHA-256ハッシュ値を計算する機能を提供
 * @details OpenSSL/EVPライブラリを使用したSHA-256ハッシュ計算
 * @author yy981
 * @version 1.0
 * @note OpenSSL(libssl-dev)が必要
 */
#pragma once
#include <filesystem>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <vector>
#include <openssl/evp.h>

namespace fs = std::filesystem;


/**
 * @brief 文字列のSHA-256ハッシュ値を計算
 * @param data ハッシュ対象の文字列
 * @return 64文字の16進数ハッシュ値文字列
 * @exception std::runtime_error EVP操作に失敗した場合
 * @details
 * OpenSSL EVP APIを使用して標準的なSHA-256を計算
 * 返される文字列は小文字の16進数表記（ゼロ埋めなし）
 */
inline std::string sha256(const std::string& data) {
	// OpenSSL初期化
	EVP_MD_CTX* ctx = EVP_MD_CTX_new();
	if (!ctx) {
		throw std::runtime_error("EVP_MD_CTX_newに失敗");
	}

	const EVP_MD* md = EVP_sha256();
	if (EVP_DigestInit_ex(ctx, md, nullptr) != 1) {
		EVP_MD_CTX_free(ctx);
		throw std::runtime_error("EVP_DigestInit_exに失敗");
	}

	// ハッシュ計算
	if (EVP_DigestUpdate(ctx, data.data(), data.size()) != 1) {
		EVP_MD_CTX_free(ctx);
		throw std::runtime_error("EVP_DigestUpdateに失敗");
	}

	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int hashLength = 0;
	if (EVP_DigestFinal_ex(ctx, hash, &hashLength) != 1) {
		EVP_MD_CTX_free(ctx);
		throw std::runtime_error("EVP_DigestFinal_exに失敗");
	}

	EVP_MD_CTX_free(ctx);

	// 文字列化
	std::ostringstream oss;
	for (unsigned int i = 0; i < hashLength; ++i) {
		oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
	}

	return oss.str();
}

/**
 * @brief ファイルのSHA-256ハッシュ値を計算
 * @param filePath ハッシュ対象のファイルパス
 * @return 64文字の16進数ハッシュ値文字列、またはエラー時は"ERROR"
 * @exception std::runtime_error EVP操作に失敗した場合
 * @details
 * ファイルを8192バイトのチャンクで読み込みながらハッシュを計算
 * 大きなファイルでもメモリ効率的に処理可能
 */
inline std::string sha256f(const fs::path& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) return "ERROR";

    // OpenSSL初期化
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        throw std::runtime_error("EVP_MD_CTX_newに失敗");
    }

    const EVP_MD* md = EVP_sha256();
    if (EVP_DigestInit_ex(ctx, md, nullptr) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("EVP_DigestInit_exに失敗");
    }

    // ハッシュ計算
    std::vector<char> buffer(8192);
    while (file) {
        file.read(buffer.data(), buffer.size());
        if (EVP_DigestUpdate(ctx, buffer.data(), file.gcount()) != 1) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("EVP_DigestUpdateに失敗");
        }
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLength = 0;
    if (EVP_DigestFinal_ex(ctx, hash, &hashLength) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("EVP_DigestFinal_exに失敗");
    }

    EVP_MD_CTX_free(ctx);

    // 文字列化
    std::ostringstream oss;
    for (unsigned int i = 0; i < hashLength; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    return oss.str();
}