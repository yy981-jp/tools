#pragma once
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <openssl/evp.h>

namespace fs = std::filesystem;


std::string sha256(const std::string& data) {
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

std::string sha256f(const fs::path& filePath) {
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