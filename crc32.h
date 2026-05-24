#pragma once
#include <boost/crc.hpp>
#include <iomanip>

/**
 * @file crc32.h
 * @brief CRC32 チェックサム計算ユーティリティ
 *
 * Boost.CRC を使用して文字列の CRC32 チェックサムを計算します。
 */

/**
 * @brief 文字列の CRC32 チェックサムを計算する
 * @param data チェックサムを計算する対象の文字列
 * @return 8桁ゼロ埋め16進数形式の CRC32 チェックサム文字列（例: "1a2b3c4d"）
 */
std::string calculateCRC32(std::string data) {
	boost::crc_32_type crc;
	crc.process_bytes(data.data(), data.size());

    std::ostringstream oss;
    oss << std::hex << std::setw(8) << std::setfill('0') << crc.checksum(); // 8桁の16進数としてゼロ埋め

    return oss.str();
}
