/**
 * @file crc32.h
 * @brief CRC32チェックサムの計算機能を提供
 * @details Boost.CRCライブラリを使用してCRC-32チェックサムを計算
 * @author yy981
 * @version 1.0
 * @note Boost.CRCが必要
 */
#pragma once
#include <boost/crc.hpp>
#include <iomanip>


/**
 * @brief 文字列のCRC32チェックサムを計算
 * @param data チェックサム対象の文字列
 * @return 8桁16進数のチェックサム文字列
 * 
 * @details
 * Boost.CRCライブラリを使用してCRC-32チェックサムを計算し、
 * 8桁の16進数（ゼロ埋め）として返します
 */
std::string calculateCRC32(std::string data) {
	boost::crc_32_type crc;
	crc.process_bytes(data.data(), data.size());

    std::ostringstream oss;
    oss << std::hex << std::setw(8) << std::setfill('0') << crc.checksum(); // 8桁の16進数としてゼロ埋め

    return oss.str();
}