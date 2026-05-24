#include <vector>
#include <algorithm>

/**
 * @file vector.h
 * @brief std::vector 補助ユーティリティ
 *
 * 標準ライブラリの std::vector に対して便利な補助関数を提供します。
 */

/**
 * @namespace vct
 * @brief vector ユーティリティ用名前空間
 */
namespace vct {
	/**
	 * @brief vector に特定の値が含まれているか調べる
	 * @tparam T ベクタの要素型
	 * @param vec   検索対象のベクタ
	 * @param value 検索する値
	 * @return 値が含まれていれば true、含まれていなければ false
	 */
	template<typename T>
	bool contains(const std::vector<T>& vec, const T& value) {
		return std::find(vec.begin(), vec.end(), value) != vec.end();
	}
}
