/**
 * @file vector.h
 * @brief std::vector のユーティリティ機能を提供
 * @details ベクトル内の要素検索など
 * @author yy981
 * @version 1.0
 */
#include <vector>
#include <algorithm>

/**
 * @namespace vct
 * @brief ベクトル操作関数の名前空間
 */
namespace vct {
	/**
	 * @brief ベクトルが特定の値を含むか判定
	 * @tparam T ベクトルの要素型
	 * @param vec 検索対象のベクトル
	 * @param value 検索する値
	 * @return valueが vecに含まれる場合true、そうでない場合false
	 */
	template<typename T>
	bool contains(const std::vector<T>& vec, const T& value) {
		return std::find(vec.begin(), vec.end(), value) != vec.end();
	}
}