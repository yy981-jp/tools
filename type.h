// boostヘッダを使うので、debug.hとは分けておきたい
#pragma once
#include <string>
#include <boost/type_index.hpp>

template <typename T>
std::string getType(const T&) {
	return boost::typeindex::type_id_with_cvr<T>().pretty_name();
}
