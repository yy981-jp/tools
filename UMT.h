// unordered_map text
/*
	O:	key: value
	X:	key:value
	X:	key : value
*/
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>


class UMT {
public:
    UMT(const std::string& filename): fname(filename) {
		std::ifstream ifs(fname);
		if (!ifs) throw std::runtime_error("UMT::UMT() ifstream");
		std::string line;
		while (std::getline(ifs,line)) separate(line);
	}
	
	std::string& operator[](const std::string& key) {
		return data[key];
	}
	
    const std::string& operator[](const std::string& key) const {
		static const std::string empty;
        auto it = data.find(key);
        if (it != data.end()) return it->second;
		else return empty;
    }
	
	UMT& operator-=(const std::string& key) {
		data.erase(key);
		save();
		return *this;
	}
	
	UMT& operator+=(const std::string& keyAndValue) {
		if (!keyAndValue.contains(separator)) throw std::runtime_error("UMT::operator+= noContains separator");
		separate(keyAndValue);
		save();
		return *this;
	}


	static std::string set(const std::string& key, const std::string& value) {
		return key + separator + value;
	}
	
    std::unordered_map<std::string, std::string> data;

	void save() {
		std::ofstream ofs(fname);
		if (!ofs) throw std::runtime_error("UMT::save() ofstream");
		std::string content;
		for (const auto& [key,value]: data) {
			content += UMT::set(key,value) + "\n";
		}
		ofs << content;
	}

private:
	std::string fname;
	
	void separate(const std::string& in) {
		if (!in.contains(separator)) throw std::runtime_error("UMT::separate() noContains separator");
		size_t separatorPos = in.find(separator);
		data.emplace(in.substr(0,separatorPos),in.substr(separatorPos+2));
	}
	
	static constexpr std::string separator = ": ";
};