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
 * @file sha256.h
 * @brief SHA-256 ハッシュ計算ユーティリティ
 *
 * OpenSSL の EVP インターフェースを使用して、文字列またはファイルの
 * SHA-256 ハッシュ値を 64 文字の小文字16進数文字列で返します。
 */

/**
 * @brief 文字列の SHA-256 ハッシュ値を計算する
 * @param data ハッシュを計算する入力文字列
 * @return 64 文字の小文字16進数 SHA-256 ハッシュ文字列
 * @throws std::runtime_error OpenSSL の初期化・計算に失敗した場合
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
 * @brief ファイルの SHA-256 ハッシュ値を計算する
 * @param filePath ハッシュを計算するファイルのパス
 * @return 64 文字の小文字16進数 SHA-256 ハッシュ文字列。ファイルを開けない場合は "ERROR"
 * @throws std::runtime_error OpenSSL の初期化・計算に失敗した場合
 * @note ファイルは 8192 バイト単位でストリーム読み込みします
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
