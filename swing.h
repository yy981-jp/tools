/**
 * @file swing.h
 * @brief 往復カウンタクラス
 *
 * 上限・下限の間を行き来する整数値を管理します。
 * アニメーションや周期的な数値変化に利用できます。
 */

/**
 * @brief 上下限の間を往復する整数カウンタ
 *
 * operator() を呼ぶたびに値が +1 または -1 され、
 * 上限に達したら折り返して下降、下限に達したら折り返して上昇します。
 *
 * @code
 * swing s(0, 3);
 * for (int i = 0; i < 8; i++) std::cout << s() << " ";
 * // 出力: 1 2 3 2 1 0 1 2
 * @endcode
 */
class swing {
public:
	/**
	 * @brief コンストラクタ
	 * @param lower_bound カウンタの最小値
	 * @param upper_bound カウンタの最大値
	 */
	swing(int lower_bound, int upper_bound) : lower_bound(lower_bound), upper_bound(upper_bound) {}

	/**
	 * @brief カウンタを1ステップ進め、現在の値を返す
	 * @return 更新後の現在値
	 */
	int operator()() {
		if (up) {
			if (value < upper_bound)
				return ++value;
			else {
				up = false;
				return --value;
			}
		} else {
			if (value > lower_bound)
				return --value;
			else {
				up = true;
				return ++value;
			}
		}
	}

	/**
	 * @brief カウンタの現在値を直接設定する
	 * @param v 設定する値
	 * @return *this への参照
	 */
	swing& operator=(int v) {
		value = v;
		return *this;
	}

	/**
	 * @brief 現在の整数値を取得する（`int v = +s;` のように使用）
	 * @return 現在の値
	 */
	int operator+() const { return value; } // 以下のように値を抽出する i(int)+s(swing)
private:
	/// @brief 現在のカウンタ値
	int value = 0;
	/// @brief 下限値
	int lower_bound;
	/// @brief 上限値
	int upper_bound;
	/// @brief 現在上昇中かどうか
	bool up = true;
};
