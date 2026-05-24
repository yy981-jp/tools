// boostヘッダを使うので、debug.hとは分けておきたい
#pragma once
#include <string>
#include <boost/type_index.hpp>

/**
 * @file type.h
 * @brief 型名取得ユーティリティ
 *
 * Boost.TypeIndex を利用して、テンプレート引数の型名を
 * CV修飾子・参照修飾子付きで人間が読みやすい文字列として返します。
 *
 * @note debug.h とは Boost ヘッダの依存関係を分離するために独立したファイルです
 */

/**
 * @brief 変数の型名を文字列で返す
 * @tparam T 型名を取得したい型（自動推論）
 * @param  (unnamed) 型を推論するための参照引数（値は使用しない）
 * @return CV修飾・参照修飾を含む完全な型名文字列（例: "const int &"）
 *
 * @code
 * int x = 42;
 * std::cout << getType(x); // "int"
 * const double& r = 3.14;
 * std::cout << getType(r); // "double const&"
 * @endcode
 */
template <typename T>
std::string getType(const T&) {
	return boost::typeindex::type_id_with_cvr<T>().pretty_name();
}
