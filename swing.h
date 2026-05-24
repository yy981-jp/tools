/**
 * @file swing.h
 * @brief 範囲内で振動(上下)する整数値を生成するクラス
 * @details 指定範囲内で継続的に数値が増減し、上限・下限で折り返す
 * @author yy981
 * @version 1.0
 */

/**
 * @class swing
 * @brief 下限と上限の間を振動する値を管理
 * 
 * @details
 * - operator()で次の値を取得
 * - 最初は上昇、上限到達で下降に切り替わる
 * - 下限到達で再び上昇に切り替わる
 * - operator=で初期値を設定可能
 * - operator+で現在値を取得可能
 */
class swing {
public:
	/**
	 * @brief コンストラクタ
	 * @param lower_bound 下限値
	 * @param upper_bound 上限値
	 */
	swing(int lower_bound, int upper_bound) : lower_bound(lower_bound), upper_bound(upper_bound) {}
	
	/**
	 * @brief 次の値を取得して内部状態を更新
	 * @return 現在の値（更新後）
	 * @details
	 * 上昇時: 値を+1、上限到達時は-1へ切り替え
	 * 下降時: 値を-1、下限到達時は+1へ切り替え
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
	 * @brief 値を設定（初期化）
	 * @param v 設定する値
	 * @return このインスタンスへの参照
	 */
	swing& operator=(int v) {
		value = v;
		return *this;
	}
	
	/**
	 * @brief 現在値を取得（operator()とは異なり状態を更新しない）
	 * @return 現在の値
	 * @details 使用例: int val = +s (int型キャスト用にoperator+を使用)
	 */
	int operator+() const { return value; } // 以下のように値を抽出する i(int)+s(swing)
private:
	int value = 0; ///< 現在値
	int lower_bound, upper_bound; ///< 下限・上限
	bool up = true; ///< 上昇フラグ
};