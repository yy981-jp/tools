#pragma once
#include <boost/crc.hpp>
#include <iomanip>


std::string calculateCRC32(std::string data) {
	boost::crc_32_type crc;
	crc.process_bytes(data.data(), data.size());

    std::ostringstream oss;
    oss << std::hex << std::setw(8) << std::setfill('0') << crc.checksum(); // 8桁の16進数としてゼロ埋め

    return oss.str();
}