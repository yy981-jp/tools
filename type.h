/**
 * @file type.h
 * @brief テンプレート型の名前を取得する機能を提供
 * @details Boostライブラリを使用して型情報を人間が読める形式で出力
 * @author yy981
 * @version 1.0
 * @note Boost.TypeIndex (boost/type_index.hpp)が必要
 * @warning debugヘッダと分離（Boostヘッダへの依存を分離するため）
 */
// boostヘッダを使うので、debug.hとは分けておきたい
#pragma once
#include <string>
#include <boost/type_index.hpp>

/**
 * @brief テンプレート型T の型名を取得
 * @tparam T 型名を取得する型
 * @param 使用しない（型推論用ダミー）
 * @return Tの型名を表す文字列
 * @details
 * 例: int -> "int", std::vector<int> -> "std::vector<int, std::allocator<int> >"
 */
template <typename T>
std::string getType(const T&) {
	return boost::typeindex::type_id_with_cvr<T>().pretty_name();
}
